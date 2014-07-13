cmake_minimum_required(VERSION 2.8)
project(solvercreator)

set(CMAKE_BUILD_TYPE Debug)

set(LLVM_ROOT "/usr/local" CACHE PATH "Root of LLVM install.")

if( NOT EXISTS ${LLVM_ROOT}/llvm34/include/llvm )
message(FATAL_ERROR "LLVM_ROOT (${LLVM_ROOT}) is not a valid LLVM 
install")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LLVM_ROOT}/share/llvm34/cmake")
include(LLVMConfig)

include_directories( ${LLVM_INCLUDE_DIRS} )
link_directories( ${LLVM_LIBRARY_DIRS} )
add_definitions( ${LLVM_DEFINITIONS} )

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

# FIXME: static {include,link} directories for FreeBSD 10.0 
include_directories( "/usr/local/llvm34/include" )
include_directories( "./include" )
link_directories( "/usr/local/llvm34/lib" )

message(STATUS "LLVM_INCLUDE: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM_LIBRARY: ${LLVM_LIBRARY_DIRS}")
message(STATUS "LLVM DEFINITIONS: ${LLVM_DEFINITIONS}")

llvm_map_components_to_libraries(REQ_LLVM_LIBRARIES native)

add_library(solvercreator SHARED src/generator.cpp)
target_link_libraries(solvercreator ${REQ_LLVM_LIBRARIES})

add_subdirectory(tests)
