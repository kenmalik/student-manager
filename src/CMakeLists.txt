cmake_minimum_required(VERSION 3.10)
project(CS003A_Project1 VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(CREATE_LINK
  "${CMAKE_BINARY_DIR}/compile_commands.json"
  "${CMAKE_SOURCE_DIR}/compile_commands.json"
  SYMBOLIC
)

add_executable(Main main.cpp)
target_include_directories(Main PUBLIC 
    "${CMAKE_BINARY_DIR}/../include"
    "${CMAKE_BINARY_DIR}/../resources"
)

add_library(Student student.cpp)
target_include_directories(Student PUBLIC "${CMAKE_BINARY_DIR}/../include")

target_link_libraries(Main PUBLIC Student)
