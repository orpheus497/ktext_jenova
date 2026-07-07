# ##Script function and purpose: CMake setup for tests.
add_executable(TestAiChatWidget
    TestAiChatWidget.cpp
    ../ui/AiChatWidget.cpp
    ../ui/AiChatInputWidget.cpp
    ../network/LlamaClient.cpp
    ../context/ContextManager.cpp
)
target_link_libraries(TestAiChatWidget
    KDev::Interfaces
    KDev::Util
    KDev::Project
    KDev::Shell
    KDev::Language
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
)
add_test(NAME TestAiChatWidget COMMAND TestAiChatWidget)
set_tests_properties(TestAiChatWidget PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
