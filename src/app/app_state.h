#pragma once

#include <QString>

enum class SceneKey {
    Landing,
    Home,
    DifficultySelect,
    Game
};

enum class Difficulty {
    Easy,
    Hard
};

struct AppState {
    SceneKey scene = SceneKey::Landing;
    int coins = 10;
    Difficulty selectedDifficulty = Difficulty::Easy;
    QString lastRoundSummary = QStringLiteral("最近一局：还没有结算记录，先开始一局吧。");
};

QString difficultyLabel(Difficulty difficulty);
QString difficultyDescription(Difficulty difficulty);
