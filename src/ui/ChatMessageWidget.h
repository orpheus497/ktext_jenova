// ##Script function and purpose: Defines the widget that renders a single chat message, parsing markdown and embedding native IDE editors for code blocks.
#pragma once

#include <QWidget>
#include <QString>

class QVBoxLayout;
class QLabel;

// ##Class purpose: Renders a single chat message using QLabels for text and KTextEditor::View for code blocks.
class ChatMessageWidget : public QWidget {
    Q_OBJECT
public:
    // ##Method purpose: Constructor.
    explicit ChatMessageWidget(const QString& role, const QString& content, bool isStreaming = false, QWidget *parent = nullptr);

    // ##Method purpose: Sets the complete content and parses it into blocks.
    void setContent(const QString& content);

    // ##Method purpose: Fast append for SSE streaming (only updates a temporary text label).
    void appendStreamToken(const QString& token);

    // ##Method purpose: Finalizes a stream, parsing the accumulated text into proper blocks.
    void finalizeStream();

    // ##Method purpose: Returns the role of this message.
    QString role() const { return m_role; }

private Q_SLOTS:
    // ##Method purpose: Injects the code block into the active KDevelop text editor.
    void applyToEditor(const QString& code);

    // ##Method purpose: Copies the code block to the system clipboard.
    void copyToClipboard(const QString& code);

private:
    // ##Method purpose: Parses markdown and generates the appropriate widgets.
    void parseAndBuildUI();

    // ##Method purpose: Builds a native KTextEditor block with syntax highlighting.
    void buildCodeBlock(const QString& lang, const QString& code);

    // ##Method purpose: Builds a word-wrapped QLabel for standard text.
    void buildTextBlock(const QString& text);

    // ##Method purpose: Sets the background color based on the role.
    void applyRoleStyling();

    QString m_role;
    QString m_content;
    bool m_isStreaming;
    QVBoxLayout *m_layout;
    QLabel *m_streamingLabel; // Used exclusively during fast token generation.
};
