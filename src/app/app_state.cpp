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

QStringList characterNames(const QVector<CharacterKind> &kinds)
{
    QStringList names;
    names.reserve(kinds.size());
    for (const CharacterKind kind : kinds) {
        names.push_back(characterName(kind));
    }
    return names;
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
        entry.fragments = 0;
        entry.unlocked = false;
        progress.push_back(entry);
    }

    return progress;
}
