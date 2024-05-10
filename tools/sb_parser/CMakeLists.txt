cmake_minimum_required(VERSION 3.2)

project(sb_parser)

set(CMAKE_CXX_STANDARD 23)

add_executable(sb_parser main.cpp
                         error.cpp
                         error.h
                         shader.h
                         shaderbinary.h
                         types.cpp
                         types.h
                         validate.cpp
                         validate.h
                         strings.h
)
target_link_libraries(sb_parser PRIVATE shader_recompiler getopt)
create_target_directory_groups(sb_parser)
