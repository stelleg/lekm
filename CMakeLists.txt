cmake_minimum_required(VERSION 2.6)

project(VM)
  
set(CMAKE_BUILD_TYPE debug)

set(CMAKE_C_FLAGS_OPT  "-O3")
set(CMAKE_C_FLAGS_TRACE "-DTRACE -Wall -g")
set(CMAKE_C_FLAGS_DEBUG "-DDEBUG -Wall -g")

include_directories(${VM_SOURCE_DIR}/bytecode)
link_directories(${VM_BINARY_DIR}/bytecode)

add_subdirectory(parser)
add_subdirectory(lekm)
