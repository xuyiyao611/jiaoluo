#pragma once

#include <QMainWindow>

class QStackedWidget;
class LandingPage;
class HomePage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void buildUi();
    void applyWindowStyle();

    QStackedWidget *m_stack;
    LandingPage *m_landingPage;
    HomePage *m_homePage;
};
