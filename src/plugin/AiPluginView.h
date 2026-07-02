// ##Script function and purpose: Defines the visual integration of the AI plugin within Kate.
#pragma once

#include <KTextEditor/Document>
#include <KTextEditor/MainWindow>
#include <KTextEditor/View>
#include <KTextEditor/MovingRange>
#include <QMenu>
#include <KXMLGUIClient>
#include <QObject>
#include <QPointer>

class AiPlugin;
class AiChatWidget;
class AiCompletionModel;
class LlamaClient;
#include <QPointer>
class QWidget;

// ##Class purpose: Manages tool views, actions, and connections for a single Kate main window.
class AiPluginView : public QObject, public KXMLGUIClient
{
    Q_OBJECT

public:
    // ##Method purpose: Constructor for the plugin view.
    explicit AiPluginView(AiPlugin *plugin, KTextEditor::MainWindow *mainWindow);
    
    // ##Method purpose: Destructor.
    ~AiPluginView() override;

private Q_SLOTS:
    // ##Method purpose: Handles switching between different editor tabs to re-register models.
    void onActiveViewChanged(KTextEditor::View *view);
    
    // ##Method purpose: Slot triggered by the "AI Refactor" action in the tools menu.
    void requestAiRefactor();

    // ##Method purpose: Slot to insert actions into the view's context menu.
    void onContextMenuAboutToShow(KTextEditor::View *view, QMenu *menu);

    // ##Method purpose: Slot triggered when the LLM returns refactored text.
    void onRefactorReceived(const QString &text);

private:
    // ##Method purpose: Sets up KXMLGUI actions and connects them to slots.
    void setupActions();
    
    // ##Method purpose: Initializes the sidebar tool view and places the chat widget inside.
    void createToolView();

    AiPlugin *m_plugin;
    QAction *m_refactorAction = nullptr;
    KTextEditor::MainWindow *m_mainWindow;
    QWidget *m_toolView;
    AiChatWidget *m_chatWidget;
    AiCompletionModel *m_completionModel;
    LlamaClient *m_refactorClient;
    KTextEditor::MovingRange *m_currentRefactorRange = nullptr;
    QPointer<KTextEditor::Document> m_refactorDocument;
    QString m_currentRefactorOriginalText;
    QPointer<KTextEditor::View> m_activeView;
};
