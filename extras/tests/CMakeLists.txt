cmake_minimum_required (VERSION 2.8)
project (allo-tests)

set(ALLO_INCLUDE_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/../../include")
include_directories(${ALLO_INCLUDE_FOLDER})

file(GLOB_RECURSE ALLO_INCLUDES "${ALLO_INCLUDE_FOLDER}/*.hpp")
source_group("Headers" FILES ${ALLO_INCLUDES})

add_executable(allo-tests src/tests.cpp ${ALLO_INCLUDES})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
