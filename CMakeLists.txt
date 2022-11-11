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
		src/Environment/SimulationController.cpp
		src/Environment/SimulationController.h
		src/Environment/Environment.cpp
		src/Environment/Environment.h
		src/Environment/MapCell.h
		src/Environment/MapCell.cpp
		src/Environment/Food.h
		src/Environment/Food.cpp
	src/types.h
	src/util.h
	src/util.cpp
	src/test.cpp
)
