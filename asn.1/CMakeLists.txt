cmake_minimum_required(VERSION 3.2)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)
project(asn1)

include(GetProjectFiles)
include(SetCompilerOptions)

set(ASN1_FILES)
get_project_files(ASN1_FILES)

add_library(${PROJECT_NAME} STATIC ${ASN1_FILES})
set_compiler_options()

set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/3rdparty/babel/babel/include
    ${CMAKE_SOURCE_DIR}/3rdparty/optional/optional/include
    ${CMAKE_SOURCE_DIR}/3rdparty/liberror/include
)
