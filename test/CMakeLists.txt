cmake_minimum_required(VERSION 3.19)
project(Shared_Test)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
        "gtest_main.cpp"
        "Container/StaticQueue_Tests.cpp"
        "CRC/CRC_Tests.cpp"
        "Enum/EnumAdvanced_Tests.cpp"
        "Enum/EnumBasic_Tests.cpp"
        "Math/Calculus_Tests.cpp"
        "Math/Statistics_Tests.cpp"
        "Serialize/BinaryWriter_Tests.cpp"
        "Serialize/FixedPoint_Tests.cpp"
        "Serialize/SerializeDeserialize_Tests.cpp"
        )

find_package(GTest CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Shared_Lib PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
