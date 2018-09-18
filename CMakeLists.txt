cmake_minimum_required(VERSION 3.9)

project(bwCompression)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED on)

set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS on)

add_library(libbwc SHARED 
MoveToFrontEncoding.cpp SuffixArray.cpp BWTransform.cpp HuffmanCompression.cpp BurrowsWheelerCompression.cpp 
MoveToFrontEncoding.h SuffixArray.h BWTransform.h HuffmanCompression.h BurrowsWheelerCompression.h)

add_library(client SHARED CompressionClient.cpp CompressionClient.h utils/Binary.h)

add_executable(demo demo/main.cpp)       
target_link_libraries(client libbwc)
target_link_libraries(demo client libbwc)