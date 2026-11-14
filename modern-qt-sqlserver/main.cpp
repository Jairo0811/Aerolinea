#include "config.hpp"
#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    application.setApplicationName(PROJECT_NAME);
    application.setApplicationDisplayName(DISPLAY_NAME);
    application.setApplicationVersion(PROJECT_VERSION);
    application.setOrganizationName(ORG_NAME);
    application.setOrganizationDomain(ORG_DOMAIN);

    // El .ico embebido contiene tamaños optimizados para barra de tareas y
    // títulos; el PNG solo se usa si el plugin de imagen ICO no está presente.
    QIcon applicationIcon(QStringLiteral(":/branding/aerolineacpp.ico"));
    if (applicationIcon.availableSizes().isEmpty()) {
        applicationIcon = QIcon(QStringLiteral(":/branding/aerolineacpp.png"));
    }
    application.setWindowIcon(applicationIcon);

    MainWindow window;
    window.show();

    return application.exec();
}
