cmake_minimum_required (VERSION 3.15.2)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
   message(FATAL_ERROR "Please choose a different directory for building")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(EMSCRIPTEN OFF CACHE BOOL "Use emscripten")
if (EMSCRIPTEN)
  SET(EMSCRIPTEN_GENERATE_BITCODE_STATIC_LIBRARIES ON
    CACHE BOOL "")
  message(STATUS "Using Emscripten toolchain")
endif()

project(skirt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -fdiagnostics-color=always")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


set(CMAKE_BUILD_DIRECTORY "build")

enable_testing()

message(STATUS ${CMAKE_SOURCE_DIR})
message(STATUS ${CMAKE_BINARY_DIR})

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release'")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Type of build" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

###############################################################################
# glog

execute_process(
  COMMAND git reset --hard --quiet
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rdp/glog)
execute_process(
  COMMAND git apply ../patches/glog.patch
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rdp/glog)

set(WITH_GFLAGS OFF
  CACHE BOOL "Use gflags")
add_subdirectory(3rdp/glog)


###############################################################################
# yaml-cpp

set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "")
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "")
add_subdirectory(3rdp/yaml-cpp)


###############################################################################
# skirt

include_directories("src")
#add_definitions(-Werror -pedantic
#  -Wno-unused-variable -Wno-unused-parameter -Wno-unused-private-field

file(GLOB_RECURSE SKIRT_SOURCE CONFIGURE_DEPENDS
  src/core/*
  src/loader/*
  )
list(REMOVE_ITEM SKIRT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/core/main.cc")

add_library(skirt STATIC ${SKIRT_SOURCE})

target_link_libraries(skirt yaml-cpp glog)
target_compile_features(skirt PRIVATE
  cxx_std_17
  cxx_constexpr
  cxx_lambdas
  cxx_auto_type)

add_executable(skirt_exe "src/core/main.cc")
set_target_properties(skirt_exe PROPERTIES OUTPUT_NAME skirt)
target_link_libraries(skirt_exe skirt yaml-cpp glog)

set_target_properties(skirt skirt_exe PROPERTIES
  LINK_FLAGS "-flto"
  COMPILE_FLAGS "-Werror -pedantic -Wno-unused-variable -Wno-unused-parameter -Wno-unused-private-field")

if (EMSCRIPTEN)
set_target_properties(skirt skirt_exe PROPERTIES
  COMPILE_FLAGS "--llvm-lto 3")
set_target_properties(skirt_exe PROPERTIES
  LINK_FLAGS "-s EXTRA_EXPORTED_RUNTIME_METHODS='[\"ccall\", \"cwrap\"]'")
else()
set_target_properties(skirt skirt_exe PROPERTIES
  COMPILE_FLAGS "-flto")
endif()

