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

class Editor {
public:
    static Editor* instance() { static Editor e; return &e; }
    std::vector<Document*> docs_list;
    const std::vector<Document*>& documents() const { return docs_list; }
    std::vector<View*> views_list; // Simulated Editor views
};

int main() {
    auto editor = Editor::instance();
    for (int i = 0; i < 1000; ++i) {
        auto doc = new Document();
        for (int j = 0; j < 5; ++j) {
            auto view = new View();
            doc->views_list.push_back(view);
            editor->views_list.push_back(view); // Simulated flat list
        }
        editor->docs_list.push_back(doc);
    }

    CodeCompletionModel* m_completionModel = new CodeCompletionModel();

    auto start1 = std::chrono::high_resolution_clock::now();
    for (auto* doc : editor->documents()) {
        for (auto* view : doc->views()) {
            view->registerCompletionModel(m_completionModel);
        }
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double1 = end1 - start1;
    std::cout << "Nested Loop Baseline: " << ms_double1.count() << " ms\n";

    // Optimized? If we use a flat list or API that iterates natively or bulk?
    // KTextEditor has registerCompletionModel but what if we only register on viewCreated globally via Editor?
    // Wait, KTextEditor::Editor does not have a viewCreated signal!
    // KTextEditor::Document has viewCreated signal.
    // The current code is fine, but maybe there is a way to optimize it by moving it to an Editor-wide hook?
    // What if the optimization is that we should NOT iterate over the existing views if we know there is another way?
    return 0;
}
