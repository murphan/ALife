
# When setting up locally, create a new file UserBuild.txt in the base directory
# and put this line in, filling in the correct url:
# make sure you use forward slashes /
#
# SET(WEBSOCKETPP_DIR "C:/{{PATH TO WEBSOCKET LIBRARY}}/websocketpp-0.8.2/websocketpp")

include(UserBuild.txt)

cmake_minimum_required(VERSION 3.21)
project(ALife CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GX")

# lets our files include from the absolute base of the src directory,
# instead of just relative
include_directories(src)

# definitions for the python interface library
include_directories(${WEBSOCKETPP_DIR})

file(GLOB_RECURSE alife_src_files CONFIGURE_DEPENDS "src/*.h" "src/*.cpp" "src/**/*.h" "src/**/*.cpp")

add_executable(ALife ${alife_src_files})
