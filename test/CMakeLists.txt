# Create a single test executable with all test files
add_executable(PillarTests
    test_assert.cpp
    test_logger.cpp
    test_pillardate.cpp
    test_xdgdirectories.cpp
    test_curl.cpp
    test_bytereader.cpp
)

find_package(CURL REQUIRED)

# Link against Google Test and the pillar library
target_link_libraries(PillarTests PRIVATE gtest_main pillar::pillar CURL::libcurl_static)

# Define build directory path for tests to use for temporary files
target_compile_definitions(PillarTests PRIVATE 
    CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}
)

# Add a test that runs all tests
add_test(NAME pillar.all_tests COMMAND PillarTests)

add_test(NAME pillar.test_assert COMMAND PillarTests --gtest_filter=test_assert.*)
add_test(NAME pillar.test_bytereader COMMAND PillarTests --gtest_filter=test_bytereader.*)
add_test(NAME pillar.test_curl COMMAND PillarTests --gtest_filter=test_curl.*)
add_test(NAME pillar.test_logger COMMAND PillarTests --gtest_filter=test_logger.*)
add_test(NAME pillar.test_pillardate COMMAND PillarTests --gtest_filter=test_pillardate.*)
add_test(NAME pillar.test_xdgdirectories COMMAND PillarTests --gtest_filter=test_xdgdirectories.*)
