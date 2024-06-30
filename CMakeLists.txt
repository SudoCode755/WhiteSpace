cmake_minimum_required(VERSION 3.24)


if(WHITESPACE_USE_CONAN)
set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES conan.cmake)
else()
include(FetchContent)

if(NOT WIN32)

FetchContent_Declare(
  GTest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14.0
  OVERRIDE_FIND_PACKAGE)
FetchContent_MakeAvailable(GTest)

else()

FetchContent_Declare(
googletest
GIT_REPOSITORY https://github.com/google/googletest.git
GIT_TAG f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14.0
OVERRIDE_FIND_PACKAGE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

endif()

endif()



project(WhiteSpaces VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set (CMAKE_EXPORT_COMPILE_COMMANDS true)

set(SOURCES
src/arithmeric.cpp
src/flow_control.cpp
src/heap_access.cpp
src/io_ops.cpp
src/env.cpp
src/stack_manip.cpp
src/utils.cpp
src/whiteSpace.cpp
)

set(INCLUDES "include" )


message("src files:")
foreach(file ${SOURCES})
        message(STATUS ${file})
endforeach()

message("include directories:")
foreach(dir ${INCLUDES})
        message(STATUS ${dir})
endforeach()

add_library(WhiteSpace-Core OBJECT ${SOURCES})
add_executable(WhiteSpace src/main.cpp)

enable_testing()
add_subdirectory(test)
target_include_directories(WhiteSpace-Core PUBLIC ${INCLUDES})
target_link_libraries(WhiteSpace PRIVATE
WhiteSpace-Core
)

add_custom_target(
    copy-compile-commands ALL
    ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/build
    )

