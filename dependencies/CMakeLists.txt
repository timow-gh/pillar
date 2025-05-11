include(FetchContent)

add_subdirectory(date)

if(${PROJECT_NAME}_TESTS)
    add_subdirectory(googletest)
endif()

