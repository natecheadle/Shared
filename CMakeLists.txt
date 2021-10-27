cmake_minimum_required(VERSION 3.19)

project(Shared)

add_subdirectory(test)
add_subdirectory(lib)

install(
    TARGETS Shared_Test 
    RUNTIME DESTINATION bin 
    LIBRARY DESTINATION lib)