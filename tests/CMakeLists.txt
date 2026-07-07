set(CMAKE_AUTOMOC ON)

add_executable(TestLlamaClient
    TestLlamaClient.cpp
    ../src/network/LlamaClient.cpp
)

target_include_directories(TestLlamaClient PRIVATE ../src/network ${CMAKE_CURRENT_SOURCE_DIR}/mocks)

target_link_libraries(TestLlamaClient
    Qt6::Core
    Qt6::Network
    Qt6::Test
)

add_test(NAME TestLlamaClient COMMAND TestLlamaClient)
