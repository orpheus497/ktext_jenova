#include <iostream>
#include <vector>
#include <chrono>

struct CodeCompletionModel {};

class View {
public:
    void registerCompletionModel(CodeCompletionModel* model) {
        volatile int x = 0;
        for (int i=0; i<10; ++i) x += i;
    }
};

class Document {
public:
    std::vector<View*> views_list;
    const std::vector<View*>& views() const { return views_list; }
};

class DocumentController {
public:
    std::vector<Document*> docs_list;
    const std::vector<Document*>& openDocuments() const { return docs_list; }
};

int main() {
    DocumentController ctrl;
    for (int i = 0; i < 1000; ++i) {
        auto doc = new Document();
        for (int j = 0; j < 5; ++j) {
            doc->views_list.push_back(new View());
        }
        ctrl.docs_list.push_back(doc);
    }

    CodeCompletionModel* m_completionModel = new CodeCompletionModel();

    auto start1 = std::chrono::high_resolution_clock::now();
    for (auto* doc : ctrl.openDocuments()) {
        auto* textDoc = doc; // In KDevelop, doc->textDocument()
        if (textDoc) {
            for (auto* view : textDoc->views()) {
                view->registerCompletionModel(m_completionModel);
            }
        }
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double1 = end1 - start1;
    std::cout << "Original N+1 openDocuments Loop Baseline: " << ms_double1.count() << " ms\n";

    // Instead of looping through all open documents and all views on startup, we can just hook up to the KTextEditor::Editor directly?
    // Let's check `KTextEditor::Editor::instance()->documents()` for all documents. But wait, `openDocuments()` is actually already a loop over all documents.
    // If the overhead is the N+1 loop (looping over documents, then views), is there a way to get all views directly?
    return 0;
}
