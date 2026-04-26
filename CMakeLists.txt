cmake_minimum_required(VERSION 4.1)
project(
    cstdlib
    VERSION 0.1.0
    DESCRIPTION "A modern replacement for the C standard library"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 23)

include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(CTest)

option(BUILD_SHARED_LIBS "Build the shared cstdlib library" ON)
option(BUILD_STATIC_LIBS "Build the static cstdlib library" ON)

if (NOT BUILD_SHARED_LIBS AND NOT BUILD_STATIC_LIBS)
    message(FATAL_ERROR "At least one of BUILD_SHARED_LIBS or BUILD_STATIC_LIBS must be ON")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(GENERATED_INCLUDE_ROOT "${CMAKE_CURRENT_BINARY_DIR}/generated/include")
set(GENERATED_INCLUDE_DIR "${GENERATED_INCLUDE_ROOT}/cstdlib")
file(MAKE_DIRECTORY "${GENERATED_INCLUDE_DIR}")

add_library(cstdlib_obj OBJECT
    src/cstdlib.c
    src/mem.c
    src/alloc.c
    src/string.c
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(cstdlib_obj PRIVATE
        src/mem_win32.c
        src/console_win32.c
    )
endif ()

target_include_directories(cstdlib_obj PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${GENERATED_INCLUDE_ROOT}"
)

target_compile_definitions(cstdlib_obj PRIVATE cstdlib_obj_EXPORTS)

generate_export_header(cstdlib_obj
    EXPORT_FILE_NAME "${GENERATED_INCLUDE_DIR}/api.h"
    BASE_NAME cstdlib
)

function(setup_cstdlib_target target is_static)
    target_include_directories(${target} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${GENERATED_INCLUDE_ROOT}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )

    if (is_static)
        target_compile_definitions(${target} PUBLIC CSTDLIB_STATIC_DEFINE)
    endif ()

    set_target_properties(${target} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
    )
endfunction()

if (BUILD_SHARED_LIBS)
    add_library(cstdlib_shared SHARED $<TARGET_OBJECTS:cstdlib_obj>)
    add_library(cstdlib::shared ALIAS cstdlib_shared)
    setup_cstdlib_target(cstdlib_shared FALSE)
endif ()

if (BUILD_STATIC_LIBS)
    add_library(cstdlib_static STATIC $<TARGET_OBJECTS:cstdlib_obj>)
    add_library(cstdlib::static ALIAS cstdlib_static)
    setup_cstdlib_target(cstdlib_static TRUE)
endif ()

if (BUILD_SHARED_LIBS)
    add_library(cstdlib::cstdlib ALIAS cstdlib_shared)
else ()
    add_library(cstdlib::cstdlib ALIAS cstdlib_static)
endif ()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif ()

if (BUILD_SHARED_LIBS)
    install(TARGETS cstdlib_shared
        EXPORT cstdlibTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif ()

if (BUILD_STATIC_LIBS)
    install(TARGETS cstdlib_static
        EXPORT cstdlibTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif ()

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY "${GENERATED_INCLUDE_DIR}/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cstdlib
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT cstdlibTargets
    FILE cstdlibTargets.cmake
    NAMESPACE cstdlib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cstdlib
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cstdlibConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cstdlibConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cstdlib
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/cstdlibConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cstdlibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cstdlibConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cstdlib
)
