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

    // We can avoid the nested loops by using application-wide editor views if possible, but that's not available in KTextEditor APIs...
    // Let's check `KTextEditor::Editor::instance()->views()` again? It was returned earlier:
    return 0;
}
