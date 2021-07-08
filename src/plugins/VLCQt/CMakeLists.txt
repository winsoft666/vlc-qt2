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

project(VLCQmlPlugin)

# Configuration #
#################
INCLUDE(../../../config.cmake)

# Use solution folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add definition for exporting symbols
ADD_DEFINITIONS(-DVLCQT_PLUGIN_QML -DUNICODE -D_UNICODE)

# Add definitions for shared or static library
IF(STATIC)
    ADD_DEFINITIONS(-DQT_STATICPLUGIN -DQT_PLUGIN)
ELSE()
    ADD_DEFINITIONS(-DQT_SHARED -DQT_PLUGIN)
ENDIF()

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${VLCQT_SDK_LIB_OUT})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${VLCQT_SDK_LIB_OUT})

# Generate files
CONFIGURE_FILE(
    windows.bat.in
    windows.bat
    @ONLY
)

CONFIGURE_FILE(
    linux.sh.in
    linux.sh
    @ONLY
)


###################
# List of sources #
###################
# Define the C++ source files
SET(VLCQT_PLUGIN_QML_SRCS
    VLCQtPlugin.cpp
)

IF(STATIC)
    SET(VLCQT_PLUGIN_QML_SRCS
        ${VLCQT_PLUGIN_QML_SRCS}
        VLCQtPluginResource.qrc
    )
ENDIF()


#####################
# Compiler settings #
#####################
INCLUDE_DIRECTORIES(
    ${VLCQt_ROOT_DIR}/src
    ${VLCQt_ROOT_DIR}/src/qml
    ${VLCQt_ROOT_DIR}/src/plugins/VLCQmlPlugin
    ${VLCQT_SDK_INC_OUT}/core
)

# Additional macros
GENERATE_WINDOWS_RC(VLCQmlPlugin plugins/VLCQmlPlugin VLCQT_PLUGIN_QML_SRCS)

if(WIN32)
  if(MSVC)
    # disable SAFESEH - to avoid "LNK2026: module unsafe" on "OpenAL32.lib"
    SET(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} /D \"SAFESEH:NO\"")
    SET(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
    SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
  endif(MSVC)
endif(WIN32)

# Create the library.
IF(STATIC)
    ADD_LIBRARY(VLCQmlPlugin STATIC ${VLCQT_PLUGIN_QML_SRCS})
    SET_TARGET_PROPERTIES(VLCQmlPlugin PROPERTIES
        AUTOMOC_MOC_OPTIONS -Muri=VLCQmlPlugin)
ELSE()
    ADD_LIBRARY(VLCQmlPlugin SHARED ${VLCQT_PLUGIN_QML_SRCS})
ENDIF()

ADD_DEPENDENCIES(VLCQmlPlugin VLCQtQml)

# Linux/Mac: Set framework, version and headers
SET_TARGET_PROPERTIES(VLCQmlPlugin PROPERTIES FRAMEWORK OFF
                                          OUTPUT_NAME "VLCQmlPlugin"
                                          VERSION ${VLCQT_VERSION}
                                          SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
)
IF(MINGW)
    SET_TARGET_PROPERTIES(VLCQmlPlugin PROPERTIES PREFIX "")
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET_TARGET_PROPERTIES(VLCQmlPlugin PROPERTIES INSTALL_RPATH "@loader_path/../../lib")
ENDIF()

# Link the required libraries
TARGET_LINK_LIBRARIES(VLCQmlPlugin PRIVATE Qt5::Quick VLCQtCore VLCQtQml)

# Generate type info
FILE(COPY qmldir DESTINATION ${CMAKE_BINARY_DIR}/src/plugins/VLCQmlPlugin)


################
# Installation #
################
INSTALL(TARGETS VLCQmlPlugin
    RUNTIME DESTINATION ${QT_INSTALL_QML}/VLCQmlPlugin
    LIBRARY DESTINATION ${QT_INSTALL_QML}/VLCQmlPlugin
    ARCHIVE DESTINATION ${QT_INSTALL_QML}/VLCQmlPlugin
)

INSTALL(FILES qmldir DESTINATION ${QT_INSTALL_QML}/VLCQmlPlugin)
IF(NOT STATIC)
    IF(MSVC OR MINGW)
        IF(NOT CMAKE_BUILD_TYPE MATCHES "Debug")
            INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_BINARY_DIR}/src/plugins/VLCQmlPlugin/windows.bat WORKING_DIRECTORY ${QT_INSTALL_QML})")
        ENDIF()
    ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_BINARY_DIR}/src/plugins/VLCQmlPlugin/linux.sh WORKING_DIRECTORY ${QT_INSTALL_QML})")
    ELSEIF(NOT MOBILE)
        INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${QT_BIN_DIR}/qmlplugindump -nonrelocatable VLCQmlPlugin 1.1 . OUTPUT_FILE VLCQmlPlugin/VLCQmlPlugin.qmltypes WORKING_DIRECTORY ${QT_INSTALL_QML})")
    ENDIF()
ENDIF()
