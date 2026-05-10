#pragma once

#include <QString>
#include <QStringList>
#include <QVector>

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

enum class CharacterKind {
    WhiteBear,
    Mole,
    WaterDragon,
    Penguin,
    Ghost,
    Cat,
    QuestionPenguin,
    FriedShrimpTail
};

struct CharacterProgress {
    CharacterKind kind = CharacterKind::WhiteBear;
    int fragments = 0;
    bool unlocked = false;
};

QVector<CharacterProgress> createInitialCharacterProgress();

struct AppState {
    SceneKey scene = SceneKey::Landing;
    int coins = 10;
    Difficulty selectedDifficulty = Difficulty::Easy;
    QString lastRoundSummary = QStringLiteral("最近一局：还没有结算记录，先开始一局吧。");
    QVector<CharacterProgress> characters = createInitialCharacterProgress();
};

QString difficultyLabel(Difficulty difficulty);
QString difficultyDescription(Difficulty difficulty);
QVector<CharacterKind> tileKindsForDifficulty(Difficulty difficulty);
QString characterName(CharacterKind kind);
QString characterSymbol(CharacterKind kind);
QStringList characterNames(const QVector<CharacterKind> &kinds);
