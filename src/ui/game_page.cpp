#include "game_page.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLinearGradient>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QRandomGenerator>
#include <QVBoxLayout>

namespace {

struct Match3DisplayConfig {
    int rows;
    int cols;
    int moveLimit;
    int targetScore;
};

Match3DisplayConfig configForDifficulty(Difficulty difficulty)
{
    if (difficulty == Difficulty::Hard) {
        return {9, 9, 27, 4500};
    }

    return {9, 9, 32, 3200};
}

} // namespace

GamePage::GamePage(QWidget *parent)
    : QWidget(parent),
      m_difficulty(Difficulty::Easy),
      m_coins(10),
      m_titleLabel(new QLabel(this)),
      m_metaLabel(new QLabel(this)),
      m_goalLabel(new QLabel(this)),
      m_hintLabel(new QLabel(this)),
      m_boardLayout(new QGridLayout())
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(36, 28, 36, 28);
    rootLayout->setSpacing(16);

    auto *headerCard = new QFrame(this);
    headerCard->setObjectName(QStringLiteral("gameHeaderCard"));
    auto *headerLayout = new QVBoxLayout(headerCard);
    headerLayout->setContentsMargins(26, 22, 26, 22);
    headerLayout->setSpacing(8);

    auto *eyebrowLabel = new QLabel(QStringLiteral("P2 游戏页骨架"), headerCard);
    eyebrowLabel->setObjectName(QStringLiteral("gameEyebrow"));

    m_titleLabel->setObjectName(QStringLiteral("gameTitle"));
    m_metaLabel->setObjectName(QStringLiteral("gameMeta"));
    m_goalLabel->setObjectName(QStringLiteral("gameGoal"));
    m_hintLabel->setObjectName(QStringLiteral("gameHint"));
    m_hintLabel->setWordWrap(true);

    headerLayout->addWidget(eyebrowLabel);
    headerLayout->addWidget(m_titleLabel);
    headerLayout->addWidget(m_metaLabel);
    headerLayout->addWidget(m_goalLabel);
    headerLayout->addWidget(m_hintLabel);

    auto *boardCard = new QFrame(this);
    boardCard->setObjectName(QStringLiteral("boardCard"));
    auto *boardOuterLayout = new QVBoxLayout(boardCard);
    boardOuterLayout->setContentsMargins(22, 22, 22, 22);
    boardOuterLayout->setSpacing(12);

    auto *boardHeadline = new QLabel(QStringLiteral("9 x 9 棋盘预览"), boardCard);
    boardHeadline->setObjectName(QStringLiteral("boardHeadline"));
    boardOuterLayout->addWidget(boardHeadline);

    m_boardLayout->setHorizontalSpacing(8);
    m_boardLayout->setVerticalSpacing(8);
    boardOuterLayout->addLayout(m_boardLayout);

    auto *buttonRow = new QHBoxLayout();
    buttonRow->setSpacing(12);

    auto *backDifficultyButton = new QPushButton(QStringLiteral("返回难度选择"), this);
    backDifficultyButton->setObjectName(QStringLiteral("ghostButton"));
    backDifficultyButton->setCursor(Qt::PointingHandCursor);

    auto *backHomeButton = new QPushButton(QStringLiteral("返回主页"), this);
    backHomeButton->setObjectName(QStringLiteral("ghostButton"));
    backHomeButton->setCursor(Qt::PointingHandCursor);

    buttonRow->addWidget(backDifficultyButton);
    buttonRow->addWidget(backHomeButton);
    buttonRow->addStretch();

    rootLayout->addWidget(headerCard);
    rootLayout->addWidget(boardCard);
    rootLayout->addLayout(buttonRow);
    rootLayout->addStretch();

    connect(backDifficultyButton, &QPushButton::clicked, this, &GamePage::backToDifficultyRequested);
    connect(backHomeButton, &QPushButton::clicked, this, &GamePage::backToHomeRequested);

    setStyleSheet(QStringLiteral(
        "#gameHeaderCard, #boardCard {"
        "  background: rgba(255, 251, 246, 225);"
        "  border: 1px solid rgba(190, 174, 161, 110);"
        "  border-radius: 28px;"
        "}"
        "#gameEyebrow {"
        "  color: #b98a72;"
        "  font-size: 13px;"
        "  font-weight: 700;"
        "  letter-spacing: 2px;"
        "}"
        "#gameTitle {"
        "  color: #6b605c;"
        "  font-size: 32px;"
        "  font-weight: 800;"
        "}"
        "#gameMeta, #gameGoal, #gameHint {"
        "  color: #887a73;"
        "  font-size: 15px;"
        "  line-height: 1.6;"
        "}"
        "#boardHeadline {"
        "  color: #6f615b;"
        "  font-size: 20px;"
        "  font-weight: 800;"
        "}"
        "#ghostButton {"
        "  min-height: 46px;"
        "  padding: 10px 20px;"
        "  border-radius: 18px;"
        "  border: none;"
        "  background: #f1e8de;"
        "  color: #6f6761;"
        "  font-size: 16px;"
        "  font-weight: 700;"
        "}"
    ));

    setDifficulty(Difficulty::Easy);
    setCoins(10);
}

