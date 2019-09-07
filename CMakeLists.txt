cmake_minimum_required (VERSION 3.15.2)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
   message(FATAL_ERROR "Please choose a different directory for building")
endif()

project(skirt LANGUAGES CXX)

set(CMAKE_BUILD_DIRECTORY "build")

enable_testing()

message(STATUS ${CMAKE_SOURCE_DIR})
message(STATUS ${CMAKE_BINARY_DIR})

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release'")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Type of build" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

add_definitions(-std=c++17 -O3 -Wall -Wextra -Werror -pedantic
  -Wno-unused-variable -Wno-unused-parameter -Wno-unused-private-field
  -fdiagnostics-color=always -Isrc/)
