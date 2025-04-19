# Create a single test executable with all test files
add_executable(PillarTests
    AssertTest.cpp
    LoggerTest.cpp
    PillarDateTest.cpp
    XDGBaseDirectoriesTest.cpp
    CurlTest.cpp
)

find_package(CURL REQUIRED)

# Link against Google Test and the pillar library
target_link_libraries(PillarTests PRIVATE gtest_main pillar::pillar CURL::libcurl_static)

# Add a test that runs all tests
add_test(NAME pillar.all_tests COMMAND PillarTests)

# Add individual test targets that filter specific test suites
add_test(NAME pillar.AssertTest COMMAND PillarTests --gtest_filter=AssertTest.*)
add_test(NAME pillar.LoggerTest COMMAND PillarTests --gtest_filter=LoggerTest.*)
add_test(NAME pillar.PillarDateTest COMMAND PillarTests --gtest_filter=PillarDateTest.*)
add_test(NAME pillar.XDGBaseDirectoriesTest COMMAND PillarTests --gtest_filter=XDGBaseDirectoriesTest.*)
add_test(NAME pillar.CurlTest COMMAND PillarTests --gtest_filter=CurlTest.*)
