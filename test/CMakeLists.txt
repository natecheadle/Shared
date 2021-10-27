cmake_minimum_required(VERSION 3.19)
project(Shared_Test)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
        "gtest_main.cpp"
        )

find_package(GTest CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)