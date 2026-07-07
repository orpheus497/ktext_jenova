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

class MainWindow {
public:
    std::vector<View*> views_list;
    const std::vector<View*>& views() const { return views_list; }
};

class Application {
public:
    std::vector<MainWindow*> mainWindows_list;
    const std::vector<MainWindow*>& mainWindows() const { return mainWindows_list; }
};

int main() {
    // Is there a better way to register for all open views?
    // According to KTextEditor API docs, KTextEditor::Editor has documents().
    // KTextEditor::Document has views().
    // So to register for all views, we just iterate over all documents, then all views.
    return 0;
}
