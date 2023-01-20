
# When setting up locally, create a new file UserBuild.txt in the base directory
# and put these two lines in:
#
# SET(PYTHON_HOME "C:/Python311")
# SET(PYTHON_VER "311")
#
# you can change the python version if you need to

include(UserBuild.txt)

cmake_minimum_required(VERSION 3.21)
project(ALife CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GX")

# lets our files include from the absolute base of the src directory,
# instead of just relative
include_directories(src)

# definitions for the python interface library
include_directories(${PYTHON_HOME}/include)

file(GLOB_RECURSE alife_src_files CONFIGURE_DEPENDS "src/*.h" "src/*.cpp" "src/**/*.h" "src/**/*.cpp")

add_executable(ALife ${alife_src_files})

# link the python interface library
# will ultimately use the python-provided dll at runtime
target_link_libraries(ALife debug ${PYTHON_HOME}/libs/python${PYTHON_VER}_d.lib)
target_link_libraries(ALife optimized ${PYTHON_HOME}/libs/python${PYTHON_VER}.lib)

# "builds" the python files by moving them to the build directory
file(GLOB_RECURSE alife_python_files CONFIGURE_DEPENDS "src/python/*.py" "src/python/**/*.py")

add_custom_command(
    TARGET ALife POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${alife_python_files}
    ${CMAKE_CURRENT_BINARY_DIR}
)

#foreach(py_file ${alife_python_files})
#    get_filename_component(py_filename ${py_file} NAME)
#    message(${py_filename})
#
#endforeach(py_file)
