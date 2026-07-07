#include <iostream>
int main() {
    std::cout << "The duplicated code connects textDoc to viewCreated, then registers for existing views. Wait. It connects viewCreated FOR EACH textDocument. And it registers for all views of that textDocument." << std::endl;
    std::cout << "But we loop over openDocuments on startup. Then we connect textDocumentCreated from core()->documentController(). This is correct." << std::endl;
    std::cout << "Is there a way to connect viewCreated globally instead of per-document?" << std::endl;
}
