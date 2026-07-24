// ##Script function and purpose: Implements the ChatMessageWidget
#include "ChatMessageWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QClipboard>
#include <QApplication>
#include <QStyle>
#include <QPalette>
#include <QIcon>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <KTextEditor/Document>
#include <KTextEditor/View>
#include <KTextEditor/Editor>
#include <interfaces/icore.h>
#include <interfaces/idocumentcontroller.h>
#include <interfaces/idocument.h>

// ##Method purpose: Constructor.
ChatMessageWidget::ChatMessageWidget(const QString& role, const QString& content, bool isStreaming, QWidget *parent)
    : QWidget(parent), m_role(role), m_content(content), m_isStreaming(isStreaming), m_streamTimer(nullptr)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(10, 10, 10, 10);
    m_layout->setSpacing(6);
    
    applyRoleStyling();
    
    if (m_isStreaming) {
        m_streamTimer = new QTimer(this);
        m_streamTimer->setInterval(50);
        connect(m_streamTimer, &QTimer::timeout, this, [this]() {
            if (!m_pendingStreamToken.isEmpty()) {
                m_content += m_pendingStreamToken;
                m_pendingStreamToken.clear();
                if (m_streamingLabel) m_streamingLabel->setText(m_content);
            }
        });
        m_streamTimer->start();
        m_streamingLabel = new QLabel(this);
        m_streamingLabel->setWordWrap(true);
        m_streamingLabel->setTextFormat(Qt::MarkdownText);
        m_streamingLabel->setText(m_content);
        m_layout->addWidget(m_streamingLabel);
    } else {
        m_streamingLabel = nullptr;
        parseAndBuildUI();
    }
}

// ##Method purpose: Handles native bubble rendering safely without QSS interference.
void ChatMessageWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_bgColor);
    painter.drawRoundedRect(rect(), 8, 8);
}

// ##Method purpose: Sets the background color based on the role.
void ChatMessageWidget::applyRoleStyling()
{
    QPalette pal = palette();
    
    if (m_role == QStringLiteral("user")) {
        m_bgColor = pal.color(QPalette::Active, QPalette::Highlight);
        pal.setColor(QPalette::WindowText, pal.color(QPalette::Active, QPalette::HighlightedText));
    } else if (m_role == QStringLiteral("assistant")) {
        m_bgColor = pal.color(QPalette::Active, QPalette::AlternateBase);
    } else if (m_role == QStringLiteral("error")) {
        m_bgColor = QColor::fromHsl(0, 80, qBound(40, pal.color(QPalette::Active, QPalette::Window).lightness(), 200));
    } else if (m_role == QStringLiteral("warning")) {
        m_bgColor = QColor::fromHsl(40, 80, qBound(40, pal.color(QPalette::Active, QPalette::Window).lightness(), 200));
    } else if (m_role == QStringLiteral("thinking")) {
        QColor highlight = pal.color(QPalette::Active, QPalette::Highlight);
        m_bgColor = QColor::fromHsl(highlight.hslHue(), 40, qBound(60, pal.color(QPalette::Active, QPalette::Window).lightness() + 15, 220));
    } else {
        m_bgColor = pal.color(QPalette::Active, QPalette::Window);
    }
    
    setPalette(pal);
}

// ##Method purpose: Fast append for SSE streaming (only updates a temporary text label).
void ChatMessageWidget::appendStreamToken(const QString& token)
{
    if (!m_isStreaming || !m_streamingLabel) return;
    m_pendingStreamToken += token;
}

// ##Method purpose: Finalizes a stream, parsing the accumulated text into proper blocks.
void ChatMessageWidget::finalizeStream()
{
    if (!m_isStreaming) return;
    m_isStreaming = false;
    
    if (m_streamTimer) {
        m_streamTimer->stop();
        m_streamTimer->deleteLater();
        m_streamTimer = nullptr;
    }
    if (!m_pendingStreamToken.isEmpty()) {
        m_content += m_pendingStreamToken;
        m_pendingStreamToken.clear();
    }
    
    if (m_streamingLabel) {
        m_layout->removeWidget(m_streamingLabel);
        delete m_streamingLabel;
        m_streamingLabel = nullptr;
    }
    
    parseAndBuildUI();
}

// ##Method purpose: Sets the complete content and parses it into blocks.
void ChatMessageWidget::setContent(const QString& content)
{
    m_content = content;
    // Clear existing layout items
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }
    m_streamingLabel = nullptr;
    m_isStreaming = false;
    parseAndBuildUI();
}

