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

project(VLCQtWidgets)

# Configuration #
#################
INCLUDE(../../config.cmake)

# Add definition for exporting symbols
ADD_DEFINITIONS(-DVLCQT_WIDGETS_LIBRARY -DUNICODE -D_UNICODE)

# Add definitions for shared or static library
IF(STATIC)
    ADD_DEFINITIONS(-DQT_STATIC -D_REENTRANT)
ELSE()
    ADD_DEFINITIONS(-DQT_SHARED -D_REENTRANT)
ENDIF()

###################
# List of sources #
###################
# Define the C++ source files
SET(VLCQT_WIDGETS_SRCS
    ControlAudio.cpp
    ControlVideo.cpp
    SharedExportWidgets.h
    WidgetSeek.cpp
    WidgetVideo.cpp
    WidgetVolumeSlider.cpp
)

# Define the Include files
file(GLOB_RECURSE VLCQT_WIDGETS_HEADERS
  "*.h"
)


#####################
# Compiler settings #
#####################
INCLUDE_DIRECTORIES(
    ${VLCQt_ROOT_DIR}/src
    ${VLCQt_ROOT_DIR}/src/widgets
    ${VLCQT_SDK_INC_OUT}/core
)


if(WIN32)
  if(MSVC)
    # disable SAFESEH - to avoid "LNK2026: module unsafe" on "OpenAL32.lib"
    SET(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} /D \"SAFESEH:NO\"")
    SET(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
    SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
  endif(MSVC)
endif(WIN32)

# Additional macros
GENERATE_PKGCONFIG(VLCQtWidgets widgets)
GENERATE_WINDOWS_RC(VLCQtWidgets widgets VLCQT_WIDGETS_SRCS)

# Create the library.
IF(STATIC)
    ADD_LIBRARY(VLCQtWidgets STATIC ${VLCQT_WIDGETS_SRCS} ${VLCQT_WIDGETS_HEADERS})
ELSE()
    ADD_LIBRARY(VLCQtWidgets SHARED ${VLCQT_WIDGETS_SRCS} ${VLCQT_WIDGETS_HEADERS})
ENDIF()

ADD_DEPENDENCIES(VLCQtWidgets VLCQtCore)

# Linux/Mac: Set framework, version and headers
SET_TARGET_PROPERTIES(VLCQtWidgets PROPERTIES FRAMEWORK ON
                                                  OUTPUT_NAME "VLCQtWidgets"
                                                  VERSION ${VLCQT_VERSION}
                                                  SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
                                                  PUBLIC_HEADER "${VLCQT_WIDGETS_HEADERS}"
)

SYMLINK_FRAMEWORK_TEST(VLCQtWidgets widgets VLCQtWidgets)

# Link the required libraries
IF(QT_VERSION MATCHES 4)
    TARGET_LINK_LIBRARIES(VLCQtWidgets PRIVATE VLCQtCore Qt4::QtGui)
ELSE()
    TARGET_LINK_LIBRARIES(VLCQtWidgets PRIVATE VLCQtCore Qt5::Widgets)
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND WITH_X11)
    TARGET_LINK_LIBRARIES(VLCQtWidgets PRIVATE -lX11)
ENDIF()


ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${VLCQT_SDK_INC_OUT}/widgets
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${VLCQT_WIDGETS_HEADERS} ${VLCQT_SDK_INC_OUT}/widgets
)

################
# Installation #
################
IF(NOT STATIC AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(VLCQtWidgetsIncludesLocation ${CMAKE_INSTALL_LIBDIR}/VLCQtWidgets.framework)
    SET(VLCQtWidgetsPublicHeaderLocation ${CMAKE_INSTALL_LIBDIR}/VLCQtWidgets.framework)
ELSE()
    SET(VLCQtWidgetsIncludesLocation ${CMAKE_INSTALL_INCLUDEDIR})
    SET(VLCQtWidgetsPublicHeaderLocation ${CMAKE_INSTALL_INCLUDEDIR}/VLCQtWidgets)
ENDIF()

INSTALL(TARGETS VLCQtWidgets EXPORT VLCQtWidgetsTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${VLCQtWidgetsIncludesLocation}
        PUBLIC_HEADER DESTINATION ${VLCQtWidgetsPublicHeaderLocation}
)
