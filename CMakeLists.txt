cmake_minimum_required(VERSION 3.10)

project(raylib_ai_projects)

set(RAYLIB_DIR raylib)
include_directories(${RAYLIB_DIR}/src ${RAYLIB_DIR}/src/external)
add_subdirectory(${RAYLIB_DIR})

include_directories(common)

add_subdirectory(raylib_hello)
add_subdirectory(vector_field)
