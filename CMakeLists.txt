cmake_minimum_required(VERSION 3.21)
project(ALife CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GX")

include_directories(.)
include_directories(src)

add_executable(
	ALife
		src/genome/genome.h
		src/genome/genome.cpp
	src/types.h
	src/util.h
	src/util.cpp
	src/test.cpp
)
