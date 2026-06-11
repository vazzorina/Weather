cmake_minimum_required(VERSION 3.16)

project(Weather VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Widgets Network QuickWidgets Positioning Location)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appWeather
    main.cpp
)

qt_add_qml_module(appWeather
    URI Weather
    QML_FILES
        Main.qml
        MapWidget.qml

    SOURCES
        managermovingicons.h managermovingicons.cpp
        settingstraymenu.h settingstraymenu.cpp
        weatherdata.h weatherdata.cpp
        managerweatherdata.h managerweatherdata.cpp

    RESOURCES
        images/tray-icon-32x32.png
        images/background.png
        images/hour-background.png
        images/ramka2.png
        images/map-marker.png
        images/1000.png
        images/1003.png
        images/1006.png
        images/1009.png
        images/1030.png
        images/1063.png
        images/1066.png

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appWeather PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appWeather
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appWeather
    PRIVATE Qt6::Quick
            Qt6::Widgets
            Qt6::Network
            Qt6::QuickWidgets
            Qt6::Positioning
            Qt6::Location
)

include(GNUInstallDirs)
install(TARGETS appWeather
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
