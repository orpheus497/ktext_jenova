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
    void registerCompletionModel(CodeCompletionModel* model) {
        // Can a document register models for all its views? No, KTextEditor::Document does not have registerCompletionModel.
        // What about KTextEditor::Editor? Does it have registerCompletionModel?
    }
};
