set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

set(VCPKG_CXX_FLAGS_DEBUG "${VCPKG_CXX_FLAGS} -stdlib=libc++")
set(VCPKG_CXX_FLAGS_RELEASE "${VCPKG_CXX_FLAGS} -stdlib=libc++")
set(VCPKG_C_FLAGS_DEBUG "${VCPKG_C_FLAGS} -stdlib=libc++")
set(VCPKG_C_FLAGS_RELEASE "${VCPKG_C_FLAGS} -stdlib=libc++")

# set(VCPKG_CXX_FLAGS_DEBUG "${VCPKG_CXX_FLAGS} -stdlib=libc++ -std=c++23")
# set(VCPKG_CXX_FLAGS_RELEASE "${VCPKG_CXX_FLAGS} -stdlib=libc++ -std=c++23")
# set(VCPKG_C_FLAGS_DEBUG "${VCPKG_C_FLAGS} -stdlib=libc++ -std=c++23")
# set(VCPKG_C_FLAGS_RELEASE "${VCPKG_C_FLAGS} -stdlib=libc++ -std=c++23")

# Specify the C and C++ compilers
# set(VCPKG_C_COMPILER clang)
# set(VCPKG_CXX_COMPILER clang++)

# Optional: Specify the full path to clang and clang++ if not in PATH
# set(VCPKG_C_COMPILER /usr/bin/clang)
# set(VCPKG_CXX_COMPILER /usr/bin/clang++)

# set(CMAKE_C_COMPILER clang)
# set(CMAKE_CXX_COMPILER clang++) 

# set(CMAKE_C_COMPILER "/usr/bin/clang")
# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

set(VCPKG_CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/../../toolchains/linux-clang.cmake")

message(STATUS "Using x64-linux-cpp23 toolchain file VCPKG_CMAKE_TOOLCHAIN_FILE: ${VCPKG_CMAKE_TOOLCHAIN_FILE}")
message(STATUS "Using x64-linux-cpp23 with CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "Using x64-linux-cpp23 with CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")

set(VCPKG_CMAKE_SYSTEM_NAME Linux)

