cmake_minimum_required(VERSION 3.2)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)
project(asn1.tests)

include(GetProjectFiles)
include(SetCompilerOptions)

set(ASN1_TESTS_FILES)
get_project_files(ASN1_TESTS_FILES)

add_executable(${PROJECT_NAME} ${ASN1_TESTS_FILES})
set_compiler_options()

set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY INCLUDE_DIRECTORIES 
    ${CMAKE_SOURCE_DIR}/asn.1/include
    ${CMAKE_SOURCE_DIR}/asn.1/src
    ${CMAKE_SOURCE_DIR}/3rdparty/gtest/include
    ${CMAKE_SOURCE_DIR}/3rdparty/babel/babel/include
    ${CMAKE_SOURCE_DIR}/3rdparty/optional/optional/include
    ${CMAKE_SOURCE_DIR}/3rdparty/iostreams/iostreams/include
    ${CMAKE_SOURCE_DIR}/3rdparty/liberror/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
    add_custom_command(TARGET ${PROJECT_NAME} 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)/data
        COMMENT "copy tests data files"
    )
else()
    add_custom_command(TARGET ${PROJECT_NAME} 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data
        COMMENT "copy tests data files"
    )
endif()

target_link_libraries(${PROJECT_NAME} asn1)
target_link_libraries(${PROJECT_NAME} gtest)
target_link_libraries(${PROJECT_NAME} babel)
target_link_libraries(${PROJECT_NAME} iostreams)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} iconv)
endif()
