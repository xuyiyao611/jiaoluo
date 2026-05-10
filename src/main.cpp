#include <QApplication>

#include "app/application_controller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("角落消消"));
    app.setOrganizationName(QStringLiteral("Nankai University"));

    ApplicationController controller;
    controller.show();

    return app.exec();
}
