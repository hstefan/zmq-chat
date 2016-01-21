cmake_minimum_required (VERSION 3.0)

project(ZMQChat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi")
add_definitions(${CXX_COMPILE_FLAGS})

find_package(PkgConfig REQUIRED)
pkg_search_module(ZMQ REQUIRED libzmq)
find_package(Protobuf REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROTOBUF_INCLUDE_DIRS}
  ${ZMQ_INCLUDE_DIRS})
add_subdirectory(src)
