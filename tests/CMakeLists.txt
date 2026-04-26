add_library(ctest STATIC
    ctest/ctest.c
)
target_include_directories(ctest PUBLIC ctest)

target_link_libraries(ctest PRIVATE
    cstdlib::shared
)

add_executable(test_cstdlib src/test_cstdlib.c)

target_link_libraries(test_cstdlib PRIVATE
    ctest
    cstdlib::shared
)

add_test(NAME cstdlib COMMAND test_cstdlib)

set_target_properties(test_cstdlib PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
