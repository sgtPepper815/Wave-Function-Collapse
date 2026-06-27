cmake_minimum_required(VERSION 3.16)

project(WaveFunctionCollapse LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML3/lib/cmake/SFML")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(WaveFunctionCollapse
    Main.cpp
    Grid.cpp
    Cell.cpp
)

target_include_directories(WaveFunctionCollapse PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(WaveFunctionCollapse PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

add_custom_command(TARGET WaveFunctionCollapse POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML3/bin"
        "$<TARGET_FILE_DIR:WaveFunctionCollapse>"
)