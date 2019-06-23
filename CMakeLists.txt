cmake_minimum_required(VERSION 3.4.3)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Various LLVM passes implemented")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")


include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS}})
link_directories(${LLVM_LIBRARY_DIRS})


include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src)


