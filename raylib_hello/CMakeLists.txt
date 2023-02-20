cmake_minimum_required(VERSION 3.10)

project(raylib_ai_projects)

add_executable(raylib_hello main.cpp)

set_property(TARGET raylib_hello PROPERTY CXX_STANDARD 11)

target_link_libraries(raylib_hello raylib_static)

