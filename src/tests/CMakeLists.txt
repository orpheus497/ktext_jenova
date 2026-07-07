// ##Script function and purpose: CMake setup for tests.
add_executable(TestAiChatWidget TestAiChatWidget.cpp)
target_link_libraries(TestAiChatWidget Qt6::Widgets kdevllm)
add_test(NAME TestAiChatWidget COMMAND TestAiChatWidget)
