cmake_minimum_required(VERSION 3.19)
project(AerolineaCPP LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql LinguistTools)

qt_standard_project_setup()

qt_add_executable(AerolineaCPP
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    rutamanager.h rutamanager.cpp
    ruta.h ruta.cpp
    destino.h destino.cpp
    databasemanager.h databasemanager.cpp
    vuelo.h vuelo.cpp
    aeronave.h aeronave.cpp
    resources.qrc
)
qt_add_translations(
    TARGETS AerolineaCPP
    TS_FILES AerolineaCPP_es_DO.ts
)

target_link_libraries(AerolineaCPP
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)

install(TARGETS AerolineaCPP
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET AerolineaCPP
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})