cmake_minimum_required(VERSION 2.8)
 
# Set project name and type (C/C++)
project(uebung05 C CXX)

if(${CMAKE_VERSION} VERSION_EQUAL "3.1.0" OR ${CMAKE_VERSION} VERSION_GREATER "3.1.0")
	cmake_policy(SET CMP0054 NEW)
endif()

if(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=gnu++11)
elseif(APPLE)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
elseif(UNIX AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL Clang)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
elseif(UNIX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=gnu++11)
endif()

add_executable(iterators iterators.cpp)
add_executable(search search.cpp)
add_executable(sort sort.cpp)
add_executable(burger burger.cpp)
