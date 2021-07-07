cmake_minimum_required(VERSION 3.5)

project(maksik LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.0.4/mingw81_64")

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
find_package(Qt6Sql)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        database.h
        database.cpp
        auth_form.h
        auth_form.cpp
        auth_form.ui
        sell_form.h
        sell_form.cpp
        sell_form.ui
        book_form.h
        book_form.cpp
        book_form.ui
        empl_form.h
        empl_form.cpp
        empl_form.ui
        statistics_form.h
        statistics_form.cpp
        statistics_form.ui
        settings_form.h
        settings_form.cpp
        settings_form.ui
        Validating.h
        Validating.cpp
        Sell.h
        Book.h
        Employee.h
        Employee.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(maksik
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(maksik SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(maksik
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(maksik PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Sql)
