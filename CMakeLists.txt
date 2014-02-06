cmake_minimum_required(VERSION 2.8)
project(CREATOR)

# A convenience variable:
set(LLVM_ROOT "/usr/local" CACHE PATH "Root of LLVM install.")

# A bit of a sanity check:
if( NOT EXISTS ${LLVM_ROOT}/llvm34/include/llvm )
message(FATAL_ERROR "LLVM_ROOT (${LLVM_ROOT}) is not a valid LLVM 
install")
endif()

# We incorporate the CMake features provided by LLVM:
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LLVM_ROOT}/share/llvm34/cmake")
include(LLVMConfig)

# Now set the header and library paths:
include_directories( ${LLVM_INCLUDE_DIRS} )
link_directories( ${LLVM_LIBRARY_DIRS} )
add_definitions( ${LLVM_DEFINITIONS} )

# FIXME: static {include,link} directories for FreeBSD 10.0 
include_directories( "/usr/local/llvm34/include" )
link_directories( "/usr/local/llvm34/lib" )

message(STATUS "LLVM_INCLUDE: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM_LIBRARY: ${LLVM_LIBRARY_DIRS}")
message(STATUS "LLVM DEFINITIONS: ${LLVM_DEFINITIONS}")

# Let's suppose we want to build a JIT compiler with support for
# binary code (no interpreter):
llvm_map_components_to_libraries(REQ_LLVM_LIBRARIES native)

# Finally, we link the LLVM libraries to our executable:

add_subdirectory(Sources)
