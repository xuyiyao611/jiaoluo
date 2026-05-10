#include "app_state.h"

QString difficultyLabel(Difficulty difficulty)
{
    switch (difficulty) {
    case Difficulty::Easy:
        return QStringLiteral("简单模式");
    case Difficulty::Hard:
        return QStringLiteral("困难模式");
    }

    return QStringLiteral("未定义");
}

QString difficultyDescription(Difficulty difficulty)
{
    const QString names = characterNames(tileKindsForDifficulty(difficulty)).join(QStringLiteral("、"));

    switch (difficulty) {
    case Difficulty::Easy:
        return QStringLiteral("目标分数较低，元素池为 %1，共 6 类。").arg(names);
    case Difficulty::Hard:
        return QStringLiteral("目标分数较高，元素池为 %1，共 6 类。").arg(names);
    }

    return QStringLiteral("未定义");
}

QVector<CharacterKind> tileKindsForDifficulty(Difficulty difficulty)
{
    if (difficulty == Difficulty::Hard) {
        return {
            CharacterKind::WaterDragon,
            CharacterKind::Penguin,
            CharacterKind::Ghost,
            CharacterKind::Cat,
            CharacterKind::QuestionPenguin,
            CharacterKind::FriedShrimpTail,
        };
    }

    return {
        CharacterKind::WhiteBear,
        CharacterKind::Mole,
        CharacterKind::WaterDragon,
        CharacterKind::Penguin,
        CharacterKind::Ghost,
        CharacterKind::Cat,
    };
}

QString characterName(CharacterKind kind)
{
    switch (kind) {
    case CharacterKind::WhiteBear:
        return QStringLiteral("白熊");
    case CharacterKind::Mole:
        return QStringLiteral("鼹鼠");
    case CharacterKind::WaterDragon:
        return QStringLiteral("水龙");
    case CharacterKind::Penguin:
        return QStringLiteral("企鹅");
    case CharacterKind::Ghost:
        return QStringLiteral("幽灵");
    case CharacterKind::Cat:
        return QStringLiteral("猫咪");
    case CharacterKind::QuestionPenguin:
        return QStringLiteral("企鹅？");
    case CharacterKind::FriedShrimpTail:
        return QStringLiteral("炸虾尾");
    }

    return QStringLiteral("未知");
}

QString characterSymbol(CharacterKind kind)
{
    switch (kind) {
    case CharacterKind::WhiteBear:
        return QStringLiteral("熊");
    case CharacterKind::Mole:
        return QStringLiteral("鼠");
    case CharacterKind::WaterDragon:
        return QStringLiteral("龙");
    case CharacterKind::Penguin:
        return QStringLiteral("企");
    case CharacterKind::Ghost:
        return QStringLiteral("幽");
    case CharacterKind::Cat:
        return QStringLiteral("猫");
    case CharacterKind::QuestionPenguin:
        return QStringLiteral("谜");
    case CharacterKind::FriedShrimpTail:
        return QStringLiteral("虾");
    }

    return QStringLiteral("?");
}

QString characterImagePath(CharacterKind kind, bool brokenThrough)
{
    const QString prefix = brokenThrough
        ? QStringLiteral(":/images/characters/breakthrough/")
        : QStringLiteral(":/images/characters/");

    switch (kind) {
    case CharacterKind::WhiteBear:
        return prefix + QStringLiteral("white-bear.png");
    case CharacterKind::Mole:
        return prefix + QStringLiteral("mole.png");
    case CharacterKind::WaterDragon:
        return prefix + QStringLiteral("water-dragon.png");
    case CharacterKind::Penguin:
        return prefix + QStringLiteral("penguin.png");
    case CharacterKind::Ghost:
        return prefix + QStringLiteral("ghost.png");
    case CharacterKind::Cat:
        return prefix + QStringLiteral("cat.png");
    case CharacterKind::QuestionPenguin:
        return prefix + QStringLiteral("question-penguin.png");
    case CharacterKind::FriedShrimpTail:
        return prefix + QStringLiteral("fried-shrimp-tail.png");
    }

    return prefix + QStringLiteral("white-bear.png");
}

