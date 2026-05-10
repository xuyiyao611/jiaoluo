#include "main_window.h"

#include <QStackedWidget>

#include "difficulty_select_page.h"
#include "game_page.h"
#include "home_page.h"
#include "landing_page.h"

namespace {

constexpr int kFragmentsPerCharacter = 200;
constexpr int kFragmentsPerBonusCoin = 20;
constexpr int kBreakthroughAffection = 20;

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_stack(new QStackedWidget(this)),
      m_landingPage(new LandingPage(this)),
      m_homePage(new HomePage(this)),
      m_difficultySelectPage(new DifficultySelectPage(this)),
      m_gamePage(new GamePage(this))
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
    m_stack->addWidget(m_gamePage);
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

    connect(m_homePage, &HomePage::buyFoodRequested, this, [this](FoodKind kind) {
        buyFood(kind);
    });

    connect(m_homePage, &HomePage::feedCharacterRequested, this, [this](CharacterKind kind, FoodKind food) {
        feedCharacter(kind, food);
    });

    connect(m_homePage, &HomePage::breakthroughRequested, this, [this](CharacterKind kind) {
        breakthroughCharacter(kind);
    });

    connect(m_difficultySelectPage, &DifficultySelectPage::backRequested, this, [this]() {
        switchScene(SceneKey::Home);
    });

    connect(m_difficultySelectPage, &DifficultySelectPage::difficultyChosen, this, [this](Difficulty difficulty) {
        m_state.selectedDifficulty = difficulty;
        syncStateToViews();
        m_gamePage->startNewRound();
        switchScene(SceneKey::Game);
    });

    connect(m_gamePage, &GamePage::backToDifficultyRequested, this, [this]() {
        switchScene(SceneKey::DifficultySelect);
    });

    connect(m_gamePage, &GamePage::backToHomeRequested, this, [this]() {
        switchScene(SceneKey::Home);
    });

    connect(m_gamePage, &GamePage::roundFinished, this, [this](const Match3RoundResult &result) {
        if (result.clearedTarget) {
            m_state.coins += result.coinReward;
            applyFragmentRewards(result);
        }

        m_state.lastRoundSummary = buildRoundSummary(result);
        syncStateToViews();
        switchScene(SceneKey::Home);
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
    m_homePage->setLastRoundSummary(m_state.lastRoundSummary);
    m_homePage->setCharacterProgress(m_state.characters);
    m_homePage->setFoodInventory(m_state.foods);
    m_difficultySelectPage->setSelectedDifficulty(m_state.selectedDifficulty);
    m_gamePage->setCoins(m_state.coins);
    m_gamePage->setDifficulty(m_state.selectedDifficulty);
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
    case SceneKey::Game:
        m_stack->setCurrentWidget(m_gamePage);
        break;
    }
}

void MainWindow::resetForNewGame()
{
    m_state = AppState{};
    syncStateToViews();
}

QString MainWindow::buildRoundSummary(const Match3RoundResult &result) const
{
    const QString difficultyText = difficultyLabel(result.difficulty);
    if (result.clearedTarget) {
        return QStringLiteral(
                   "最近一局：%1挑战成功，基础分 %2，步数奖励 %3，最终总分 %4，金币 +%5。")
            .arg(difficultyText)
            .arg(result.baseScore)
            .arg(result.moveBonusScore)
            .arg(result.finalScore)
            .arg(result.coinReward);
    }

    return QStringLiteral("最近一局：%1挑战失败，基础分 %2，未获得金币奖励。")
        .arg(difficultyText)
        .arg(result.baseScore);
}

void MainWindow::applyFragmentRewards(const Match3RoundResult &result)
{
    for (int index = 0; index < result.activeKinds.size() && index < result.clearedCounts.size(); ++index) {
        const CharacterKind kind = result.activeKinds.at(index);
        const int gain = result.clearedCounts.at(index);

        for (CharacterProgress &progress : m_state.characters) {
            if (progress.kind != kind) {
                continue;
            }

            const int previousFragments = progress.fragments;
            const int nextFragments = previousFragments + gain;
            const int previousBonusCoins = qMax(previousFragments - kFragmentsPerCharacter, 0) / kFragmentsPerBonusCoin;
            const int nextBonusCoins = qMax(nextFragments - kFragmentsPerCharacter, 0) / kFragmentsPerBonusCoin;

            progress.fragments = nextFragments;
            progress.unlocked = progress.unlocked || nextFragments >= kFragmentsPerCharacter;
            m_state.coins += nextBonusCoins - previousBonusCoins;
            break;
        }
    }
}

void MainWindow::buyFood(FoodKind kind)
{
    const int price = foodPrice(kind);
    if (m_state.coins < price) {
        return;
    }

    m_state.coins -= price;
    for (FoodInventory &food : m_state.foods) {
        if (food.kind == kind) {
            food.count += 1;
            break;
        }
    }
    syncStateToViews();
}

void MainWindow::feedCharacter(CharacterKind kind, FoodKind food)
{
    FoodInventory *targetFood = nullptr;
    for (FoodInventory &entry : m_state.foods) {
        if (entry.kind == food) {
            targetFood = &entry;
            break;
        }
    }

    if (!targetFood || targetFood->count <= 0) {
        return;
    }

    for (CharacterProgress &progress : m_state.characters) {
        if (progress.kind != kind || !progress.unlocked) {
            continue;
        }

        progress.affection += affectionGain(kind, food);
        targetFood->count -= 1;
        break;
    }

    syncStateToViews();
}

void MainWindow::breakthroughCharacter(CharacterKind kind)
{
    for (CharacterProgress &progress : m_state.characters) {
        if (progress.kind != kind || !progress.unlocked || progress.brokenThrough || progress.affection < kBreakthroughAffection) {
            continue;
        }

        progress.brokenThrough = true;
        break;
    }

    syncStateToViews();
}
