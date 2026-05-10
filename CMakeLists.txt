cmake_minimum_required(VERSION 3.21)

project(CornerMatchQt VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

qt_add_executable(CornerMatchQt
    src/main.cpp
    src/app/app_state.h
    src/app/app_state.cpp
    src/app/application_controller.h
    src/app/application_controller.cpp
    src/ui/difficulty_select_page.h
    src/ui/difficulty_select_page.cpp
    src/ui/main_window.h
    src/ui/main_window.cpp
    src/ui/landing_page.h
    src/ui/landing_page.cpp
    src/ui/home_page.h
    src/ui/home_page.cpp
    resources/resources.qrc
)

target_include_directories(CornerMatchQt
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(CornerMatchQt
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

if (WIN32)
    set_target_properties(CornerMatchQt PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
