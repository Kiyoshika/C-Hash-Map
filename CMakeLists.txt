project(chashmap)
cmake_minimum_required(VERSION 3.11.0)

# Add my C-Linked-List library
include(FetchContent)
FetchContent_Declare(linkedlist
        GIT_REPOSITORY https://github.com/Kiyoshika/C-Linked-List.git
        )
FetchContent_MakeAvailable(linkedlist)
FetchContent_GetProperties(linkedlist)
if (NOT linkedlist_POPULATED)
    FetchContent_Populate(linkedlist)
    add_subdirectory(${linkedlist_SOURCE_DIR})
endif()
link_directories(${linkedlist_BINARY_DIR})

# link my linked list library to hashmap library
add_library(hashmap src/hashmap.c)
target_include_directories(hashmap PUBLIC ${linkedlist_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(hashmap linkedlist)

add_executable(hashmap_testing hashmap_testing.c)
target_link_libraries(hashmap_testing hashmap)
target_include_directories(hashmap_testing PUBLIC ${CMAKE_SOURCE_DIR}/include)

