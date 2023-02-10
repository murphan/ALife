# A-Life
2022 OSU Capstone Project

Depends on the JSON for Modern C++ library. You can download the `json.hpp` file from https://github.com/nlohmann/json/releases/tag/v3.11.2.

To set up the build, create a `UserBuild.txt` file in the project's directory and include the following line with the local path to the header file you downloaded:

> `SET(JSONHPP_DIR "C:/Users/{{path to folder containing json.hpp}}")`

To add new files to the CMake project, click the refresh button in the CMake panel in CLion.
Do not manually add files to the `add_executable()` in `CMakeLists.txt`.
