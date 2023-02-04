# A-Life
2022 OSU Capstone Project

Depends on the Websocketpp library. Get it from this link: https://github.com/zaphoyd/websocketpp/releases/tag/0.8.2

To set up the build, create a `UserBuild.txt` file in the project's directory and include the following line with the local path to the header files of Websocketpp:

> `SET(JSONHPP_DIR "C:/Users/{{path to folder containing json.hpp}}")`

To add new files to the CMake project, click the refresh button in the CMake panel in CLion.
Do not manually add files to the `add_executable()` in `CMakeLists.txt`.  
