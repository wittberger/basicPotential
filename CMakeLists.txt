cmake_minimum_required(VERSION 3.22)

project(
    basicPotential
    VERSION 0.1
    DESCRIPTION "Configurable multipole field which can be visualised"
    LANGUAGES CXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick PrintSupport REQUIRED)



set(PROJECT_SOURCES
        main.cpp
        MultipoleField.cc
        MultipoleField.hh
        PointCharge.cc
        PointCharge.hh
        MultipolePlot.cc
        MultipolePlot.hh
        qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(basicPotential
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(basicPotential SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(basicPotential
          ${PROJECT_SOURCES}
        )
    endif()
endif()

add_subdirectory(qcustomplot)

target_compile_definitions(
    basicPotential
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(
    basicPotential
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Quick
            Qt${QT_VERSION_MAJOR}::PrintSupport
            qcustomplot_static
)
