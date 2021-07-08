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

project(VLCQtCore)

# Configuration #
#################
INCLUDE(../../config.cmake)

set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "Generated Files")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add definition for exporting symbols
ADD_DEFINITIONS(-DVLCQT_CORE_LIBRARY -DUNICODE -D_UNICODE)

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
SET(VLCQT_CORE_SRCS_DEPRECATED
    VideoFrame.cpp
    VideoMemoryStream.cpp
)
IF(NOT MSVC)
    SET_SOURCE_FILES_PROPERTIES (${VLCQT_CORE_SRCS_DEPRECATED} PROPERTIES COMPILE_FLAGS -Wno-deprecated)
ENDIF()

# Define the C++ source files
SET(VLCQT_CORE_SRCS
    AbstractVideoFrame.cpp
    AbstractVideoStream.cpp
    Audio.cpp
    Common.cpp
    Enums.cpp
    Error.cpp
    Instance.cpp
    Media.cpp
    MediaList.cpp
    MediaListPlayer.cpp
    MediaPlayer.cpp
    MetaManager.cpp
    ModuleDescription.cpp
    SharedExportCore.h
    Stats.h
    TrackModel.cpp
    Video.cpp
    VideoDelegate.h
    VideoStream.cpp
    YUVVideoFrame.cpp
    ${VLCQT_CORE_SRCS_DEPRECATED}
)

# Deprecated Include files
SET(VLCQT_CORE_HEADERS_DEPRECATED
    VideoFrame.h
    VideoMemoryStream.h
)

# Define the Include files
SET(VLCQT_CORE_HEADERS
    AbstractVideoFrame.h
    AbstractVideoStream.h
    Audio.h
    Common.h
    Enums.h
    Error.h
    Instance.h
    Media.h
    MediaList.h
    MediaListPlayer.h
    MediaPlayer.h
    MetaManager.h
    ModuleDescription.h
    SharedExportCore.h
    Stats.h
    TrackModel.h
    Video.h
    VideoDelegate.h
    VideoStream.h
    YUVVideoFrame.h
    ${VLCQT_CORE_HEADERS_DEPRECATED}
    ${CMAKE_BINARY_DIR}/src/Config.h
)

IF(${LIBVLC_VERSION} EQUAL 0x020200 OR ${LIBVLC_VERSION} GREATER 0x020200)
    SET(VLCQT_CORE_SRCS
        ${VLCQT_CORE_SRCS}
        Equalizer.cpp
    )

    SET(VLCQT_CORE_HEADERS
        ${VLCQT_CORE_HEADERS}
        Equalizer.h
    )
ENDIF()

IF(NOT STATIC AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(VLCQT_CORE_SRCS
        ${VLCQT_CORE_SRCS}
        ${Vlc_Libs}
        ${Vlc_Plugins}
    )
    SET_SOURCE_FILES_PROPERTIES (
        ${Vlc_Libs}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION ${CMAKE_INSTALL_LIBDIR}
    )
    SET_SOURCE_FILES_PROPERTIES (
        ${Vlc_Plugins}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION ${CMAKE_INSTALL_LIBDIR}/vlc/plugins
    )
ENDIF()

IF(MSVC OR MINGW)
    SET(VLCQT_CORE_SRCS
        ${VLCQT_CORE_SRCS}
        compat/asprintf.c
        compat/vasprintf.c
    )
ENDIF()


#####################
# Compiler settings #
#####################
INCLUDE_DIRECTORIES(
    ${LIBVLC_EXTRA_INCLUDE_DIR}
    ${LIBVLC_EXTRA_INCLUDE_DIR}/vlc/plugins
    ${LIBVLC_INCLUDE_DIR}
    ${LIBVLC_INCLUDE_DIR}/vlc/plugins
)

INCLUDE_DIRECTORIES(
    ${VLCQt_ROOT_DIR}/src
    ${VLCQt_ROOT_DIR}/src/core
    ${CMAKE_BINARY_DIR}/src
)

# Additional macros
GENERATE_PKGCONFIG(VLCQtCore core)
GENERATE_WINDOWS_RC(VLCQtCore core VLCQT_CORE_SRCS)

# Create the library.
IF(STATIC)
    ADD_LIBRARY(${PROJECT_NAME} STATIC ${VLCQT_CORE_SRCS} ${VLCQT_CORE_HEADERS})
ELSE()
    ADD_LIBRARY(${PROJECT_NAME} SHARED ${VLCQT_CORE_SRCS} ${VLCQT_CORE_HEADERS})
ENDIF()

# Linux/Mac: Set framework, version and headers
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES FRAMEWORK ON
                                               OUTPUT_NAME "VLCQtCore"
                                               VERSION ${VLCQT_VERSION}
                                               SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
                                               PUBLIC_HEADER "${VLCQT_CORE_HEADERS}"
)

SYMLINK_FRAMEWORK_TEST(${PROJECT_NAME} core VLCQtCore)

if(WIN32)
  if(MSVC)
    # disable SAFESEH - to avoid "LNK2026: module unsafe"
    SET(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} /D \"SAFESEH:NO\"")
    SET(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
    SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
  endif(MSVC)
endif(WIN32)

# Link the required libraries
IF(QT_VERSION MATCHES 4)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE Qt4::QtCore)
ELSE()
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE Qt5::Core)
ENDIF()
IF(NOT STATIC)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${LIBVLC_LIBRARY} ${LIBVLCCORE_LIBRARY})
ENDIF()


# Provide valid includes for other modules
IF(MINGW OR MSVC)
    FILE(COPY ${VLCQT_CORE_HEADERS} DESTINATION "${VLCQT_SDK_INC_OUT}/core")
ELSE()
    FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}")
    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMAND "${CMAKE_COMMAND}" -E create_symlink "${VLCQt_ROOT_DIR}/src/core" "${CMAKE_INSTALL_INCLUDEDIR}/core"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
ENDIF()


################
# Installation #
################
IF(NOT STATIC AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(VLCQtCoreIncludesLocation ${CMAKE_INSTALL_LIBDIR}/VLCQtCore.framework)
  SET(VLCQtCorePublicHeaderLocation ${CMAKE_INSTALL_LIBDIR}/VLCQtCore.framework)
ELSE()
  SET(VLCQtCoreIncludesLocation ${CMAKE_INSTALL_INCLUDEDIR})
  SET(VLCQtCorePublicHeaderLocation ${CMAKE_INSTALL_INCLUDEDIR}/VLCQtCore)
ENDIF()

INSTALL(TARGETS ${PROJECT_NAME} EXPORT VLCQtCoreTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${VLCQtCoreIncludesLocation}
  PUBLIC_HEADER DESTINATION ${VLCQtCorePublicHeaderLocation}
)
