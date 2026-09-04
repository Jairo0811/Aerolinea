cmake_minimum_required(VERSION 3.19)
project(AerolineaCPP VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)
qt_standard_project_setup()

qt_add_executable(AerolineaCPP
    WIN32 MACOSX_BUNDLE
    config.hpp.in
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    rutamanager.cpp rutamanager.h
    ruta.cpp ruta.h
    destino.cpp destino.h
    databasemanager.cpp databasemanager.h
    vuelo.cpp vuelo.h
    aeronave.cpp aeronave.h
)

configure_file(config.hpp.in config.hpp)

set_source_files_properties(
    aerolineacpp_logo.png
    PROPERTIES QT_RESOURCE_ALIAS "aerolineacpp.png"
)
set_source_files_properties(
    itla_logo.png
    PROPERTIES QT_RESOURCE_ALIAS "itla.png"
)

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
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/config/database.example.ini"
            "$<TARGET_FILE_DIR:AerolineaCPP>/config/database.example.ini"
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

target_link_libraries(AerolineaCPP PRIVATE Qt::Core Qt::Widgets Qt::Sql)

function(aerolinea_apply_hardening target_name)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE /W4 /permissive- /sdl /guard:cf)
        target_link_options(${target_name} PRIVATE /DYNAMICBASE /NXCOMPAT /guard:cf)
    else()
        target_compile_options(${target_name} PRIVATE
            -Wall -Wextra -Wpedantic -fstack-protector-strong
            $<$<CONFIG:Release>:-D_FORTIFY_SOURCE=3>
        )
        if(UNIX AND NOT APPLE)
            target_link_options(${target_name} PRIVATE -Wl,-z,relro,-z,now)
        endif()
    endif()
endfunction()

aerolinea_apply_hardening(AerolineaCPP)

include(CTest)
if(BUILD_TESTING)
    find_package(Qt6 6.5 REQUIRED COMPONENTS Test)

    qt_add_executable(AerolineaCPPTests
        tests/rutamanager_tests.cpp
        rutamanager.cpp rutamanager.h
        ruta.cpp ruta.h
        destino.cpp destino.h
        vuelo.cpp vuelo.h
        aeronave.cpp aeronave.h
    )
    target_include_directories(AerolineaCPPTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(AerolineaCPPTests PRIVATE Qt::Core Qt::Test)
    aerolinea_apply_hardening(AerolineaCPPTests)

    add_test(NAME RutaManagerTests COMMAND AerolineaCPPTests)
endif()

install(TARGETS AerolineaCPP
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET AerolineaCPP
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
