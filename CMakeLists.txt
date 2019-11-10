cmake_minimum_required(VERSION 3.14)

set(_PROJECT_MAJOR_VERSION 0)
set(_PROJECT_MINOR_VERSION 1)
set(_PROJECT_PATCH_VERSION 0)
set(_PROJECT_VERSION
  ${_PROJECT_MAJOR_VERSION}.${_PROJECT_MINOR_VERSION}.${_PROJECT_PATCH_VERSION})

project(fbi LANGUAGES CXX VERSION "${_PROJECT_VERSION}")

# ############################################################### #
# fbi options ################################################### #
# ############################################################### #
option(fbi_BUILD_TEST "Build library tests." OFF)
option(fbi_BUILD_SHARED "Build the library as shared." OFF)
set(
    fbi_INSTALL_CMAKE_PREFIX 
    "lib/cmake/fbi" 
    CACHE PATH "Installation directory for CMake files")
set(
    fbi_INSTALL_INCLUDE_PREFIX 
    "include/fbi" 
    CACHE PATH "Installation directory for header files")
set(
    fbi_INSTALL_BIN_PREFIX 
    "bin" 
    CACHE PATH "Installation directory for executables")
set(
    fbi_INSTALL_LIB_PREFIX 
    "lib" 
    CACHE PATH "Installation directory for libraries")

# Make x64
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SYSTEM_BIT_SUFFIX x64)
    # Make x86
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(SYSTEM_BIT_SUFFIX x86)
endif()

# Cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules")
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src/fbi)

enable_testing()
if (${fbi_BUILD_TEST})
    add_subdirectory(test)
endif()