// ##Method purpose: Parses markdown and generates the appropriate widgets.
void ChatMessageWidget::parseAndBuildUI()
{
    if (m_content.isEmpty()) return;
    
    QStringList chunks = m_content.split(QStringLiteral("```"));
    for (int i = 0; i < chunks.size(); ++i) {
        QString chunk = chunks[i];
        if (chunk.isEmpty()) continue;
        
        if (i % 2 == 0) {
            // Text block
            buildTextBlock(chunk.trimmed());
        } else {
            // Code block
            int firstNewline = chunk.indexOf(QLatin1Char('\n'));
            QString lang = QStringLiteral("C++"); // default
            QString code = chunk;
            if (firstNewline != -1) {
                QString potentialLang = chunk.left(firstNewline).trimmed();
                if (!potentialLang.isEmpty()) {
                    lang = potentialLang;
                }
                code = chunk.mid(firstNewline + 1);
            }
            buildCodeBlock(lang, code);
        }
    }
}

// ##Method purpose: Builds a word-wrapped QLabel for standard text.
void ChatMessageWidget::buildTextBlock(const QString& text)
{
    if (text.isEmpty()) return;
    auto *label = new QLabel(this);
    label->setWordWrap(true);
    label->setTextFormat(Qt::MarkdownText);
    label->setText(text);
    // Inherit text color from palette
    QPalette pal = label->palette();
    pal.setColor(QPalette::WindowText, this->palette().color(QPalette::WindowText));
    label->setPalette(pal);
    m_layout->addWidget(label);
}

// ##Method purpose: Builds a native KTextEditor block with syntax highlighting.
void ChatMessageWidget::buildCodeBlock(const QString& lang, const QString& code)
{
    // Toolbar
    auto *toolbarLayout = new QHBoxLayout();
    toolbarLayout->setContentsMargins(0, 0, 0, 0);
    
    auto *langLabel = new QLabel(lang, this);
    QFont f = langLabel->font();
    f.setBold(true);
    langLabel->setFont(f);
    toolbarLayout->addWidget(langLabel);
    toolbarLayout->addStretch();
    
    auto *copyBtn = new QPushButton(QIcon::fromTheme(QStringLiteral("edit-copy")), QStringLiteral("Copy"), this);
    auto *applyBtn = new QPushButton(QIcon::fromTheme(QStringLiteral("document-export")), QStringLiteral("Apply to Editor"), this);
    
    connect(copyBtn, &QPushButton::clicked, this, [this, code]() { copyToClipboard(code); });
    connect(applyBtn, &QPushButton::clicked, this, [this, code]() { applyToEditor(code); });
    
    toolbarLayout->addWidget(copyBtn);
    toolbarLayout->addWidget(applyBtn);
    
    m_layout->addLayout(toolbarLayout);
    
    // Editor View
    auto *editor = KTextEditor::Editor::instance();
    if (editor) {
        auto *doc = editor->createDocument(this);
        doc->setText(code);
        doc->setReadWrite(false);
        // Try to set highlighting mode if available
        if (!lang.isEmpty()) {
            doc->setHighlightingMode(lang);
        }
        
        auto *view = doc->createView(this);
        int lines = doc->lines();
        int viewHeight = qMax(80, lines * QFontMetrics(view->font()).lineSpacing() + 10);
        view->setFixedHeight(viewHeight);
        
        m_layout->addWidget(view);
    } else {
        // Fallback if KTextEditor is unavailable
        buildTextBlock(QStringLiteral("```") + lang + QStringLiteral("\n") + code + QStringLiteral("\n```"));
    }
}

// ##Method purpose: Copies the code block to the system clipboard.
void ChatMessageWidget::copyToClipboard(const QString& code)
{
    if (QClipboard *clipboard = QApplication::clipboard()) {
        clipboard->setText(code);
    }
}

// ##Method purpose: Injects the code block into the active KDevelop text editor.
void ChatMessageWidget::applyToEditor(const QString& code)
{
    auto *core = KDevelop::ICore::self();
    if (!core) return;
    auto *docCtrl = core->documentController();
    if (!docCtrl) return;
    auto *activeDoc = docCtrl->activeDocument();
    if (!activeDoc) return;
    auto *view = activeDoc->activeTextView();
    if (!view && activeDoc->textDocument() && !activeDoc->textDocument()->views().isEmpty()) {
        view = activeDoc->textDocument()->views().first();
    }
    if (!view || !view->document()) return;
    
    auto *doc = view->document();
    if (view->selection()) {
        // Replace selection
        doc->replaceText(view->selectionRange(), code);
    } else {
        // Insert at cursor
        doc->insertText(view->cursorPosition(), code);
    }
}
