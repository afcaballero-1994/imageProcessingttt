cmake_minimum_required(VERSION 3.28)
project(imageprocessingtest C)

set(CMAKE_C_STANDARD 23)

add_executable(imageprocessingtest src/main.c
        src/filters.c
)

target_link_libraries(imageprocessingtest PRIVATE m)
