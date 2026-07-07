#include <iostream>
#include <vector>
#include <chrono>

struct CodeCompletionModel {};

class View {
public:
    void registerCompletionModel(CodeCompletionModel* model) {
        volatile int x = 0;
        for (int i=0; i<100; ++i) x += i;
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
    std::vector<View*> flat_views; // If we could get a flat list of views...

    for (int i = 0; i < 1000; ++i) {
        auto doc = new Document();
        for (int j = 0; j < 5; ++j) {
            auto view = new View();
            doc->views_list.push_back(view);
            flat_views.push_back(view);
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
    std::cout << "Original loop: " << ms_double1.count() << " ms\n";

    auto start2 = std::chrono::high_resolution_clock::now();
    for (auto* view : flat_views) {
        view->registerCompletionModel(m_completionModel);
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double2 = end2 - start2;
    std::cout << "Flat list loop: " << ms_double2.count() << " ms\n";

    return 0;
}
