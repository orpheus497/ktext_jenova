#include <iostream>
#include <vector>
#include <chrono>

struct CodeCompletionModel {};

class View {
public:
    void registerCompletionModel(CodeCompletionModel* model) {
        // Simulating minor overhead
        volatile int x = 0;
        for (int i=0; i<100; ++i) x += i;
    }
};

class Document {
public:
    std::vector<View*> views_list;
    const std::vector<View*>& views() const { return views_list; }
    void viewCreated(Document*, View*) {}
};

class IDocument {
public:
    Document* textDoc;
    Document* textDocument() { return textDoc; }
};

class DocumentController {
public:
    std::vector<IDocument*> docs_list;
    const std::vector<IDocument*>& openDocuments() const { return docs_list; }
    void textDocumentCreated(IDocument*) {}
};

int main() {
    DocumentController ctrl;

    // Simulate opening many documents and views
    for (int i = 0; i < 1000; ++i) {
        auto idoc = new IDocument();
        idoc->textDoc = new Document();
        for (int j = 0; j < 5; ++j) {
            idoc->textDoc->views_list.push_back(new View());
        }
        ctrl.docs_list.push_back(idoc);
    }

    CodeCompletionModel* m_completionModel = new CodeCompletionModel();

    // Measure original duplicate method
    auto start1 = std::chrono::high_resolution_clock::now();
    for (auto* doc : ctrl.openDocuments()) {
        if (auto* textDoc = doc->textDocument()) {
            // Simulated connect viewCreated
            for (auto* view : textDoc->views()) {
                view->registerCompletionModel(m_completionModel);
            }
        }
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double1 = end1 - start1;

    // Reset/clear overhead is negligible for measurement. We are measuring the exact same code execution,
    // but demonstrating the logic is equivalent. Since the actual C++ code generated is virtually identical,
    // we measure the refactored method here:
    auto setupTextDocument = [&](IDocument* doc) {
        if (auto* textDoc = doc->textDocument()) {
            for (auto* view : textDoc->views()) {
                view->registerCompletionModel(m_completionModel);
            }
        }
    };

    auto start2 = std::chrono::high_resolution_clock::now();
    for (auto* doc : ctrl.openDocuments()) {
        setupTextDocument(doc);
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double2 = end2 - start2;

    std::cout << "Original Method Time:   " << ms_double1.count() << " ms\n";
    std::cout << "Refactored Method Time: " << ms_double2.count() << " ms\n";
    std::cout << "Note: Performance change is minor as it was primarily a deduplication and code-size optimization.\n";
    return 0;
}
