cmake_minimum_required(VERSION 3.21)
project(ALife CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GX")

include_directories(.)
include_directories(src)

add_executable(
	ALife
		src/Environment/SimulationController.cpp
		src/Environment/SimulationController.h
		src/Environment/Environment.cpp
		src/Environment/Environment.h
		src/Environment/MapCell.h
		src/Environment/MapCell.cpp
		src/Environment/Food.h
		src/Environment/Food.cpp
    
	src/genome/genome.h
	src/genome/genome.cpp
	src/types.h
	src/util.h
	src/util.cpp
	src/test.cpp
        src/genome/phenome.h
        src/genome/phenome.cpp
	src/genome/gene/reactionGene.h
	src/genome/gene/reactionGene.cpp
        src/genome/body.cpp
        src/genome/body.h
	src/genome/direction.h
	src/genome/direction.cpp
	src/food.h
	src/genome/genomeView.h
	src/genome/genomeView.cpp
	src/genome/gene.h
	src/genome/gene.cpp
	src/genome/gene/bodyGene.h
	src/genome/gene/bodyGene.cpp
	src/genome/gene/foodGene.h
	src/genome/gene/foodGene.cpp
	src/genome/gene/mutationRateGene.h
	src/genome/gene/mutationRateGene.cpp
)
