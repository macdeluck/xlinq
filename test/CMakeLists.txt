cmake_minimum_required (VERSION 2.8)
project (xLinqTests)

include_directories("../include")

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11)
endif()

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

message("Detecting tests...")
file(GLOB xLinqTestsSrcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*test.cpp" )
add_executable(xlinq_test_runner "xlinq_test_runner.cpp" ${xLinqTestsSrcs})
target_link_libraries(xlinq_test_runner ${GTEST_BOTH_LIBRARIES})
add_test(xlinq_test_runner xlinq_test_runner)