QString characterDescription(CharacterKind kind)
{
    switch (kind) {
    case CharacterKind::WhiteBear:
        return QStringLiteral("白熊喜欢安静和柔软的角落，性格温吞。");
    case CharacterKind::Mole:
        return QStringLiteral("鼹鼠平时安安静静，但对新鲜事物很好奇。");
    case CharacterKind::WaterDragon:
        return QStringLiteral("水龙外表冷静，其实内心柔软。");
    case CharacterKind::Penguin:
        return QStringLiteral("企鹅总带着一点迟疑感，表情很有辨识度。");
    case CharacterKind::Ghost:
        return QStringLiteral("幽灵存在感很轻，但轮廓独特。");
    case CharacterKind::Cat:
        return QStringLiteral("猫咪看起来懒洋洋的，很适合慢慢养成。");
    case CharacterKind::QuestionPenguin:
        return QStringLiteral("企鹅？有很强的反差感，是后期重点角色。");
    case CharacterKind::FriedShrimpTail:
        return QStringLiteral("炸虾尾辨识度很强，适合做稀有感角色。");
    }

    return QStringLiteral("未知角色");
}

QStringList characterNames(const QVector<CharacterKind> &kinds)
{
    QStringList names;
    names.reserve(kinds.size());
    for (const CharacterKind kind : kinds) {
        names.push_back(characterName(kind));
    }
    return names;
}

FoodKind favoriteFoodForCharacter(CharacterKind kind)
{
    switch (kind) {
    case CharacterKind::WhiteBear:
    case CharacterKind::Penguin:
    case CharacterKind::QuestionPenguin:
        return FoodKind::Popsicle;
    case CharacterKind::Mole:
    case CharacterKind::WaterDragon:
    case CharacterKind::FriedShrimpTail:
        return FoodKind::Soda;
    case CharacterKind::Ghost:
    case CharacterKind::Cat:
        return FoodKind::Pudding;
    }

    return FoodKind::Pudding;
}

QString foodName(FoodKind kind)
{
    switch (kind) {
    case FoodKind::Pudding:
        return QStringLiteral("布丁");
    case FoodKind::Soda:
        return QStringLiteral("汽水");
    case FoodKind::Popsicle:
        return QStringLiteral("冰棍");
    }

    return QStringLiteral("未知食物");
}

QString foodImagePath(FoodKind kind)
{
    switch (kind) {
    case FoodKind::Pudding:
        return QStringLiteral(":/images/foods/pudding.png");
    case FoodKind::Soda:
        return QStringLiteral(":/images/foods/soda.png");
    case FoodKind::Popsicle:
        return QStringLiteral(":/images/foods/popsicle.png");
    }

    return QStringLiteral(":/images/foods/pudding.png");
}

int foodPrice(FoodKind kind)
{
    switch (kind) {
    case FoodKind::Pudding:
        return 20;
    case FoodKind::Soda:
        return 30;
    case FoodKind::Popsicle:
        return 40;
    }

    return 0;
}

QVector<FoodKind> allFoodKinds()
{
    return {FoodKind::Pudding, FoodKind::Soda, FoodKind::Popsicle};
}

int affectionGain(CharacterKind kind, FoodKind food)
{
    return favoriteFoodForCharacter(kind) == food ? 3 : 1;
}

QVector<CharacterProgress> createInitialCharacterProgress()
{
    const QVector<CharacterKind> allKinds = {
        CharacterKind::WhiteBear,
        CharacterKind::Mole,
        CharacterKind::WaterDragon,
        CharacterKind::Penguin,
        CharacterKind::Ghost,
        CharacterKind::Cat,
        CharacterKind::QuestionPenguin,
        CharacterKind::FriedShrimpTail,
    };

    QVector<CharacterProgress> progress;
    progress.reserve(allKinds.size());

    for (const CharacterKind kind : allKinds) {
        CharacterProgress entry;
        entry.kind = kind;
        progress.push_back(entry);
    }

    return progress;
}

QVector<FoodInventory> createInitialFoodInventory()
{
    QVector<FoodInventory> inventory;
    for (const FoodKind kind : allFoodKinds()) {
        FoodInventory entry;
        entry.kind = kind;
        inventory.push_back(entry);
    }
    return inventory;
}
