#include <iostream>
#include <vector>
#include <chrono>

struct CodeCompletionModel {};
struct View {
    void registerCompletionModel(CodeCompletionModel* m) {}
};
struct Document {
    std::vector<View*> views_list;
    const std::vector<View*>& views() const { return views_list; }
    void viewCreated(Document*, View*) {}
};
struct Editor {
    std::vector<Document*> docs_list;
    const std::vector<Document*>& documents() const { return docs_list; }
    void documentCreated(Editor*, Document*) {}
    void viewCreated(Editor*, View*) {} // Does this exist? No.
};

int main() {
    return 0;
}
