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
FOREACH(test ${xLinqTestsSrcs})
	get_filename_component(TestName ${test} NAME_WE)
	message("Found test " ${TestName})
	add_executable(${TestName} ${TestName}.cpp)
	target_link_libraries(${TestName} ${GTEST_BOTH_LIBRARIES})
	add_test(${TestName} ${TestName})
ENDFOREACH(test)
message("Detecting tests finished")