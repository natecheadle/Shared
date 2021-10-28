set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

if (BUILD_COVERAGE)
    SET(CLANG_COVERAGE_COMPILE_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_COVERAGE_COMPILE_FLAGS}")
endif ()
