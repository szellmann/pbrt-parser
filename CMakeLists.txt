## ======================================================================== ##
## Copyright 2009-2019 Ingo Wald                                            ##
##                                                                          ##
## Licensed under the Apache License, Version 2.0 (the "License");          ##
## you may not use this file except in compliance with the License.         ##
## You may obtain a copy of the License at                                  ##
##                                                                          ##
##     http://www.apache.org/licenses/LICENSE-2.0                           ##
##                                                                          ##
## Unless required by applicable law or agreed to in writing, software      ##
## distributed under the License is distributed on an "AS IS" BASIS,        ##
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. ##
## See the License for the specific language governing permissions and      ##
## limitations under the License.                                           ##
## ======================================================================== ##

cmake_minimum_required(VERSION 2.8)

project(pbrtParser)

set(pbrtParser_VERSION_MAJOR 2)
set(pbrtParser_VERSION_MINOR 1)
set(pbrtParser_VERSION_PATCH 3)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(PbrtParserAddTarget)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)

  if(${CMAKE_MAJOR_VERSION} EQUAL 3)
    cmake_policy(SET CMP0042 NEW)
  endif(${CMAKE_MAJOR_VERSION} EQUAL 3)
endif(COMMAND cmake_policy)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall)
  

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# include out local 'include/' dir, we can include our two public
# headers via pbrtParser/Scene.h and pbrtParser/math.h just the way an
# app would include them through /usr/local/
include_directories(${PROJECT_SOURCE_DIR}/pbrtParser/include)

#include_directories(${PROJECT_SOURCE_DIR})
#include_directories(${PROJECT_SOURCE_DIR}/internal/)

#add_subdirectory(internal/ospcommon EXCLUDE_FROM_ALL)

option(BUILD_SHARED_LIBS "Build all libraries as shared libraries instead of static" ON)

set(pbrtParser_VERSION "${pbrtParser_VERSION_MAJOR}.${pbrtParser_VERSION_MINOR}.${pbrtParser_VERSION_PATCH}")

set(pbrtParser_SHARED_LIB_POSTFIX_DEBUG "-gd" CACHE STRING "Postfix for shared libraries (Debug)")
set(pbrtParser_SHARED_LIB_POSTFIX_RELEASE "" CACHE STRING "Postfix for shared libraries (Release)")
set(pbrtParser_SHARED_LIB_POSTFIX_MINSIZEREL "-m" CACHE STRING "Postfix for shared libraries (MinSizeRel)")
set(pbrtParser_SHARED_LIB_POSTFIX_RELWITHDEBINFO "-d" CACHE STRING "Postfix for shared libraries (RelWithDebInfo")

set(pbrtParser_STATIC_LIB_POSTFIX_DEBUG "-sgd" CACHE STRING "Postfix for static libraries (Debug)")
set(pbrtParser_STATIC_LIB_POSTFIX_RELEASE "-s" CACHE STRING "Postfix for static libraries (Release)")
set(pbrtParser_STATIC_LIB_POSTFIX_MINSIZEREL "-sm" CACHE STRING "Postfix for static libraries (MinSizeRel)")
set(pbrtParser_STATIC_LIB_POSTFIX_RELWITHDEBINFO "-sd" CACHE STRING "Postfix for static libraries (RelWithDebInfo")

# syntactic-only parser
#add_subdirectory(pbrtParser/syntactic)
#add_subdirectory(pbrtParser/semantic)
add_subdirectory(pbrtParser)

#add_subdirectory(amalgamation)

add_subdirectory(apps)

