cmake_minimum_required (VERSION 3.15.3)

set(CMAKE_POLICY_DEFAULT_CMP0082 NEW CACHE STRING "")

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
   message(FATAL_ERROR "Please choose a different directory for building")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(EMSCRIPTEN OFF CACHE BOOL "Use emscripten")
if (EMSCRIPTEN)
  message(STATUS "Using Emscripten toolchain")
  SET(EMSCRIPTEN_FORCE_COMPILERS OFF CACHE BOOL "")
  SET(EMSCRIPTEN_GENERATE_BITCODE_STATIC_LIBRARIES ON
    CACHE BOOL "")
endif()

project(skirt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always")

set(COMPILE_OPTIONS
  "-Wall"
  "-Wextra"
  "-Werror"
  "-pedantic"
  "-Wno-unused-variable"
  "-Wno-unused-parameter"
  "-Wno-unused-private-field")

enable_testing()

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release'")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Type of build" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

include(TestBigEndian)
TEST_BIG_ENDIAN(BIG_ENDIAN)

configure_file(src/core/config.h.in config.h)

###############################################################################
# glog

execute_process(
  COMMAND git reset --hard --quiet
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rdp/glog)
execute_process(
  COMMAND git apply ../patches/glog.patch
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rdp/glog)
execute_process(
  COMMAND touch -r configure.ac src/logging.cc
  COMMAND touch -r configure.ac CMakeLists.txt
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rdp/glog)

set(WITH_GFLAGS OFF
  CACHE BOOL "Use gflags")
add_subdirectory(3rdp/glog EXCLUDE_FROM_ALL)


###############################################################################
# yaml-cpp

set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "")
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "")
add_subdirectory(3rdp/yaml-cpp EXCLUDE_FROM_ALL)


###############################################################################
# googletest

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(3rdp/googletest EXCLUDE_FROM_ALL)


###############################################################################
# rapidcheck

set(RC_ENABLE_GTEST ON CACHE BOOL "" FORCE)
set(RC_ENABLE_GMOCK ON CACHE BOOL "" FORCE)
add_subdirectory(3rdp/rapidcheck EXCLUDE_FROM_ALL)

###############################################################################
# skirt

include_directories("src")

file(GLOB_RECURSE SKIRT_SOURCE CONFIGURE_DEPENDS
  src/core/*
  src/loader/*
  src/shapes/*
  src/3rdp/*
  )
list(REMOVE_ITEM SKIRT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/core/main.cc")

add_library(skirt STATIC ${SKIRT_SOURCE})
target_compile_options(skirt PRIVATE ${COMPILE_OPTIONS})

target_link_libraries(skirt yaml-cpp glog)

if(NOT EMSCRIPTEN)
  target_link_libraries(skirt c++fs)
endif()

target_compile_features(skirt PRIVATE
  cxx_std_17
  cxx_constexpr
  cxx_lambdas
  cxx_auto_type)


###############################################################################
# skirt tests

file(GLOB_RECURSE SKIRT_TESTS CONFIGURE_DEPENDS src/tests/*)

add_executable(skirt_tests ${SKIRT_TESTS})
target_link_libraries(skirt_tests gtest rapidcheck_gtest skirt)
target_compile_options(skirt_tests PRIVATE ${COMPILE_OPTIONS})

if (EMSCRIPTEN)
set_target_properties(skirt_tests PROPERTIES
  COMPILE_FLAGS "-s DISABLE_EXCEPTION_CATCHING=0"
  LINK_FLAGS "-s DISABLE_EXCEPTION_CATCHING=0 --pre-js ${CMAKE_CURRENT_SOURCE_DIR}/tools/skirt_tests_pre.js")
endif()


###############################################################################
# skirt executable

add_executable(skirt_exe "src/core/main.cc")
set_target_properties(skirt_exe PROPERTIES OUTPUT_NAME skirt)
target_compile_options(skirt_exe PRIVATE ${COMPILE_OPTIONS})
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

