cmake_minimum_required(VERSION 3.10)
project(CPU_USAGE_TRACKER LANGUAGES C)

set(CMAKE_C_STANDARD 99)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	add_compile_options("-Weverything")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  	add_compile_options("-Wall -Wextra")
endif()

add_executable(${PROJECT_NAME} Main.c)

target_link_libraries(${PROJECT_NAME} pthread)