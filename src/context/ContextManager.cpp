// ##Script function and purpose: Implements the logic for scanning directories and formatting the system prompt.
#include "ContextManager.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

// ##Method purpose: Constructor implementation.
ContextManager::ContextManager(QObject *parent) : QObject(parent) {}

// ##Method purpose: Scans upwards from the document's directory to find .git or CMakeLists.txt.
QString ContextManager::getProjectRoot(KTextEditor::Document *doc) const
{
    // ##Condition purpose: Ensure we have a valid document URL.
    if (!doc || doc->url().isEmpty()) {
        return QString();
    }
    
    QDir dir = QFileInfo(doc->url().toLocalFile()).absoluteDir();
    
    // ##Loop purpose: Traverse up the directory tree to locate the project root indicator.
    while (dir.absolutePath() != QStringLiteral("/")) {
        // ##Condition purpose: Match standard project root markers.
        if (dir.exists(QStringLiteral(".git")) || dir.exists(QStringLiteral("CMakeLists.txt"))) {
            return dir.absolutePath();
        }
        dir.cdUp();
    }
    return QString();
}

// ##Method purpose: Searches for AGENTS.md or .agents/AGENTS.md and reads it.
QString ContextManager::getAgentsInstruction(const QString &projectRoot) const
{
    // ##Condition purpose: Skip if project root is undefined.
    if (projectRoot.isEmpty()) return QString();

    QStringList candidates = {
        QStringLiteral("AGENTS.md"),
        QStringLiteral(".agents/AGENTS.md")
    };

    // ##Loop purpose: Check all possible locations for the AGENTS.md file.
    for (const auto &candidate : candidates) {
        QFile file(QDir(projectRoot).filePath(candidate));
        
        // ##Condition purpose: Only read the file if we can successfully open it.
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            return in.readAll();
        }
    }
    return QString();
}

// ##Method purpose: Assembles the final system prompt string containing the root instructions and selection.
QString ContextManager::buildSystemPrompt(KTextEditor::View *view) const
{
    QString prompt = QStringLiteral("You are an expert AI coding assistant integrated into the Kate/KDevelop editor.\n");
    
    // ##Condition purpose: Only append specific context if a valid view and document are active.
    if (view && view->document()) {
        QString root = getProjectRoot(view->document());
        QString agentsInst = getAgentsInstruction(root);
        
        // ##Condition purpose: Inject AGENTS.md instructions if found.
        if (!agentsInst.isEmpty()) {
            prompt += QStringLiteral("\nFollow these project-specific instructions from AGENTS.md:\n");
            prompt += agentsInst + QStringLiteral("\n");
        }
        
        prompt += QStringLiteral("\nCurrent file: ") + view->document()->url().toLocalFile() + QStringLiteral("\n");
        prompt += QStringLiteral("\n--- File Content ---\n```\n");
        prompt += view->document()->text();
        prompt += QStringLiteral("\n```\n");
        
        // ##Condition purpose: Inject the user's active text selection into the prompt context.
        if (view->selection()) {
            prompt += QStringLiteral("\nThe user has selected the following code:\n```\n");
            prompt += view->selectionText();
            prompt += QStringLiteral("\n```\n");
        }
    }
    
    return prompt;
}

// ##Method purpose: Builds a context-aware prompt for the AI refactoring action.
QString ContextManager::buildRefactorPrompt(const QString &instruction, const QString &code, KTextEditor::View *view) const
{
    QString prompt = QStringLiteral("You are an expert developer. ");
    
    if (view && view->document()) {
        prompt += QStringLiteral("You are working in the file: ") + view->document()->url().toLocalFile() + QStringLiteral("\n\n");
        prompt += QStringLiteral("Here is the full content of the file for context:\n```\n");
        prompt += view->document()->text();
        prompt += QStringLiteral("\n```\n\n");
    }
    
    prompt += QStringLiteral("The user has selected the following code to modify:\n```\n");
    prompt += code;
    prompt += QStringLiteral("\n```\n\n");
    
    prompt += QStringLiteral("Instruction: ") + instruction + QStringLiteral("\n\n");
    prompt += QStringLiteral("Please output ONLY the resulting modified code block to replace the selection. Do not include any conversational text or markdown wrappers in your output. Only raw code.");
    
    return prompt;
}
