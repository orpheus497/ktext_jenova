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
    void registerCompletionModel(CodeCompletionModel* model) {}
};

class Editor {
public:
    static Editor* instance() { static Editor e; return &e; }
    std::vector<Document*> docs_list;
    const std::vector<Document*>& documents() const { return docs_list; }
};

int main() {
    auto editor = Editor::instance();
    for (int i = 0; i < 1000; ++i) {
        auto doc = new Document();
        for (int j = 0; j < 5; ++j) {
            doc->views_list.push_back(new View());
        }
        editor->docs_list.push_back(doc);
    }

    CodeCompletionModel* m_completionModel = new CodeCompletionModel();

    // Original nested loops
    auto start1 = std::chrono::high_resolution_clock::now();
    for (auto* doc : editor->documents()) {
        for (auto* view : doc->views()) {
            view->registerCompletionModel(m_completionModel);
        }
    }
    auto end1 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms_double1 = end1 - start1;
    std::cout << "Original loop time: " << ms_double1.count() << " ms\n";

    // How to optimize this?
    // Maybe we just shouldn't do this in the constructor, but do it globally if possible?
    // Wait, the KTextEditor::Editor doesn't have a way to register for all views... Wait, does it?
    // No, we already checked.

    // BUT there is a duplication in the code!
    return 0;
}
