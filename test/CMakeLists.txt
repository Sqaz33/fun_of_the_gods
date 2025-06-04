find_package(GTest CONFIG REQUIRED)
add_executable(tst ${SRC} tests.cpp)
target_link_libraries(tst PRIVATE Core)
target_compile_definitions(tst PRIVATE TEST)
target_link_libraries(tst PRIVATE GTest::gtest)
add_test(NAME tst COMMAND tst)

add_custom_command(TARGET tst POST_BUILD
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
        "$<TARGET_FILE_DIR:tst>"
)