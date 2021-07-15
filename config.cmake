#
# Build environment
#

# The Qt SDK path
SET(QT_SDK_DIR "C:/Qt/Qt5.15.2-MSVC2017-x86-Shared")


# The libvlc SDK path
SET(LIBVLC_VERSION      0x030014)
SET(LIBVLC_LIBRARY      "d:/sourcecode/Github/vlc-qt/vlc-3.0.14/sdk/lib/libvlc.lib")
SET(LIBVLCCORE_LIBRARY  "d:/sourcecode/Github/vlc-qt/vlc-3.0.14/sdk/lib/libvlccore.lib")
SET(LIBVLC_INCLUDE_DIR  "d:/sourcecode/Github/vlc-qt/vlc-3.0.14/sdk/include")
SET(LIBVLC_BIN_DIR      "d:/sourcecode/Github/vlc-qt/vlc-3.0.14/sdk/bin")

SET(LIBVLC_PLUGINS_DIR  "${LIBVLC_BIN_DIR}/plugins")
SET(LIBVLC_LIB_DIR      "${LIBVLC_BIN_DIR}/lib")

# VLCQt Version
SET(PROJECT_VERSION_MAJOR 2)
SET(PROJECT_VERSION_MINOR 0)
SET(PROJECT_VERSION_PATCH 1)


SET(VLCQt_OUT_DIR "${CMAKE_SOURCE_DIR}/out")