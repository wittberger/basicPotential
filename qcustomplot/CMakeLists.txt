cmake_minimum_required(VERSION 3.22)

project(
    qcustomplot_static
    VERSION 2.1.0
    DESCRIPTION "QCustomPlot library, slightly modified that it runs with newest compiler"
    LANGUAGES CXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


add_library(qcustomplot_static STATIC)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick PrintSupport REQUIRED)

set_target_properties(
    qcustomplot_static
    PROPERTIES OUTPUT_NAME qcustomplot
)

set_target_properties(
    qcustomplot_static
    PROPERTIES VERSION ${PROJECT_VERSION}
               SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_sources(
    qcustomplot_static
    PRIVATE src/qcustomplot.cpp
            include/qcustomplot/qcustomplot.h
)

target_compile_features(
    qcustomplot_static
    PUBLIC cxx_std_17
)

target_include_directories(
    qcustomplot_static
    PRIVATE src/qcustomplot_static
    PUBLIC include
)

target_compile_definitions(
    qcustomplot_static
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(
    qcustomplot_static
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Quick
            Qt${QT_VERSION_MAJOR}::PrintSupport
)
