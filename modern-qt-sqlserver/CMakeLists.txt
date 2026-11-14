cmake_minimum_required(VERSION 3.19)
project(AerolineaCPP
    VERSION 1.1.1
    DESCRIPTION "AerolineaCPP - consulta y optimización de rutas aéreas"
    LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Atribución única usada por los metadatos del ejecutable de Windows.
set(AEROLINEA_COMPANY
    "Proyecto académico ITLA - mantenido por Francis Jairo Matías Rosario")
set(AEROLINEA_COPYRIGHT
    "© 2018 Francis Jairo Matías Rosario, Jorge de Jesús Torres Pérez y Sebastian Donastor Hernández (ITLA, SOF-012). Modernización 2026: Francis Jairo Matías Rosario.")
set(AEROLINEA_COMMENTS
    "Proyecto académico de portafolio. No es un sistema de operación aérea real.")

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)
qt_standard_project_setup()

configure_file(config.hpp.in config.hpp)

# Código sin dependencias de GUI ni SQL, compartido por la aplicación y los tests.
set(AEROLINEA_CORE_SOURCES
    rutamanager.cpp rutamanager.h
    ruta.cpp ruta.h
    destino.cpp destino.h
    vuelo.cpp vuelo.h
    aeronave.cpp aeronave.h
)

qt_add_executable(AerolineaCPP
    WIN32 MACOSX_BUNDLE
    config.hpp.in
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    databasemanager.cpp databasemanager.h
    configuracionbasedatos.cpp configuracionbasedatos.h
    ${AEROLINEA_CORE_SOURCES}
)

# Recursos embebidos: única fuente de verdad para logos, icono y plantilla.
set_source_files_properties(aerolineacpp_logo.png
    PROPERTIES QT_RESOURCE_ALIAS "aerolineacpp.png")
set_source_files_properties(itla_logo.png
    PROPERTIES QT_RESOURCE_ALIAS "itla.png")
qt_add_resources(AerolineaCPP "branding_resources"
    PREFIX "/branding"
    FILES
        aerolineacpp_logo.png
        aerolineacpp.ico
        itla_logo.png
)
qt_add_resources(AerolineaCPP "config_resources"
    PREFIX "/"
    FILES
        config/database.example.ini
)

if(WIN32)
    set(AEROLINEA_DESCRIPTION "${PROJECT_DESCRIPTION}")
    set(AEROLINEA_ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/aerolineacpp.ico")
    configure_file(app_icon.rc.in "${CMAKE_CURRENT_BINARY_DIR}/app_icon.rc" @ONLY)
    target_sources(AerolineaCPP PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/app_icon.rc")
endif()

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
        ${AEROLINEA_CORE_SOURCES}
    )
    target_include_directories(AerolineaCPPTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(AerolineaCPPTests PRIVATE Qt::Core Qt::Test)
    aerolinea_apply_hardening(AerolineaCPPTests)
    add_test(NAME RutaManagerTests COMMAND AerolineaCPPTests)

    qt_add_executable(AerolineaCPPConfigTests
        tests/configuracion_tests.cpp
        configuracionbasedatos.cpp configuracionbasedatos.h
    )
    target_include_directories(AerolineaCPPConfigTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(AerolineaCPPConfigTests PRIVATE Qt::Core Qt::Test)
    aerolinea_apply_hardening(AerolineaCPPConfigTests)
    add_test(NAME ConfiguracionTests COMMAND AerolineaCPPConfigTests)
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
