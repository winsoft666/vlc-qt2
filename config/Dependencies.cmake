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
# Qt

IF(NOT DEFINED QT_VERSION)
    SET(QT_VERSION 5)
ELSE()
    IF(QT_VERSION MATCHES 4)
      MESSAGE(FATAL_ERROR "Only support Qt5+")
    ENDIF()
ENDIF()

FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Quick REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)

FIND_PACKAGE(Qt5QuickTest REQUIRED)
FIND_PACKAGE(Qt5Test REQUIRED)

SET(SYSTEM_QML OFF CACHE BOOL "Install to system QML import path")

MESSAGE(STATUS "Using Qt ${Qt5Core_VERSION}")
MESSAGE(STATUS "Installing to system QML import path: ${SYSTEM_QML}")


IF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND QT_VERSION MATCHES 5 AND Qt5Core_VERSION VERSION_LESS "5.5.0")
    MESSAGE(WARNING "Your Qt5 version is old and support for it will be removed. Please update to Qt 5.5 or later soon.")
ENDIF()

IF((MINGW OR MSVC) AND QT_VERSION MATCHES 5 AND Qt5Core_VERSION VERSION_LESS "5.5.0")
    SET(WITH_GLES OFF CACHE BOOL "Build with OpenGL ES2")
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(WITH_X11 OFF CACHE BOOL "Link with X11")
ENDIF()



# Set Qt bin dir to find QtCoreX.dll and other libs to install
STRING(REGEX REPLACE "([^ ]+)[/\\].*" "\\1" QT_BIN_DIR_TMP "${QT_MOC_EXECUTABLE}")
STRING(REGEX REPLACE "\\\\" "/" QT_BIN_DIR "${QT_BIN_DIR_TMP}")  # Replace back slashes to slashes
STRING(REGEX REPLACE "bin" "lib" QT_LIB_DIR "${QT_BIN_DIR}")

IF(SYSTEM_QML)
    FIND_PROGRAM(QMAKE NAMES qmake-qt5 qmake)
    IF(NOT QMAKE)
        MESSAGE(FATAL_ERROR "qmake not found")
    ENDIF()
    EXECUTE_PROCESS(
        COMMAND ${QMAKE} -query QT_INSTALL_QML
        OUTPUT_VARIABLE QT_INSTALL_QML OUTPUT_STRIP_TRAILING_WHITESPACE
    )
ELSE()
    SET(QT_INSTALL_QML ${CMAKE_INSTALL_PREFIX}/qml)
ENDIF()
