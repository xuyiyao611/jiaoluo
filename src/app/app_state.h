#pragma once

#include <QString>

enum class SceneKey {
    Landing,
    Home,
    DifficultySelect
};

enum class Difficulty {
    Easy,
    Hard
};

struct AppState {
    SceneKey scene = SceneKey::Landing;
    int coins = 10;
    Difficulty selectedDifficulty = Difficulty::Easy;
};

QString difficultyLabel(Difficulty difficulty);
QString difficultyDescription(Difficulty difficulty);
