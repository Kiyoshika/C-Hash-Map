project(chashmap)
cmake_minimum_required(VERSION 3.11.0)

add_library(hashmap src/hashmap.c)

add_executable(hashmap_testing hashmap_testing.c)
target_link_libraries(hashmap_testing hashmap ${CMAKE_SOURCE_DIR}/ext/liblinkedlist.a)