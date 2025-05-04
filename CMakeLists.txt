cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(Fun_Of_The_Gods)

if (UNIX) 
    set(CMAKE_PREFIX_PATH "/mnt/d/sfml3ub")
else()
    set(CMAKE_PREFIX_PATH "C:/SFML-3.0.0")
endif()

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window)

file(GLOB_RECURSE SRC "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window) 

set(FONT_FILE "${CMAKE_SOURCE_DIR}/calibri.ttf")
add_custom_command(TARGET tst POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${FONT_FILE}" "$<TARGET_FILE_DIR:tst>"
)
