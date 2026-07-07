#include <iostream>
#include <vector>
#include <chrono>

struct CodeCompletionModel {};

class View {
public:
    void registerCompletionModel(CodeCompletionModel* model) {}
};

class Document {
public:
    std::vector<View*> views_list;
    const std::vector<View*>& views() const { return views_list; }
    void viewCreated(Document*, View*) {}
};

class DocumentController {
public:
    std::vector<Document*> docs_list;
    const std::vector<Document*>& openDocuments() const { return docs_list; }
    void textDocumentCreated(Document*) {}
};

class Editor {
public:
    static Editor* instance() { static Editor e; return &e; }
    std::vector<Document*> docs_list;
    const std::vector<Document*>& documents() const { return docs_list; }
};

int main() {
    return 0;
}
