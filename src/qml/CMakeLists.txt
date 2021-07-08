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

project(VLCQtQml)

# Configuration #
#################
INCLUDE(../../config.cmake)

# Add definition for exporting symbols
ADD_DEFINITIONS(-DVLCQT_QML_LIBRARY -DUNICODE -D_UNICODE)

# Add definitions for shared or static library
IF(STATIC)
    ADD_DEFINITIONS(-DQT_STATIC -D_REENTRANT)
ELSE()
    ADD_DEFINITIONS(-DQT_SHARED -D_REENTRANT)
ENDIF()

###################
# List of sources #
###################
# Deprecated C++ source files
SET(VLCQT_QML_SRCS_DEPRECATED
    QmlVideoObject.cpp
    QmlVideoPlayer.cpp
    painter/GlPainter.cpp
    painter/GlslPainter.cpp
)
IF(NOT MSVC)
    SET_SOURCE_FILES_PROPERTIES (${VLCQT_QML_SRCS_DEPRECATED} PROPERTIES COMPILE_FLAGS -Wno-deprecated)
ENDIF()

# Define the C++ source files
SET(VLCQT_QML_SRCS
    SharedExportQml.h
    Qml.cpp
    QmlPlayer.cpp
    QmlSource.cpp

    QmlVideoOutput.cpp

    rendering/QmlVideoStream.cpp
    rendering/VideoMaterial.cpp
    rendering/VideoMaterialShader.cpp
    rendering/VideoNode.cpp

    ${VLCQT_QML_SRCS_DEPRECATED}
)

# Deprecated Include files
SET(VLCQT_QML_HEADERS_DEPRECATED
    QmlVideoObject.h
    QmlVideoPlayer.h
)

# Define the Include files
SET(VLCQT_QML_HEADERS
    Qml.h
    QmlPlayer.h
    QmlSource.h
    QmlVideoOutput.h
    QmlVideoPlayer.h
    SharedExportQml.h

    ${VLCQT_QML_HEADERS_DEPRECATED}
)

SET(VLCQT_QML_SRCS
    ${VLCQT_QML_SRCS}
    ${VLCQT_QML_HEADERS}
)


#####################
# Compiler settings #
#####################
INCLUDE_DIRECTORIES(
    ${VLCQt_ROOT_DIR}/src
    ${VLCQt_ROOT_DIR}/src/qml
    ${VLCQT_SDK_INC_OUT}/core
)

INCLUDE_DIRECTORIES(
    ${LIBVLC_EXTRA_INCLUDE_DIR}
    ${LIBVLC_EXTRA_INCLUDE_DIR}/vlc/plugins
    ${LIBVLC_INCLUDE_DIR}
    ${LIBVLC_INCLUDE_DIR}/vlc/plugins
)

# Additional macros
GENERATE_PKGCONFIG(VLCQtQml qml)
GENERATE_WINDOWS_RC(VLCQtQml qml VLCQT_QML_SRCS)

# Create the library.
IF(STATIC)
    ADD_LIBRARY(VLCQtQml STATIC ${VLCQT_QML_SRCS} ${VLCQT_QML_HEADERS})
ELSE()
    ADD_LIBRARY(VLCQtQml SHARED ${VLCQT_QML_SRCS} ${VLCQT_QML_HEADERS})
ENDIF()

ADD_DEPENDENCIES(VLCQtQml VLCQtCore)

# MSVC: Fix build
IF(MSVC)
    ADD_DEFINITIONS(-DNOMINMAX)
ENDIF()

if(WIN32)
  if(MSVC)
    # disable SAFESEH - to avoid "LNK2026: module unsafe" on "OpenAL32.lib"
    SET(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} /D \"SAFESEH:NO\"")
    SET(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
    SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
  endif(MSVC)
endif(WIN32)

# Linux/Mac: Set framework, version and headers
SET_TARGET_PROPERTIES(VLCQtQml PROPERTIES FRAMEWORK ON
                                              OUTPUT_NAME "VLCQtQml"
                                              VERSION ${VLCQT_VERSION}
                                              SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
                                              PUBLIC_HEADER "${VLCQT_QML_HEADERS}"
)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET_TARGET_PROPERTIES(VLCQtQml PROPERTIES INSTALL_RPATH "@loader_path/../../../VLCQtCore.framework/Versions/${VLCQT_VERSION}/lib")
ENDIF()

SYMLINK_FRAMEWORK_TEST(VLCQtQml qml VLCQtQml)

# Link the required libraries
TARGET_LINK_LIBRARIES(VLCQtQml PRIVATE Qt5::Quick VLCQtCore)
IF(NOT STATIC)
    TARGET_LINK_LIBRARIES(VLCQtQml PRIVATE ${LIBVLC_LIBRARY} ${LIBVLCCORE_LIBRARY})
    IF(Qt5Core_VERSION VERSION_LESS "5.5.0" AND WITH_GLES)
        IF(MSVC)
            TARGET_LINK_LIBRARIES(VLCQtQml PRIVATE
                                  ${_qt5Core_install_prefix}/lib/libEGL${LE}.lib
                                  ${_qt5Core_install_prefix}/lib/libGLESv2${LE}.lib)
        ENDIF()
        IF(MINGW)
            TARGET_LINK_LIBRARIES(VLCQtQml PRIVATE
                                  ${_qt5Core_install_prefix}/lib/liblibEGL${LE}.a
                                  ${_qt5Core_install_prefix}/lib/liblibGLESv2${LE}.a)
        ENDIF()
    ENDIF()

    IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND WITH_X11)
        TARGET_LINK_LIBRARIES(VLCQtQml PRIVATE -lX11)
    ENDIF()
ENDIF()

################
# Installation #
################
IF(NOT STATIC AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(VLCQtQmlIncludesLocation ${CMAKE_INSTALL_LIBDIR}/VLCQtQml.framework)
    SET(VLCQtQmlPublicHeaderLocation ${CMAKE_INSTALL_LIBDIR}/VLCQtQml.framework)
ELSE()
    SET(VLCQtQmlIncludesLocation ${CMAKE_INSTALL_INCLUDEDIR})
    SET(VLCQtQmlPublicHeaderLocation ${CMAKE_INSTALL_INCLUDEDIR}/VLCQtQml)
ENDIF()

INSTALL(TARGETS VLCQtQml EXPORT VLCQtQmlTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${VLCQtQmlIncludesLocation}
        PUBLIC_HEADER DESTINATION ${VLCQtQmlPublicHeaderLocation}
)
