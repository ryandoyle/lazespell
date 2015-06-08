cmake_minimum_required(VERSION 3.2)

project(lazespell)
include_directories(PROJECT_SOURCE_DIR)

add_executable(lazespell lazespell.c)
add_library(speller speller.c)

target_link_libraries(lazespell speller)
target_link_libraries(speller aspell)

