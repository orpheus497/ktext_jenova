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

class Editor {
public:
    static Editor* instance() { static Editor e; return &e; }
    Application* application() { return new Application(); }
};

int main() {
    Editor* editor = Editor::instance();
    auto app = editor->application();
    for(int i = 0; i < 5; ++i) {
        auto win = new MainWindow();
        for(int j=0; j< 1000; ++j) {
            win->views_list.push_back(new View());
        }
        app->mainWindows_list.push_back(win);
    }

    CodeCompletionModel* m_completionModel = new CodeCompletionModel();

    auto start1 = std::chrono::high_resolution_clock::now();
    for (auto* win : app->mainWindows()) {
        for (auto* view : win->views()) {
            view->registerCompletionModel(m_completionModel);
        }
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double1 = end1 - start1;
    std::cout << "MainWindow Loop Baseline: " << ms_double1.count() << " ms\n";
    return 0;
}
