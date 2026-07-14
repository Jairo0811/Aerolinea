#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    application.setApplicationName("AerolineaCPP");
    application.setApplicationDisplayName("AerolineaCPP - Sistema de Rutas");
    application.setApplicationVersion("1.0.0");
    application.setOrganizationName("JM Software Solutions");
    application.setOrganizationDomain("github.com/Jairo0811");

    application.setWindowIcon(QIcon(":/aerolineacpp_logo.png"));

    MainWindow window;
    window.show();

    return application.exec();
}