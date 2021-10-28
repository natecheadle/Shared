set(GCC_VERSION "9" CACHE STRING "Version Of Clang")

Message(STATUS "Using GCC Version - ${CLANG_VERSION}.")

if(GCC_VERSION STREQUAL "9")
  set(CMAKE_C_COMPILER "gcc-9")
  set(CMAKE_CXX_COMPILER "g++-9")
elseif(GCC_VERSION STREQUAL "8")
  set(CMAKE_C_COMPILER "gcc-8")
  set(CMAKE_CXX_COMPILER "g++-8")
else()
  MESSAGE(FATAL_ERROR "Minimum GCC Version is GCC-8")
endif()


if (BUILD_COVERAGE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif ()
