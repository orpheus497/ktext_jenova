// ##Script function and purpose: Implements the visual integration, handling UI creation and action bindings.
#include "AiPluginView.h"
#include "AiPlugin.h"
#include "../ui/AiChatWidget.h"
#include <KTextEditor/Document>
#include <KTextEditor/Message>
#include "../completion/AiCompletionModel.h"
#include "../network/LlamaClient.h"
#include "../context/ContextManager.h"

#include <KActionCollection>
#include <KLocalizedString>
#include <QAction>
#include <QVBoxLayout>
#include <KXMLGUIFactory>
#include <QMenu>
#include <QInputDialog>

// ##Method purpose: Initializes the view, creates actions, tool views, and connects signals.
AiPluginView::AiPluginView(AiPlugin *plugin, KTextEditor::MainWindow *mainWindow)
    : QObject(mainWindow)
    , KXMLGUIClient()
    , m_plugin(plugin)
    , m_mainWindow(mainWindow)
    , m_toolView(nullptr)
    , m_completionModel(new AiCompletionModel(this))
    , m_refactorClient(new LlamaClient(this))
{
    setComponentName(QStringLiteral("jenovaktext"), i18n("Jenova K Text"));
    setXMLFile(QStringLiteral("jenovaktextui.rc"));

    connect(m_refactorClient, &LlamaClient::refactorReceived, this, &AiPluginView::onRefactorReceived);
    connect(m_refactorClient, &LlamaClient::errorOccurred, this, [this](const QString &err) {
        if (m_refactorDocument && m_currentRefactorRange) {
            delete m_currentRefactorRange;
            m_currentRefactorRange = nullptr;
            auto *msg = new KTextEditor::Message(i18n("AI Refactor failed: %1", err), KTextEditor::Message::Error);
            msg->setPosition(KTextEditor::Message::TopInView);
            m_refactorDocument->postMessage(msg);
        }
    });

    setupActions();
    createToolView();

    m_mainWindow->guiFactory()->addClient(this);

    connect(m_mainWindow, &KTextEditor::MainWindow::viewChanged, this, &AiPluginView::onActiveViewChanged);
    onActiveViewChanged(m_mainWindow->activeView());
}

// ##Method purpose: Cleans up the view and removes it from the KXMLGUI factory.
AiPluginView::~AiPluginView()
{
    m_mainWindow->guiFactory()->removeClient(this);
    // ##Condition purpose: Ensure tool view is safely deleted if it exists.
    if (m_toolView) {
        delete m_toolView;
    }
}

// ##Method purpose: Registers the custom AI actions into the application's action collection.
void AiPluginView::setupActions()
{
    m_refactorAction = actionCollection()->addAction(QStringLiteral("ai_refactor"));
    m_refactorAction->setText(i18n("AI: Refactor Selection..."));
    connect(m_refactorAction, &QAction::triggered, this, &AiPluginView::requestAiRefactor);
}

// ##Method purpose: Constructs the dockable tool view in the right sidebar.
void AiPluginView::createToolView()
{
    m_toolView = m_mainWindow->createToolView(
        m_plugin,
        QStringLiteral("jenova_ktext_assistant"),
        KTextEditor::MainWindow::Right,
        QIcon::fromTheme(QStringLiteral("code-context")),
        i18n("Jenova K Text")
    );

    if (!m_toolView) return;

    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(m_toolView->layout());
    if (!layout) {
        layout = new QVBoxLayout(m_toolView);
    }
    
    layout->setContentsMargins(0, 0, 0, 0);
    m_chatWidget = new AiChatWidget(m_mainWindow, m_toolView);
    m_chatWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(m_chatWidget);
}

// ##Method purpose: Unregisters the completion model from the old view and registers it on the new one.
void AiPluginView::onActiveViewChanged(KTextEditor::View *view)
{
    // ##Condition purpose: Clean up old view registration.
    if (m_activeView) {
        m_activeView->unregisterCompletionModel(m_completionModel);
    }

    m_activeView = view;

    // ##Condition purpose: Register on the new view.
    if (m_activeView) {
        m_activeView->registerCompletionModel(m_completionModel);
    }
}

// ##Method purpose: Handles the execution of the refactor action, asking user for instruction.
void AiPluginView::requestAiRefactor()
{
    // ##Condition purpose: Validate that a document and selection exist before processing.
    if (!m_activeView || !m_activeView->document() || !m_activeView->selection()) {
        return;
    }
    
    bool ok;
    QString instruction = QInputDialog::getText(
        m_mainWindow->window(),
        i18n("AI Refactor"),
        i18n("What should the AI do with this selection? (e.g. 'Optimize', 'Rename X to Y')"),
        QLineEdit::Normal,
        QString(),
        &ok
    );
    
    if (!ok || instruction.isEmpty()) {
        return;
    }
    
    if (m_refactorDocument && m_currentRefactorRange) {
        delete m_currentRefactorRange;
    }
    
    m_refactorDocument = m_activeView->document();
    m_currentRefactorRange = m_refactorDocument->newMovingRange(m_activeView->selectionRange());
    m_currentRefactorOriginalText = m_activeView->selectionText();
    
    ContextManager contextMgr(this);
    QString promptText = contextMgr.buildRefactorPrompt(instruction, m_currentRefactorOriginalText, m_activeView.data());
    
    m_refactorClient->requestRefactor(promptText);
}

// ##Method purpose: Handles the response from the LLM and shows the comparison view.
void AiPluginView::onRefactorReceived(const QString &text)
{
    if (!m_refactorDocument) {
        m_currentRefactorRange = nullptr; // destroyed with document
        return;
    }
    
    if (m_currentRefactorRange) {
        m_refactorDocument->replaceText(m_currentRefactorRange->toRange(), text);
        delete m_currentRefactorRange;
        m_currentRefactorRange = nullptr;
    }
    
    
    auto *msg = new KTextEditor::Message(i18n("AI Refactor applied. Press Ctrl+Z to undo."), KTextEditor::Message::Information);
    msg->setPosition(KTextEditor::Message::TopInView);
    m_refactorDocument->postMessage(msg);
}
