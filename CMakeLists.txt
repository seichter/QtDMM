#
# CMake build for QtDMM
#
# (c) Hartmut Seichter
#
# Licensed under BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8)

set(PROJECT "QtDMM")

project(${PROJECT})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/etc/cmake ${CMAKE_MODULE_PATH})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 3rd party tools
find_package(Qt5 COMPONENTS Core Widgets PrintSupport SerialPort REQUIRED)

# add hidapi - might need to refactor to make it optional
find_package(HIDAPI REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS}
	src/sources
	${CMAKE_BINARY_DIR}
	${HIDAPI_INCLUDE_DIRS}
	)


add_definitions(${Qt5Widgets_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(HEADERS
	README
	LICENSE
	)

set(SOURCES
	src/sources/Settings.cpp
	src/sources/Settings.h
	src/sources/colorbutton.cpp
	src/sources/colorbutton.h
	src/sources/config.h
	src/sources/configdlg.cpp
	src/sources/configdlg.h
	src/sources/configitem.cpp
	src/sources/configitem.h
#	src/sources/displayprefs.cpp
#	src/sources/displayprefs.h
    src/sources/displaywid.cpp
	src/sources/displaywid.h
	src/sources/dmm.cpp
	src/sources/dmm.h
	src/sources/dmmbar.cpp
	src/sources/dmmbar.h
	src/sources/dmmgraph.cpp
	src/sources/dmmgraph.h
	src/sources/dmmprefs.cpp
	src/sources/dmmprefs.h
	src/sources/engnumbervalidator.cpp
	src/sources/engnumbervalidator.h
	src/sources/executeprefs.cpp
	src/sources/executeprefs.h
	src/sources/graphprefs.cpp
	src/sources/graphprefs.h
	src/sources/guiprefs.cpp
	src/sources/guiprefs.h
	src/sources/integrationprefs.cpp
	src/sources/integrationprefs.h
	src/sources/main.cpp
	src/sources/mainwid.cpp
	src/sources/mainwid.h
	src/sources/mainwin.cpp
	src/sources/mainwin.h
	src/sources/prefwidget.cpp
	src/sources/prefwidget.h
	src/sources/porthandle.cpp
	src/sources/porthandle.h
	src/sources/printdlg.cpp
	src/sources/printdlg.h
	src/sources/qcleanuphandler.h
	src/sources/readerthread.cpp
	src/sources/readerthread.h
	src/sources/readevent.h
	src/sources/recorderprefs.cpp
	src/sources/recorderprefs.h
	src/sources/scaleprefs.cpp
	src/sources/scaleprefs.h
	src/sources/tipdlg.cpp
	src/sources/tipdlg.h
	)

qt5_wrap_ui(UI
	src/forms/uiconfigdlg.ui
	src/forms/uidmmprefs.ui
	src/forms/uiexecuteprefs.ui
	src/forms/uigraphprefs.ui
	src/forms/uiguiprefs.ui
	src/forms/uiintegrationprefs.ui
	src/forms/uimainwid.ui
	src/forms/uimainwin.ui
	src/forms/uiprintdlg.ui
	src/forms/uirecorderprefs.ui
	src/forms/uiscaleprefs.ui
#	src/forms/uitest.ui
    src/forms/uitipdlg.ui
	)

qt5_add_resources(QT_RESOURCES src/Resources.qrc)

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})

add_executable(${PROJECT} MACOSX_BUNDLE WIN32 ${HEADERS} ${SOURCES} ${UI} ${QT_RESOURCES})

target_link_libraries(${PROJECT} ${HIDAPI_LIBRARIES})

qt5_use_modules(${PROJECT} Core Widgets PrintSupport SerialPort)

