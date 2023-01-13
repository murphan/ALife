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
	src/phenome/phenome.h
	src/phenome/phenome.cpp
	src/genome/gene/reactionGene.h
	src/genome/gene/reactionGene.cpp
	src/phenome/body.cpp
	src/phenome/body.h
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
