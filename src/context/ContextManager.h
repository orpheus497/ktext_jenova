// ##Script function and purpose: Manages the collection of context data (AGENTS.md, selection) for the LLM.
#pragma once
#include <QObject>
#include <QString>
#include <KTextEditor/Document>
#include <KTextEditor/View>

// ##Class purpose: Utility class for finding project roots and aggregating context for prompts.
class ContextManager : public QObject {
    Q_OBJECT
public:
    // ##Method purpose: Constructor.
    explicit ContextManager(QObject *parent = nullptr);
    
    // ##Method purpose: Destructor.
    ~ContextManager() override = default;

    // ##Method purpose: Determines the project root directory from a given document.
    QString getProjectRoot(KTextEditor::Document *doc) const;

    // ##Method purpose: Reads and returns the contents of AGENTS.md if found in the project root.
    QString getAgentsInstruction(const QString &projectRoot) const;

    // ##Method purpose: Constructs the initial system prompt containing all gathered context.
    QString buildSystemPrompt(KTextEditor::View *view) const;
};
