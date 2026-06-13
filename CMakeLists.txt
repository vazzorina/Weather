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

    RESOURCES images/1000.png images/1003.png images/1006.png images/1009.png images/1012.png images/1015.png images/1021.png images/1024.png images/1027.png images/1030.png images/1033.png images/1039.png images/1042.png images/1045.png images/1048.png images/1063.png images/1066.png images/1069.png images/1072.png images/1087.png images/1114.png images/1117.png images/1135.png images/1147.png images/1150.png images/1153.png images/1168.png images/1171.png images/1180.png images/1183.png images/1186.png images/1189.png images/1192.png images/1195.png images/1198.png images/1201.png images/1204.png images/1207.png images/1210.png images/1213.png images/1216.png images/1219.png images/1222.png images/1225.png images/1237.png images/1240.png images/1243.png images/1246.png images/1249.png images/1252.png images/1255.png images/1258.png images/1261.png images/1264.png images/1273.png images/1276.png images/1279.png images/1282.png images/background.png images/hour-background.png images/current-hour-background.png images/map-marker.png images/ramka.png images/tray-icon.png


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
