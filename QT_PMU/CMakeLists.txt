cmake_minimum_required(VERSION 3.16)
project(PMU
  VERSION 1.0
  LANGUAGES CXX
)

find_package(Qt6
  6.3
  REQUIRED
  COMPONENTS
    Core
    Gui
    Charts
    PrintSupport
    Widgets
)
qt_standard_project_setup()


qt_add_executable(pmu
  "main.cpp"
  "mainwindow.cpp"
  "mainwindow.h"
  "monitorgraph.cpp"
  "monitorgraph.h"
  "qcustomplot.cpp"
  "qcustomplot.h"
)

set_target_properties(pmu
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(pmu
  PRIVATE QT_DEPRECATED_WARNINGS
)

target_include_directories(pmu
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(pmu
  PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Charts
    Qt::PrintSupport
    Qt::Widgets
)

qt_add_resources(pmu
  "Icons"
    PREFIX "/icons"
    FILES
      "res/home_256.png"
      "res/wave-graph.png"
)
