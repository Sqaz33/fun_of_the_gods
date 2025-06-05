cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(Fun_Of_The_Gods)

if (UNIX) 
    set(SFML_LIB "/mnt/d/sfml3ub")
else()
    set(SFML_LIB "C:/SFML-3.0.0")
endif()
list(APPEND CMAKE_PREFIX_PATH ${SFML_LIB})

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window)

add_library(Core INTERFACE)
target_include_directories(Core INTERFACE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(Core INTERFACE SFML::Graphics SFML::Window) 

file(GLOB_RECURSE SRC "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE Core)

set(FONT_FILE 
    "${CMAKE_CURRENT_SOURCE_DIR}/game_data/calibri.ttf")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${FONT_FILE}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_LIB}/bin/sfml-audio-3.dll"
        "${SFML_LIB}/bin/sfml-audio-d-3.dll"
        "${SFML_LIB}/bin/sfml-graphics-d-3.dll"
        "${SFML_LIB}/bin/sfml-graphics-3.dll"
        "${SFML_LIB}/bin/sfml-network-3.dll"
        "${SFML_LIB}/bin/sfml-network-d-3.dll"
        "${SFML_LIB}/bin/sfml-system-3.dll"
        "${SFML_LIB}/bin/sfml-system-d-3.dll"
        "${SFML_LIB}/bin/sfml-window-d-3.dll"
        "${SFML_LIB}/bin/sfml-window-3.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

enable_testing()
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
