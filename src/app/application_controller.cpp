#include "application_controller.h"

#include "ui/main_window.h"

ApplicationController::ApplicationController(QObject *parent)
    : QObject(parent),
      m_mainWindow(new MainWindow())
{
}

ApplicationController::~ApplicationController()
{
    delete m_mainWindow;
}

void ApplicationController::show()
{
    m_mainWindow->show();
}
