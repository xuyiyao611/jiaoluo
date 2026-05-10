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
    switch (difficulty) {
    case Difficulty::Easy:
        return QStringLiteral("目标分数更低，刷新白熊、鼹鼠、水龙、企鹅、幽灵、猫咪 6 类元素。");
    case Difficulty::Hard:
        return QStringLiteral("目标分数更高，刷新水龙、企鹅、幽灵、猫咪、企鹅？、炸虾尾 6 类元素。");
    }

    return QStringLiteral("未定义");
}
