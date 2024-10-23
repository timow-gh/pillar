cmake_minimum_required(VERSION 3.23)

project("pillar"
        VERSION 1.0.0
        LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(warnings)
include(static_analysis)

include(cmake/CPM.cmake)
if (NOT CPM_SOURCE_CACHE AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CPMSourceVariable.cmake")
    include(CPMSourceVariable.cmake)
endif ()

if (NOT CPM_SOURCE_CACHE)
    message(AUTHOR_WARNING "CPM_SOURCE_CACHE not set. The source files are downloaded for every build. This is not recommended.
        Set the CPM_SOURCE_CACHE variable to a directory of your choice to cache the cloned git repositories.
        Set the CPM_SOURCE_CACHE variable in CPMSourceVariable.cmake to keep your paths private.")
endif ()

# Set common properties for cross platform builds
include(project_preamble)
project_preamble()

if ((CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR))
    set(IS_TOP_LEVEL_PROJECT TRUE)
else ()
    set(IS_TOP_LEVEL_PROJECT FALSE)
endif ()

if (IS_TOP_LEVEL_PROJECT)
    option(${PROJECT_NAME}_USE_SCCACHE "Use sccache" ON)
    option(${PROJECT_NAME}_USE_WARNINGS "Compile using warnings." ON)
    option(${PROJECT_NAME}_WARNINGS_AS_ERRORS "Treat compiler warnings as errors " OFF)
    option(${PROJECT_NAME}_STATIC_ANALYSIS "" ON)
    option(${PROJECT_NAME}_USE_CPPCHECK "" ON)
    option(${PROJECT_NAME}_TESTS "" ON)
else ()
    option(${PROJECT_NAME}_USE_SCCACHE "Use sccache" OFF)
    option(${PROJECT_NAME}_USE_WARNINGS "Compile using warnings." OFF)
    option(${PROJECT_NAME}_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
    option(${PROJECT_NAME}_STATIC_ANALYSIS "" OFF)
    option(${PROJECT_NAME}_USE_CPPCHECK "" OFF)
    option(${PROJECT_NAME}_TESTS "" OFF)
endif ()
option(${PROJECT_NAME}_INSTALL "Install project targets" ON)
option(${PROJECT_NAME}_BUILD_DOCUMENTATION "Build documentation" OFF)

if (${PROJECT_NAME}_USE_SCCACHE)
    include(setup_sccache)
    enable_sccache()
endif ()

if (${PROJECT_NAME}_STATIC_ANALYSIS)
    include(cppcheck)
    create_cppcheck_target(${TARGET_NAME})
endif ()

find_package(spdlog REQUIRED CONFIG)

add_subdirectory(dependencies)

if (${PROJECT_NAME}_INSTALL)
    set(project_export_set_name "${PROJECT_NAME}TargetsExportSet")
endif ()

add_subdirectory(pillar_lib)

if (${PROJECT_NAME}_TESTS)
    include(CTest)
    # "This command should be in the source directory root because ctest expects to find a test
    # file in the build directory root.", CMake docs
    enable_testing()
    add_subdirectory(test)
endif ()

if (${PROJECT_NAME}_INSTALL)
    include(setup_export_set_installation)
    # Install the export set, using the helper function from cmake/setupExportSetInstall.cmake and the file Config.cmake.in
    # In the Config.cmake.in all dependencies of the project must be listed.
    message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
    message(STATUS "project_export_set_name: ${project_export_set_name}")
    message(STATUS "PROJECT_VERSION_MAJOR: ${PROJECT_VERSION_MAJOR}")
    message(STATUS "PROJECT_VERSION_MINOR: ${PROJECT_VERSION_MINOR}")
    message(STATUS "PROJECT_VERSION_PATCH: ${PROJECT_VERSION_PATCH}")

    setup_export_set_installation(
            ${PROJECT_NAME}
            ${project_export_set_name}
            Config.cmake.in
            ${PROJECT_VERSION_MAJOR}
            ${PROJECT_VERSION_MINOR}
            ${PROJECT_VERSION_PATCH}
    )
endif ()
