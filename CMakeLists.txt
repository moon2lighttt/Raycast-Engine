cmake_minimum_required(VERSION 3.10.0)
project(raycast-engine VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE 
        src/main.c
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
