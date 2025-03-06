cmake_minimum_required(VERSION 3.7)

project(sorting-utility LANGUAGES C)

set(CMAKE_C_STANDARD 99)

option(SWITCH_TO_MMAP OFF)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable(sort_strings ${SOURCES})

if(SWITCH_TO_MMAP)
    target_compile_definitions(sort_strings PRIVATE SWITCH_TO_MMAP)
endif()

if(MSVC)
    target_compile_options(sort_strings PRIVATE /W4 /WX /fsanitize=address)
else()
    target_compile_options(sort_strings PRIVATE -Wall -Wextra -pedantic -Werror -fsanitize=address)
    target_link_options(sort_strings PRIVATE -fsanitize=address)
endif()

