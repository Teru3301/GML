cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


project(GML)


include_directories(
    include
    lib
)

add_executable(GML
    ./src/main.cpp
    ./src/GML.cpp 
)



