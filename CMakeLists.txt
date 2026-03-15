cmake_minimum_required(VERSION 3.20)
project(mouseclick)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mouseclick
    main.cpp
    memory.cpp
    process.cpp
    mouse.cpp
)

target_link_libraries(mouseclick PRIVATE user32)
