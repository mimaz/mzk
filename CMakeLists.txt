cmake_minimum_required(VERSION 3.0)

set(lib_sources 
	src/timer.cc 
	src/signal.cc 
	src/shared.cc 
	src/property.cc
	src/logger.cc
	src/animator.cc
)

include_directories(include/)

set(CMAKE_CXX_FLAGS "-g -Og -Wall -std=c++14")

add_executable(mzk-test main.cc)

target_link_libraries(mzk-test mzk)

add_library(mzk STATIC ${lib_sources})
