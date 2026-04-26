add_library(ctest STATIC
    ctest/ctest.c
)
target_include_directories(ctest PUBLIC ctest)

target_link_libraries(ctest PRIVATE
    cstdlib::shared
)

function(add_cstdlib_test test_name test_source)
    add_executable(${test_name} ${test_source})
    target_link_libraries(${test_name} PRIVATE
        ctest
        cstdlib::shared
    )
    set_target_properties(${test_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_cstdlib_test(test_mem_utils src/test_mem_utils.c)
add_cstdlib_test(test_string src/test_string.c)
