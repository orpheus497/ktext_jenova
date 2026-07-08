# CMakeLists.txt for testing

find_package(Qt6 REQUIRED COMPONENTS Test Core Network Widgets)

set(TEST_SRCS
    TestAiCompletionModel.cpp
)

add_executable(TestAiCompletionModel ${TEST_SRCS})
set_target_properties(TestAiCompletionModel PROPERTIES AUTOMOC ON)

target_link_libraries(TestAiCompletionModel PRIVATE
    kdevllm
    Qt6::Test
    Qt6::Core
    Qt6::Network
    Qt6::Widgets
)

find_package(KF6 REQUIRED COMPONENTS TextEditor CoreAddons)
target_link_libraries(TestAiCompletionModel PRIVATE KF6::TextEditor KF6::CoreAddons)

add_test(NAME TestAiCompletionModel COMMAND TestAiCompletionModel)

set(TEST_CMD_SRCS
    TestCommandTextEdit.cpp
)
add_executable(TestCommandTextEdit ${TEST_CMD_SRCS})
set_target_properties(TestCommandTextEdit PROPERTIES AUTOMOC ON)
target_link_libraries(TestCommandTextEdit PRIVATE kdevllm Qt6::Test Qt6::Core Qt6::Widgets)
add_test(NAME TestCommandTextEdit COMMAND TestCommandTextEdit)
