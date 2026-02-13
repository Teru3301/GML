cmake_minimum_required(VERSION 4.1.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


project(GML)


include_directories(
    include
    lib
)

file(GLOB_RECURSE SRC "src/*.cpp")
add_executable(GML ${SRC})

