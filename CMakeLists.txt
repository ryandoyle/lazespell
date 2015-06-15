cmake_minimum_required(VERSION 2.6)

project(lazespell)
include_directories(PROJECT_SOURCE_DIR)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(lazespell lazespell.c)
add_library(speller speller.c)

target_link_libraries(lazespell speller ${GTK3_LIBRARIES})
target_link_libraries(speller aspell)

install(TARGETS lazespell DESTINATION bin)
