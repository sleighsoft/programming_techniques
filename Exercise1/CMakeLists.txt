
# Set project name and type (C/C++)
project(uebung01 C CXX)

if(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=gnu++11)
elseif(APPLE)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
elseif(UNIX AND ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
elseif(UNIX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=gnu++11)
endif()

add_executable(triangular triangular.cpp)
add_executable(fibonacci_recursive fibonacci_recursive.cpp)
add_executable(fibonacci_iterative fibonacci_iterative.cpp)
add_executable(walking_stairs walking_stairs.cpp)
add_executable(change change.cpp)
