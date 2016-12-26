cmake_minimum_required(VERSION 3.2)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)
project(asn1.project)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cmake-modules)

add_subdirectory("${CMAKE_SOURCE_DIR}/3rdparty/babel/babel")
add_subdirectory("${CMAKE_SOURCE_DIR}/3rdparty/iostreams/iostreams")
add_subdirectory("${CMAKE_SOURCE_DIR}/3rdparty/gtest")

add_subdirectory("${CMAKE_SOURCE_DIR}/asn.1")
add_subdirectory("${CMAKE_SOURCE_DIR}/tests")

