cmake_minimum_required(VERSION 3.4.1)
project(Player)

set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "Generated Files")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 COMPONENTS Core GUI Svg Widgets Network REQUIRED)

ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

include_directories(
  ${VLCQT_SDK_INC_OUT}
)

file(GLOB_RECURSE _SRC_FILES
  "*.h"
  "*.cpp"
)

file(GLOB_RECURSE _UI_FILES
  "*.ui"
)
source_group("Form Files" FILES ${_UI_FILES})

if (OS_WINDOWS)
  file(GLOB_RECURSE _RES_FILES
    "*.qrc"
    "*.rc"
  )
  source_group("Resource Files" FILES ${_RES_FILES})
endif()

add_executable(${PROJECT_NAME} WIN32 
  ${_SRC_FILES}
  ${_UI_FILES}
  ${_RES_FILES}
)

if (OS_WINDOWS)
  set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Test)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
  Qt5::Core
  Qt5::Gui 
  Qt5::Widgets
  Qt5::Svg
  Qt5::Network
  VLCQtCore
  VLCQtWidgets
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Svg> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Network> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
  COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/imageformats/
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QSvgPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/imageformats/
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${VLCQT_SDK_BIN_OUT}/$<CONFIGURATION> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBVLC_BIN_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>
)