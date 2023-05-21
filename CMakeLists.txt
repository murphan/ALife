
# When setting up locally, create a new file UserBuild.txt in the base directory
# and put this line in, filling in the correct url:
# make sure you use forward slashes /
#
# SET(JSONHPP_DIR "C:/Users/{{path to folder containing json.hpp}}")

include(UserBuild.txt)
include(CMakePrintHelpers)

cmake_minimum_required(VERSION 3.26.4)
project(ALife CXX)

set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

# lets our files include from the absolute base of the src directory,
# instead of just relative
include_directories(src "${JSONHPP_DIR}")

file(GLOB_RECURSE alife_all_src_files RELATIVE "${PROJECT_SOURCE_DIR}" CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp" "src/*.h" "src/**/*.cpp" "src/**/*.h" "src/**/*.hpp")

set(alife_header_src_files "src/environment/noise/OpenSimplex2S.hpp" "src/environment/noise/OpenSimplex2S.cpp")

set(alife_module_src_files "${alife_all_src_files}")
list(REMOVE_ITEM alife_module_src_files ${alife_header_src_files})
list(REMOVE_ITEM alife_module_src_files "src/main.cpp")

cmake_print_variables(alife_all_src_files)
cmake_print_variables(alife_module_src_files)
cmake_print_variables(alife_header_src_files)

add_executable(ALife ${alife_all_src_files})
target_sources(ALife
    PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    ${alife_module_src_files}
)

# add_library(OpenSimplex STATIC ${alife_header_src_files})
# link_libraries(ALife OpenSimplex)
#FILE_SET cxx TYPE HEADERS FILES
#"src/main.cpp"