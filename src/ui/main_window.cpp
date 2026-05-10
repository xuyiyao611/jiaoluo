#include "main_window.h"

#include <QStackedWidget>

#include "difficulty_select_page.h"
#include "home_page.h"
#include "landing_page.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_stack(new QStackedWidget(this)),
      m_landingPage(new LandingPage(this)),
      m_homePage(new HomePage(this)),
      m_difficultySelectPage(new DifficultySelectPage(this))
{
    buildUi();
    bindSignals();
    applyWindowStyle();
    syncStateToViews();
    switchScene(SceneKey::Landing);
}

void MainWindow::buildUi()
{
    setWindowTitle(QStringLiteral("角落消消"));
    resize(1280, 800);
    setMinimumSize(1000, 680);

    m_stack->addWidget(m_landingPage);
    m_stack->addWidget(m_homePage);
    m_stack->addWidget(m_difficultySelectPage);
    setCentralWidget(m_stack);
}

void MainWindow::bindSignals()
{
    connect(m_landingPage, &LandingPage::startRequested, this, [this]() {
        switchScene(SceneKey::Home);
    });

    connect(m_landingPage, &LandingPage::newGameRequested, this, [this]() {
        resetForNewGame();
        switchScene(SceneKey::Home);
    });

    connect(m_homePage, &HomePage::startRequested, this, [this]() {
        switchScene(SceneKey::DifficultySelect);
    });

    connect(m_homePage, &HomePage::newGameRequested, this, [this]() {
        resetForNewGame();
        switchScene(SceneKey::Landing);
    });

    connect(m_difficultySelectPage, &DifficultySelectPage::backRequested, this, [this]() {
        switchScene(SceneKey::Home);
    });

    connect(m_difficultySelectPage, &DifficultySelectPage::difficultyChosen, this, [this](Difficulty difficulty) {
        m_state.selectedDifficulty = difficulty;
        syncStateToViews();
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

void MainWindow::syncStateToViews()
{
    m_homePage->setCoins(m_state.coins);
    m_difficultySelectPage->setSelectedDifficulty(m_state.selectedDifficulty);
}

void MainWindow::switchScene(SceneKey scene)
{
    m_state.scene = scene;

    switch (scene) {
    case SceneKey::Landing:
        m_stack->setCurrentWidget(m_landingPage);
        break;
    case SceneKey::Home:
        m_stack->setCurrentWidget(m_homePage);
        break;
    case SceneKey::DifficultySelect:
        m_stack->setCurrentWidget(m_difficultySelectPage);
        break;
    }
}

void MainWindow::resetForNewGame()
{
    m_state = AppState{};
    syncStateToViews();
}
