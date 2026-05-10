#pragma once

#include <QMainWindow>

#include "app/app_state.h"
#include "ui/game_page.h"

class QStackedWidget;
class LandingPage;
class HomePage;
class DifficultySelectPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void buildUi();
    void bindSignals();
    void applyWindowStyle();
    void syncStateToViews();
    void switchScene(SceneKey scene);
    void resetForNewGame();
    QString buildRoundSummary(const Match3RoundResult &result) const;

    AppState m_state;
    QStackedWidget *m_stack;
    LandingPage *m_landingPage;
    HomePage *m_homePage;
    DifficultySelectPage *m_difficultySelectPage;
    GamePage *m_gamePage;
};
