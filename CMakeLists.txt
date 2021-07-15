cmake_minimum_required(VERSION 3.20)

set(CMAKE_BUILD_TYPE Release)
project(lua_obfuscator
	VERSION 1.0.0
	DESCRIPTION "some lua 5.1 vm based obfuscator thing"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_LIB_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/lib")

add_compile_options("-O2")
add_compile_options("-stdlib=libstdc++")

add_subdirectory(src)
