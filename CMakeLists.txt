
# When setting up locally, create a new file UserBuild.txt in the base directory
# and put this line in, filling in the correct url:
# make sure you use forward slashes /
#
# SET(JSONHPP_DIR "C:/Users/{{path to folder containing json.hpp}}")

include(UserBuild.txt)

cmake_minimum_required(VERSION 3.21)
project(ALife CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GX")

# lets our files include from the absolute base of the src directory,
# instead of just relative
include_directories(src "${JSONHPP_DIR}")

file(GLOB_RECURSE alife_src_files CONFIGURE_DEPENDS "src" "src/*.h" "src/*.cpp" "src/**/*.h" "src/**/*.cpp")

add_executable(ALife ${alife_src_files})
