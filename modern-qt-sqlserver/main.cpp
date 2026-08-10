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

    application.setWindowIcon(QIcon(":/aerolineacpp_logo.png"));

    MainWindow window;
    window.show();

    return application.exec();
}
