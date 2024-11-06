#############################################################################
# VLC-Qt - Qt and libvlc connector library
# Copyright (C) 2016 Tadej Novak <tadej@tano.si>
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library. If not, see <http://www.gnu.org/licenses/>.
#############################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

SET(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/config/platform
    ${CMAKE_MODULE_PATH}
)

option(BUILD_TESTING "Build tests project" ON)

INCLUDE(config.cmake)

SET(VLCQt_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE(ParseVersion)

PROJECT(VLC-Qt
        VERSION ${VLCQT_VERSION})

SET(PROJECT_AUTHOR        "Tadej Novak; Jeffery Jiang")
SET(PROJECT_COPYRIGHT     "Copyright (C) 2021 ${PROJECT_AUTHOR}.")
SET(PROJECT_DESCRIPTION   "VLC-Qt - Qt bindings for libVLC")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(VLCQT_SDK_BIN_OUT              ${VLCQt_OUT_DIR}/VLCQt/bin)
set(VLCQT_SDK_LIB_OUT              ${VLCQt_OUT_DIR}/VLCQt/lib)
set(VLCQT_SDK_INC_OUT              ${VLCQt_OUT_DIR}/VLCQt/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${VLCQT_SDK_BIN_OUT})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${VLCQT_SDK_LIB_OUT}) # for static library
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${VLCQT_SDK_LIB_OUT})

# Append the QT dir to CMAKE_PREFIX_PATH
set(CMAKE_PREFIX_PATH              ${CMAKE_PREFIX_PATH} ${QT_SDK_DIR})

# Determine the platform.
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  set(OS_MACOSX 1)
  set(OS_POSIX 1)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  set(OS_LINUX 1)
  set(OS_POSIX 1)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  set(OS_WINDOWS 1)
endif()

INCLUDE(GNUInstallDirs)
INCLUDE(Macros)
INCLUDE(CompilerConfig)
INCLUDE(Dependencies)
INCLUDE(Platform)

IF(${LIBVLC_VERSION} LESS 0x020200)
  MESSAGE(FATAL_ERROR "Only support libvlc 2.2.0+")
ELSE()
  MESSAGE(STATUS "Use libvlc ${LIBVLC_VERSION}")
ENDIF()

###################
# Compile and set #
###################
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(docs)


#########
# Tests #
#########
IF(BUILD_TESTING)
    IF(NOT MOBILE)
        ENABLE_TESTING(true)
        ADD_SUBDIRECTORY(tests)

        IF(NOT MSVC)
            SET(COVERAGE OFF CACHE BOOL "Coverage report")
        ENDIF()

        IF(COVERAGE)
            MESSAGE(STATUS "Generate coverage report using 'coverage' target")
            INCLUDE(CodeCoverage)
            SETUP_TARGET_FOR_COVERAGE(coverage ${CMAKE_CTEST_COMMAND} "${CMAKE_BINARY_DIR}/coverage")
        ENDIF()
    ENDIF()
ENDIF()

##################################
# CMake Package files generation #
##################################
INCLUDE(Package)
