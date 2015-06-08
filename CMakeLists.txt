cmake_minimum_required(VERSION 3.2)

project(lazespell)

set(SOURCE_FILES
    lazespell.c)


add_executable(lazespell ${SOURCE_FILES})

target_link_libraries(lazespell aspell)

#add_library(aspell aspell.h)
