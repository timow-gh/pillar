find_program(GIT_PROGRAM NAMES git)
if (NOT GIT_PROGRAM)
    message(FATAL_ERROR "git not found!")
endif ()
message(STATUS "Found git: ${GIT_PROGRAM}")

set(CURL_SOURCE_DIR "${CMAKE_BINARY_DIR}/custom_downloads/curl/curl")
set(CURL_BUILD_DIR "${CMAKE_BINARY_DIR}/custom_downloads/curl/curl_build")
set(CURL_INSTALL_DIR "${CMAKE_BINARY_DIR}/custom_downloads/curl/curl_install")

file(MAKE_DIRECTORY ${CURL_SOURCE_DIR})
file(MAKE_DIRECTORY ${CURL_BUILD_DIR})
file(MAKE_DIRECTORY ${CURL_INSTALL_DIR})

set(CURL_REPO_URL "https://github.com/curl/curl.git")
set(CURL_TAG "curl-8_10_1")

function(print_result_vars name)
    message("${name}_RESULT result: ${${name}_RESULT}")
    message("${name}_OUTPUT_RESULT output: ${${name}_OUTPUT_RESULT}")
    message("${name}_ERROR_RESULT error: ${${name}_ERROR_RESULT}")
endfunction()


execute_process(
        COMMAND git clone --depth 1 --branch ${CURL_TAG} ${CURL_REPO_URL} ${CURL_SOURCE_DIR}
        RESULT_VARIABLE CURL_CLONE_RESULT
#        OUTPUT_VARIABLE CURL_CLONE_OUTPUT_RESULT
        ERROR_VARIABLE CURL_CLONE_ERROR_RESULT
)
print_result_vars(CURL_CLONE)

find_program(CMAKE_PROGRAM NAMES cmake)
if (NOT CMAKE_PROGRAM)
    message(FATAL_ERROR "cmake not found!")
endif ()
message(STATUS "Found cmake: ${CMAKE_PROGRAM}")

# Configure the curl project
execute_process(
        COMMAND ${CMAKE_PROGRAM} -S ${CURL_SOURCE_DIR} -B ${CURL_BUILD_DIR} -G ${CMAKE_GENERATOR}
        -DBUILD_SHARED_LIBS=OFF
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_INSTALL_PREFIX=${CURL_INSTALL_DIR}
        -DCURL_WERROR=OFF
        -DPICKY_COMPILER=ON
        -DBUILD_CURL_EXE=ON
        -DBUILD_SHARED_LIBS=OFF
        -DBUILD_STATIC_LIBS=ON
        -DBUILD_STATIC_CURL=ON
        -DENABLE_ARES=OFF
        -DCURL_DISABLE_INSTALL=OFF
        RESULT_VARIABLE CURL_CONFIGURE_RESULT
        ERROR_VARIABLE CURL_CONFIGURE_ERROR_RESULT
)
print_result_vars(CURL_CONFIGURE)

# Build curl project
execute_process(
        COMMAND ${CMAKE_PROGRAM} --build ${CURL_BUILD_DIR} --config "Release" --parallel
        RESULT_VARIABLE CURL_BUILD_RESULT
        ERROR_VARIABLE CURL_BUILD_ERROR_RESULT
)
print_result_vars(CURL_BUILD)

# Install curl project
execute_process(
        COMMAND ${CMAKE_PROGRAM} --install ${CURL_BUILD_DIR} --config "Release"
        RESULT_VARIABLE CURL_INSTALL_RESULT
        OUTPUT_VARIABLE CURL_INSTALL_OUTPUT_RESULT
        ERROR_VARIABLE CURL_INSTALL_ERROR_RESULT
)
print_result_vars(CURL_INSTALL)

# Append the CURL_INSTALL dir to the CMAKE_PREFIX_PATH so that find_package can find it
list(APPEND CMAKE_PREFIX_PATH ${CURL_INSTALL_DIR})

# Download and setup date which will use the installed curl lib.
FetchContent_Declare(
        date
        GIT_REPOSITORY https://github.com/HowardHinnant/date.git
        GIT_TAG v3.0.2)

set(USE_SYSTEM_TZ_DB OFF CACHE BOOL "Use the operating system's timezone database" FORCE)
set(MANUAL_TZ_DB OFF CACHE BOOL "User will set TZ DB manually by invoking set_install in their code" FORCE)
set(USE_TZ_DB_IN_DOT OFF CACHE BOOL "Save the timezone database in the current folder" FORCE)
set(ENABLE_DATE_TESTING OFF CACHE BOOL "Enable unit tests" FORCE)
set(DISABLE_STRING_VIEW OFF CACHE BOOL "Disable string view" FORCE)
set(COMPILE_WITH_C_LOCALE OFF CACHE BOOL "define ONLY_C_LOCALE=1" FORCE)
set(BUILD_TZ_LIB ON CACHE BOOL "build/install of TZ library" FORCE)
set(ENABLE_DATE_INSTALL ON CACHE BOOL "Enable install" FORCE)
set(CURL_USE_PKGCONFIG OFF CACHE BOOL "Enable install" FORCE)

FetchContent_MakeAvailable(date)
