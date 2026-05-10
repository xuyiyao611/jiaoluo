#pragma once

#include <QObject>

class MainWindow;

class ApplicationController : public QObject
{
    Q_OBJECT

public:
    explicit ApplicationController(QObject *parent = nullptr);
    ~ApplicationController() override;

    void show();

private:
    MainWindow *m_mainWindow;
};
