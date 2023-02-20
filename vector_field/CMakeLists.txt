cmake_minimum_required(VERSION 3.10)

project(raylib_ai_projects)

add_executable(vector_field main.cpp game_state.cpp particle_system.cpp)

set_property(TARGET vector_field PROPERTY CXX_STANDARD 11)

target_link_libraries(vector_field raylib_static)

# add_custom_command(TARGET vector_field POST_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E copy_directory
#                        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:vector_field>)

# add_custom_command(TARGET vector_field POST_BUILD
#                        COMMAND ${CMAKE_COMMAND} -E copy_directory
#                            ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:vector_field>/assets)

add_custom_command(TARGET vector_field POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:vector_field>/assets)