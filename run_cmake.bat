@echo off
set QT_DIR=C:\Qt\Qt5.15.2-MSVC2017-x86-Shared
set PATH=%QT_DIR%;%PATH%

cmake .. -G "Visual Studio 15 2017" -DCMAKE_BUILD_TYPE=Debug ^
  -DCMAKE_INSTALL_PREFIX="d:/sourcecode/Github/vlc-qt/install_dist" ^
  -DLIBVLC_LIBRARY="d:/sourcecode/Github/vlc-qt/vlc-3.0.14/sdk/lib/libvlc.lib" ^
  -DLIBVLCCORE_LIBRARY="d:/sourcecode/Github/vlc-qt/vlc-3.0.14/sdk/lib/libvlccore.lib" ^
  -DLIBVLC_INCLUDE_DIR="d:/sourcecode/Github/vlc-qt/vlc-3.0.14/sdk/lib/include" ^
  -S %~dp0 -B %~dp0build