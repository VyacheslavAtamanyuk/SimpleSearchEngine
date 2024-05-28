include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(browser_tests browser_tests.cpp)

target_include_directories(browser_tests PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(browser_tests Browser GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(browser_tests)