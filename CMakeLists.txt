cmake_minimum_required(VERSION 3.5)

project(monitor VERSION 0.1 LANGUAGES CXX)

option(CMAKE_BUILD_RELEASE "debug or release" ON)

if(CMAKE_BUILD_RELEASE)
add_definitions(-DBUILD_RELEASE)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "RELEASE")

set ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS" )

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

if(MSVC)
file(GLOB hotkey_SRC ${CMAKE_CURRENT_SOURCE_DIR}/QHotKey/qhotkey_win.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/QHotKey/qhotkey.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/QHotKey/qhotkey.h
                ${CMAKE_CURRENT_SOURCE_DIR}/QHotKey/qhotkey_p.h)
elseif(UNIX)
file(GLOB hotkey_SRC ${CMAKE_CURRENT_SOURCE_DIR}/QHotKey/qhotkey_x11.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/QHotKey/qhotkey.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/QHotKey/qhotkey.h
                ${CMAKE_CURRENT_SOURCE_DIR}/QHotKey/qhotkey_p.h)
elseif(OSX)
file(GLOB hotkey_SRC ${CMAKE_CURRENT_SOURCE_DIR}/QHotKey/qhotkey_mac.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/QHotKey/qhotkey.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/QHotKey/qhotkey.h
                ${CMAKE_CURRENT_SOURCE_DIR}/QHotKey/qhotkey_p.h)
endif(MSVC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/QHotKey)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mrglobalapplication.h mrglobalapplication.cpp
        keyboardhistory.h keyboardhistory.cpp
        hookutil.h hookutil.cpp
        macrosdef.h
        config.h
        mousehistory.h
        mainwindow.ui
        res.qrc
        ${hotkey_SRC}
        app_win32.rc

        networker.cpp networker.h
        stringutil.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(monitor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(monitor SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(monitor
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(monitor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(monitor PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(monitor)
endif()

if(MSVC)
target_link_libraries(monitor PRIVATE iphlpapi)
endif(MSVC)