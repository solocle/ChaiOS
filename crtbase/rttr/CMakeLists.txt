####################################################################################
#                                                                                  #
#  Copyright (c) 2013 Axel Menzel <info@axelmenzel.de>                             #
#                                                                                  #
#  This file is part of the Runtime Type Reflection System (RTTR).                 #
#                                                                                  #
#  Permission is hereby granted, free of charge, to any person obtaining           #
#  a copy of this software and associated documentation files (the "Software"),    #
#  to deal in the Software without restriction, including without limitation       #
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,        #
#  and/or sell copies of the Software, and to permit persons to whom the           #
#  Software is furnished to do so, subject to the following conditions:            #
#                                                                                  #
#  The above copyright notice and this permission notice shall be included in      #
#  all copies or substantial portions of the Software.                             #
#                                                                                  #
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      #
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        #
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     #
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          #
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   #
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   #
#  SOFTWARE.                                                                       #
#                                                                                  #
####################################################################################

project(TypeInfo)

message(STATUS "Scanning "  ${PROJECT_NAME} " module.")
message(STATUS "===========================")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(-DRTTR_DLL -DRTTR_DLL_EXPORTS)

if (Boost_FOUND AND BUILD_WITH_BOOST OR Boost_FOUND AND NOT HAVE_STD_TYPE_TRAITS)
  add_definitions(-DRTTR_USE_BOOST)
  include_directories(${Boost_INCLUDE_DIR})
endif()

loadFolder("TypeInfo" HPP_FILES SRC_FILES)

if (CREATE_UNITY_BUILD)
  generateUnityFile(UnityBuild SRC_FILES)
endif()

if (USE_PCH)
  activate_precompiled_headers("PCH.h" SRC_FILES)
endif()

add_library(TypeInfo SHARED ${UnityBuild} ${SRC_FILES} ${HPP_FILES})
target_link_libraries(TypeInfo)

install(TARGETS TypeInfo
        RUNTIME DESTINATION ${RTTR_BIN_INSTALL_DIR} 
        LIBRARY DESTINATION ${RTTR_BIN_INSTALL_DIR})
        
install(TARGETS TypeInfo
        RUNTIME DESTINATION ${RTTR_LIB_INSTALL_DIR} 
        LIBRARY DESTINATION ${RTTR_LIB_INSTALL_DIR} 
        ARCHIVE DESTINATION ${RTTR_LIB_INSTALL_DIR})

message(STATUS "Scanning " ${PROJECT_NAME} " module finished!")
message(STATUS "")