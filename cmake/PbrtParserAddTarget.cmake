## ======================================================================== ##
## Copyright 2015-2019 Ingo Wald                                            ##
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

macro(pbrt_parser_link_libraries)
    set(__PBRT_PARSER_LINK_LIBRARIES ${__PBRT_PARSER_LINK_LIBRARIES} ${ARGN})
endmacro()

function(pbrt_parser_add_executable name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} ${__PBRT_PARSER_LINK_LIBRARIES})

    if(PBRT_MACOSX_BUNDLE)
        set_target_properties(${name} PROPERTIES MACOSX_BUNDLE TRUE)
    endif()
endfunction()


#---------------------------------------------------------------------------------------------------
# __pbrt_parser_set_target_postfixes(target)
#

function(__pbrt_parser_set_target_postfixes target)
  if(BUILD_SHARED_LIBS)
    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX "${pbrtParser_SHARED_LIB_POSTFIX_DEBUG}")
    set_target_properties(${target} PROPERTIES RELEASE_POSTFIX "${pbrtParser_SHARED_LIB_POSTFIX_RELEASE}")
    set_target_properties(${target} PROPERTIES MINSIZEREL_POSTFIX "${pbrtParser_SHARED_LIB_POSTFIX_MINSIZEREL}")
    set_target_properties(${target} PROPERTIES RELWITHDEBINFO_POSTFIX "${pbrtParser_SHARED_LIB_POSTFIX_RELWITHDEBINFO}")
  else()
    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX "${pbrtParser_STATIC_LIB_POSTFIX_DEBUG}")
    set_target_properties(${target} PROPERTIES RELEASE_POSTFIX "${pbrtParser_STATIC_LIB_POSTFIX_RELEASE}")
    set_target_properties(${target} PROPERTIES MINSIZEREL_POSTFIX "${pbrtParser_STATIC_LIB_POSTFIX_MINSIZEREL}")
    set_target_properties(${target} PROPERTIES RELWITHDEBINFO_POSTFIX "${pbrtParser_STATIC_LIB_POSTFIX_RELWITHDEBINFO}")
  endif()
endfunction()


#---------------------------------------------------------------------------------------------------
# pbrt_parser_add_library(name, sources...)
#

function(pbrt_parser_add_library name)
    add_library(${name} ${ARGN})
    target_link_libraries(${name} ${__PBRT_PARSER_LINK_LIBRARIES})

    set_target_properties(${name} PROPERTIES VERSION ${pbrtParser_VERSION})

    if(BUILD_SHARED_LIBS)
        target_compile_definitions(${name} PUBLIC PBRT_PARSER_DLL_INTERFACE)
        set_target_properties(${name} PROPERTIES CXX_VISIBILITY_PRESET hidden)
    endif()

    __pbrt_parser_set_target_postfixes(${name})
endfunction()
