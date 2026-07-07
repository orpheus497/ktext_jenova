#include <iostream>
int main() {
    // The issue says:
    // "Nested loops for iterating over open documents and views to register completion models is inefficient if many views are opened. We could maybe do it in bulk."
    //
    // And "KDevLLMPlugin::~KDevLLMPlugin() unregisters by doing:"
    // for (auto* doc : KTextEditor::Editor::instance()->documents()) {
    //     for (auto* view : doc->views()) {
    //         view->unregisterCompletionModel(m_completionModel);
    //     }
    // }
    return 0;
}
