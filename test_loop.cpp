#include <iostream>
#include <chrono>
#include <vector>

class Document;

class View {
public:
    void registerCompletionModel(void*) {
        volatile int x = 0;
        for(int i=0; i<100; ++i) x += i;
    }
};

class Document {
public:
    std::vector<View*> views_list;
    const std::vector<View*>& views() const { return views_list; }
};

int main() {
    std::vector<Document*> openDocuments;
    for (int i = 0; i < 1000; ++i) {
        auto doc = new Document();
        for (int j = 0; j < 5; ++j) {
            doc->views_list.push_back(new View());
        }
        openDocuments.push_back(doc);
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (auto* doc : openDocuments) {
        for (auto* view : doc->views()) {
            view->registerCompletionModel(nullptr);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double = end - start;

    std::cout << "Original loop time: " << ms_double.count() << " ms\n";

    // Check if there is an optimized way.
    // The user's code has nested loops inside KDevLLMPlugin::KDevLLMPlugin:
    //
    // for (auto* doc : core()->documentController()->openDocuments()) {
    //     if (auto* textDoc = doc->textDocument()) {
    //         connect(textDoc, &KTextEditor::Document::viewCreated, this, [this](KTextEditor::Document*, KTextEditor::View* view) {
    //             view->registerCompletionModel(m_completionModel);
    //         });
    //         for (auto* view : textDoc->views()) {
    //             view->registerCompletionModel(m_completionModel);
    //         }
    //     }
    // }
    //
    // The nested loops iterate over all open documents and their views.
    // However, wait! There is a duplication in the source code! Let's check!
    return 0;
}
