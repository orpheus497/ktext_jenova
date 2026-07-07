#include <iostream>
#include <vector>
#include <chrono>
#include <functional>

struct CodeCompletionModel {};

class View {
public:
    void registerCompletionModel(CodeCompletionModel* model) {
        // dummy delay
        volatile int x = 0;
        for (int i=0; i<100; ++i) x += i;
    }
    void unregisterCompletionModel(CodeCompletionModel* model) {}
};

class Document {
public:
    std::vector<View*> views_list;
    const std::vector<View*>& views() const { return views_list; }
    void viewCreated(Document*, View*) {}
};

class DocumentController {
public:
    std::vector<Document*> docs_list;
    const std::vector<Document*>& openDocuments() const { return docs_list; }
    void textDocumentCreated(Document*) {}
};

class Core {
public:
    DocumentController* docCtrl = new DocumentController();
    DocumentController* documentController() { return docCtrl; }
};

int main() {
    Core core;
    for (int i = 0; i < 1000; ++i) {
        auto doc = new Document();
        for (int j = 0; j < 5; ++j) {
            doc->views_list.push_back(new View());
        }
        core.docCtrl->docs_list.push_back(doc);
    }

    CodeCompletionModel* m_completionModel = new CodeCompletionModel();

    auto start = std::chrono::high_resolution_clock::now();

    // Original behavior
    for (auto* doc : core.documentController()->openDocuments()) {
        auto* textDoc = doc; // Simulating doc->textDocument()
        if (textDoc) {
            for (auto* view : textDoc->views()) {
                view->registerCompletionModel(m_completionModel);
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double1 = end - start;
    std::cout << "Original loop time: " << ms_double1.count() << " ms\n";

    return 0;
}
