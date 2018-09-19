cmake_minimum_required(VERSION 3.9)

project(bwCompression)

set(CMAKE_CXX_STANDARD 14) 
set(CMAKE_CXX_STANDARD_REQUIRED on)

set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS on)

add_library(libbwc SHARED 
MoveToFrontEncoding.cpp SuffixArray.cpp BWTransform.cpp HuffmanCompression.cpp BurrowsWheelerCompression.cpp 
MoveToFrontEncoding.h SuffixArray.h BWTransform.h HuffmanCompression.h BurrowsWheelerCompression.h)

add_library(client SHARED CompressionClient.cpp CompressionClient.h utils/Binary.h)

add_executable(main main/main.cpp)       
target_link_libraries(client libbwc)
target_link_libraries(main client libbwc)

# tests
configure_file(GTest.CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

add_executable(tests test/EndToEndTests.cpp)
target_link_libraries(tests gtest_main libbwc)

include(GoogleTest)
gtest_discover_tests(tests)