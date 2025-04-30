cmake_minimum_required(VERSION 3.14)
project(MyProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Source files
file(GLOB USER_FILES "code/*.h" "code/*.cpp")
file(GLOB TEST_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "tests/test_*.cpp")

# Try to find system-installed GTest
find_package(GTest QUIET)

if(GTEST_FOUND)
    message(">> GTest was found in the system.")
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(run_tests ${TEST_FILES} ${USER_FILES})
    target_link_libraries(run_tests gtest_main ${GTEST_LIBRARIES} pthread)
else()
    message(">> GTest not found, downloading via ExternalProject...")

    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)

    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)

    if(result)
      message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} --build .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)

    if(result)
      message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                     ${CMAKE_BINARY_DIR}/googletest-build
                     EXCLUDE_FROM_ALL)

    include_directories(${CMAKE_BINARY_DIR}/googletest-src/googletest/include)

    add_executable(run_tests ${TEST_FILES} ${USER_FILES})
    target_link_libraries(run_tests gtest_main pthread)
endif()

enable_testing()

add_executable(run_app app/main.cpp ${USER_FILES})