cmake_minimum_required(VERSION 3.9)

project(bwCompression)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED on)

set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS on)
add_library(shared SHARED MoveToFrontEncoding.cpp SuffixArray.cpp BWTransform.cpp MoveToFrontEncoding.h SuffixArray.h BWTransform.h)

add_executable(demo demo/main.cpp)        
target_link_libraries(demo shared)