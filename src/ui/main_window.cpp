#include "main_window.h"

#include <QStackedWidget>

#include "home_page.h"
#include "landing_page.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_stack(new QStackedWidget(this)),
      m_landingPage(new LandingPage(this)),
      m_homePage(new HomePage(this))
{
    buildUi();
    applyWindowStyle();
}

void MainWindow::buildUi()
{
    setWindowTitle(QStringLiteral("角落消消"));
    resize(1280, 800);
    setMinimumSize(1000, 680);

    m_stack->addWidget(m_landingPage);
    m_stack->addWidget(m_homePage);
    m_stack->setCurrentWidget(m_landingPage);
    setCentralWidget(m_stack);

    connect(m_landingPage, &LandingPage::startRequested, this, [this]() {
        m_stack->setCurrentWidget(m_homePage);
    });

    connect(m_landingPage, &LandingPage::newGameRequested, this, [this]() {
        m_stack->setCurrentWidget(m_homePage);
    });
}

void MainWindow::applyWindowStyle()
{
    setStyleSheet(QStringLiteral(
        "QMainWindow {"
        "  background-color: #f8f3ea;"
        "}"
    ));
}
