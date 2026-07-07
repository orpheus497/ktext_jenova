#include <iostream>
#include <chrono>
#include <vector>

// Dummy structures to simulate KTextEditor behavior
namespace KTextEditor {
    class Document;
    class View {
    public:
        void registerCompletionModel(void*) {
            // Simulate some small overhead
            volatile int x = 0;
            for(int i=0; i<100; ++i) x += i;
        }
    };

    class Document {
    public:
        std::vector<View*> views_list;
        std::vector<View*> views() { return views_list; }
    };

    class Editor {
    public:
        static Editor* instance() {
            static Editor ed;
            return &ed;
        }
        std::vector<Document*> docs_list;
        std::vector<Document*> documents() { return docs_list; }
        // Dummy signals/methods to mimic viewCreated if possible, but we don't have Qt
    };
}

int main() {
    auto editor = KTextEditor::Editor::instance();
    for (int i = 0; i < 50; ++i) {
        auto doc = new KTextEditor::Document();
        for (int j = 0; j < 10; ++j) {
            doc->views_list.push_back(new KTextEditor::View());
        }
        editor->docs_list.push_back(doc);
    }

    auto start = std::chrono::high_resolution_clock::now();

    // The N+1 way
    void* m_completionModel = nullptr;
    for (auto* doc : editor->documents()) {
        for (auto* view : doc->views()) {
            view->registerCompletionModel(m_completionModel);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double = end - start;

    std::cout << "Original N+1 Time: " << ms_double.count() << " ms\n";

    // We want to simulate what the optimized way would be, but since the
    // optimization is using `KTextEditor::Editor::instance()->views()` or hooking up to
    // KTextEditor::Editor global signal... wait.
    // Let me check if KTextEditor::Editor has a global viewCreated signal or views() method!
    return 0;
}