void GamePage::setDifficulty(Difficulty difficulty)
{
    m_difficulty = difficulty;
    const auto config = configForDifficulty(difficulty);
    const QString difficultyText = difficulty == Difficulty::Hard ? QStringLiteral("困难模式") : QStringLiteral("简单模式");

    m_titleLabel->setText(QStringLiteral("角落消消 - %1").arg(difficultyText));
    m_metaLabel->setText(QStringLiteral("棋盘：%1 x %2    剩余金币：%3").arg(config.rows).arg(config.cols).arg(m_coins));
    m_goalLabel->setText(QStringLiteral("目标分数：%1    初始步数：%2").arg(config.targetScore).arg(config.moveLimit));
    m_hintLabel->setText(QStringLiteral("当前为 Qt 版游戏页骨架。下一步会把真正的三消交换、消除、连锁与结算逻辑迁移过来。"));

    rebuildBoard();
}

void GamePage::setCoins(int coins)
{
    m_coins = coins;
    setDifficulty(m_difficulty);
}

void GamePage::rebuildBoard()
{
    while (QLayoutItem *item = m_boardLayout->takeAt(0)) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
    m_boardCells.clear();

    const auto config = configForDifficulty(m_difficulty);
    const int total = config.rows * config.cols;

    for (int index = 0; index < total; ++index) {
        auto *cell = new QLabel(tileSymbol(index), this);
        cell->setAlignment(Qt::AlignCenter);
        cell->setMinimumSize(54, 54);
        cell->setStyleSheet(QStringLiteral(
            "background: rgba(255, 255, 255, 0.92);"
            "border: 2px solid rgba(195, 181, 169, 90);"
            "border-radius: 16px;"
            "color: #6f615b;"
            "font-size: 24px;"
            "font-weight: 700;"
        ));

        m_boardLayout->addWidget(cell, index / config.cols, index % config.cols);
        m_boardCells.push_back(cell);
    }
}

QString GamePage::tileSymbol(int index) const
{
    Q_UNUSED(index);

    static const QStringList symbols = {
        QStringLiteral("熊"),
        QStringLiteral("鼠"),
        QStringLiteral("龙"),
        QStringLiteral("企"),
        QStringLiteral("灵"),
        QStringLiteral("猫")
    };

    const int randomIndex = QRandomGenerator::global()->bounded(symbols.size());
    return symbols.at(randomIndex);
}

void GamePage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient backgroundGradient(rect().topLeft(), rect().bottomLeft());
    backgroundGradient.setColorAt(0.0, QColor(251, 247, 240));
    backgroundGradient.setColorAt(1.0, QColor(246, 241, 231));
    painter.fillRect(rect(), backgroundGradient);

    painter.setOpacity(0.24);
    painter.drawPixmap(rect(), QPixmap(QStringLiteral(":/images/page-background.jpg")));
}
