cmake_minimum_required(VERSION 3.19)
project(AerolineaCPP LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

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
)

# Branding embebido directamente en el ejecutable.
# qt_add_resources evita depender del procesamiento implícito de resources.qrc
# y garantiza que los logos estén disponibles en builds locales y de CI.
qt_add_resources(AerolineaCPP "branding_resources"
    PREFIX "/branding"
    FILES
        aerolineacpp_logo.png
        itla_logo.png
)

if(WIN32)
    target_sources(AerolineaCPP PRIVATE app_icon.rc)
endif()

add_custom_command(TARGET AerolineaCPP POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:AerolineaCPP>/config"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/config"
            "$<TARGET_FILE_DIR:AerolineaCPP>/config"
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:AerolineaCPP>/branding"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/aerolineacpp_logo.png"
            "$<TARGET_FILE_DIR:AerolineaCPP>/branding/aerolineacpp_logo.png"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/itla_logo.png"
            "$<TARGET_FILE_DIR:AerolineaCPP>/branding/itla_logo.png"
    COMMENT "Copiando configuración y recursos de branding"
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
