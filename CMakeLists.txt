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
		src/genome/phenome.h src/genome/phenome.cpp src/genome/action.h src/genome/action.cpp src/genome/body.cpp src/genome/body.h src/genome/direction.h src/genome/direction.cpp src/genome/genomeCreator.h src/genome/genomeCreator.cpp)
