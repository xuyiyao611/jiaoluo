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

enum class FoodKind {
    Pudding,
    Soda,
    Popsicle
};

struct CharacterProgress {
    CharacterKind kind = CharacterKind::WhiteBear;
    int fragments = 0;
    bool unlocked = false;
    int affection = 0;
    bool brokenThrough = false;
};

struct FoodInventory {
    FoodKind kind = FoodKind::Pudding;
    int count = 0;
};

QVector<CharacterProgress> createInitialCharacterProgress();
QVector<FoodInventory> createInitialFoodInventory();

struct AppState {
    SceneKey scene = SceneKey::Landing;
    int coins = 10;
    Difficulty selectedDifficulty = Difficulty::Easy;
    QString lastRoundSummary = QStringLiteral("最近一局：还没有结算记录，先开始一局吧。");
    QVector<CharacterProgress> characters = createInitialCharacterProgress();
    QVector<FoodInventory> foods = createInitialFoodInventory();
};

QString difficultyLabel(Difficulty difficulty);
QString difficultyDescription(Difficulty difficulty);
QVector<CharacterKind> tileKindsForDifficulty(Difficulty difficulty);
QString characterName(CharacterKind kind);
QString characterSymbol(CharacterKind kind);
QString characterImagePath(CharacterKind kind, bool brokenThrough = false);
QString characterDescription(CharacterKind kind);
QStringList characterNames(const QVector<CharacterKind> &kinds);
FoodKind favoriteFoodForCharacter(CharacterKind kind);
QString foodName(FoodKind kind);
QString foodImagePath(FoodKind kind);
int foodPrice(FoodKind kind);
QVector<FoodKind> allFoodKinds();
int affectionGain(CharacterKind kind, FoodKind food);
