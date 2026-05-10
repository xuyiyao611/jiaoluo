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
#include <QSet>
#include <QStringList>
#include <QVBoxLayout>

namespace {

constexpr int kTileKindCount = 6;
constexpr int kScorePerTile = 100;

} // namespace

GamePage::GamePage(QWidget *parent)
    : QWidget(parent),
      m_difficulty(Difficulty::Easy),
      m_coins(10),
      m_score(0),
      m_movesLeft(0),
      m_titleLabel(new QLabel(this)),
      m_metaLabel(new QLabel(this)),
      m_goalLabel(new QLabel(this)),
      m_hintLabel(new QLabel(this)),
      m_boardLayout(new QGridLayout()),
      m_selectedCell(-1, -1)
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(36, 28, 36, 28);
    rootLayout->setSpacing(16);

    auto *headerCard = new QFrame(this);
    headerCard->setObjectName(QStringLiteral("gameHeaderCard"));
    auto *headerLayout = new QVBoxLayout(headerCard);
    headerLayout->setContentsMargins(26, 22, 26, 22);
    headerLayout->setSpacing(8);

    auto *eyebrowLabel = new QLabel(QStringLiteral("Qt + C++ 三消试玩"), headerCard);
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

    auto *boardHeadline = new QLabel(QStringLiteral("9 x 9 棋盘"), boardCard);
    boardHeadline->setObjectName(QStringLiteral("boardHeadline"));
    boardOuterLayout->addWidget(boardHeadline);

    m_boardLayout->setHorizontalSpacing(8);
    m_boardLayout->setVerticalSpacing(8);
    boardOuterLayout->addLayout(m_boardLayout);

    auto *buttonRow = new QHBoxLayout();
    buttonRow->setSpacing(12);

    auto *restartButton = new QPushButton(QStringLiteral("重新开局"), this);
    restartButton->setObjectName(QStringLiteral("accentButton"));
    restartButton->setCursor(Qt::PointingHandCursor);

    auto *backDifficultyButton = new QPushButton(QStringLiteral("返回难度选择"), this);
    backDifficultyButton->setObjectName(QStringLiteral("ghostButton"));
    backDifficultyButton->setCursor(Qt::PointingHandCursor);

    auto *backHomeButton = new QPushButton(QStringLiteral("返回主页"), this);
    backHomeButton->setObjectName(QStringLiteral("ghostButton"));
    backHomeButton->setCursor(Qt::PointingHandCursor);

    buttonRow->addWidget(restartButton);
    buttonRow->addWidget(backDifficultyButton);
    buttonRow->addWidget(backHomeButton);
    buttonRow->addStretch();

    rootLayout->addWidget(headerCard);
    rootLayout->addWidget(boardCard);
    rootLayout->addLayout(buttonRow);
    rootLayout->addStretch();

    connect(restartButton, &QPushButton::clicked, this, &GamePage::startNewRound);
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
        "#ghostButton, #accentButton {"
        "  min-height: 46px;"
        "  padding: 10px 20px;"
        "  border-radius: 18px;"
        "  border: none;"
        "  font-size: 16px;"
        "  font-weight: 700;"
        "}"
        "#ghostButton {"
        "  background: #f1e8de;"
        "  color: #6f6761;"
        "}"
        "#accentButton {"
        "  background: #efaf7d;"
        "  color: white;"
        "}"
    ));

    setDifficulty(Difficulty::Easy);
    setCoins(10);
    startNewRound();
}

void GamePage::setDifficulty(Difficulty difficulty)
{
    m_difficulty = difficulty;
    updateHeaderText();
}

void GamePage::setCoins(int coins)
{
    m_coins = coins;
    updateHeaderText();
}

void GamePage::startNewRound()
{
    const auto config = configForDifficulty(m_difficulty);
    m_score = 0;
    m_movesLeft = config.moveLimit;
    m_selectedCell = QPoint(-1, -1);
    m_statusMessage = QStringLiteral("选中一个格子，再点相邻格子交换。横向或纵向三个及以上相同元素会消除。");

    rebuildBoardWidgets();
    initializeBoard();
    updateBoardView();
    updateHeaderText();
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

void GamePage::rebuildBoardWidgets()
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
        auto *cell = new QPushButton(this);
        cell->setCursor(Qt::PointingHandCursor);
        cell->setMinimumSize(58, 58);
        cell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        const int row = index / config.cols;
        const int col = index % config.cols;
        connect(cell, &QPushButton::clicked, this, [this, row, col]() {
            handleCellClicked(row, col);
        });

        m_boardLayout->addWidget(cell, row, col);
        m_boardCells.push_back(cell);
    }

    m_boardKinds.fill(0, total);
}

void GamePage::initializeBoard()
{
    const auto config = configForDifficulty(m_difficulty);
    const int total = config.rows * config.cols;
    m_boardKinds.fill(-1, total);

    for (int row = 0; row < config.rows; ++row) {
        for (int col = 0; col < config.cols; ++col) {
            const int index = boardIndex(row, col);

            int kind = randomKind();
            m_boardKinds[index] = kind;

            while (hasMatchAt(row, col)) {
                kind = randomKind();
                m_boardKinds[index] = kind;
            }
        }
    }

    while (!hasPlayableState()) {
        for (int index = 0; index < total; ++index) {
            m_boardKinds[index] = randomKind();
        }

        for (int row = 0; row < config.rows; ++row) {
            for (int col = 0; col < config.cols; ++col) {
                while (hasMatchAt(row, col)) {
                    m_boardKinds[boardIndex(row, col)] = randomKind();
                }
            }
        }
    }
}

void GamePage::updateBoardView()
{
    const auto config = configForDifficulty(m_difficulty);

    for (int row = 0; row < config.rows; ++row) {
        for (int col = 0; col < config.cols; ++col) {
            const int index = boardIndex(row, col);
            auto *cell = m_boardCells.at(index);
            const int kind = m_boardKinds.at(index);
            const bool isSelected = m_selectedCell == QPoint(row, col);

            cell->setText(tileSymbol(kind));

            const QString selectedBorder = isSelected
                ? QStringLiteral("3px solid rgba(231, 148, 110, 230)")
                : QStringLiteral("2px solid rgba(195, 181, 169, 90)");

            cell->setStyleSheet(QStringLiteral(
                "QPushButton {"
                "  background: %1;"
                "  border: %2;"
                "  border-radius: 16px;"
                "  color: #5f5651;"
                "  font-size: 22px;"
                "  font-weight: 700;"
                "}"
                "QPushButton:hover {"
                "  background: #fff9f1;"
                "}"
            ).arg(tileColor(kind), selectedBorder));
        }
    }
}

void GamePage::updateHeaderText()
{
    const auto config = configForDifficulty(m_difficulty);
    const QString difficultyText = m_difficulty == Difficulty::Hard ? QStringLiteral("困难模式") : QStringLiteral("简单模式");

    m_titleLabel->setText(QStringLiteral("角落消消 - %1").arg(difficultyText));
    m_metaLabel->setText(
        QStringLiteral("棋盘：%1 x %2    剩余步数：%3    当前分数：%4    金币：%5")
            .arg(config.rows)
            .arg(config.cols)
            .arg(m_movesLeft)
            .arg(m_score)
            .arg(m_coins));
    m_goalLabel->setText(QStringLiteral("目标分数：%1").arg(config.targetScore));

    QString hint = m_statusMessage;
    if (m_score >= config.targetScore) {
        hint += QStringLiteral(" 当前已达到目标分数。");
    } else if (m_movesLeft == 0) {
        hint += QStringLiteral(" 步数已用完，请重新开局。");
    }

    m_hintLabel->setText(hint);
}

QString GamePage::tileSymbol(int kind) const
{
    static const QStringList symbols = {
        QStringLiteral("熊"),
        QStringLiteral("鼠"),
        QStringLiteral("龙"),
        QStringLiteral("企"),
        QStringLiteral("幽"),
        QStringLiteral("猫")
    };

    return symbols.value(kind % symbols.size(), QStringLiteral("?"));
}

QString GamePage::tileColor(int kind) const
{
    static const QStringList colors = {
        QStringLiteral("#fff6df"),
        QStringLiteral("#f6ead8"),
        QStringLiteral("#e8f4e6"),
        QStringLiteral("#e6f0fb"),
        QStringLiteral("#efe7fb"),
        QStringLiteral("#fbe8ea")
    };

    return colors.value(kind % colors.size(), QStringLiteral("#ffffff"));
}

GamePage::Match3DisplayConfig GamePage::configForDifficulty(Difficulty difficulty) const
{
    if (difficulty == Difficulty::Hard) {
        return {9, 9, 27, 4500};
    }

    return {9, 9, 32, 3200};
}

bool GamePage::hasMatchAt(int row, int col) const
{
    const auto config = configForDifficulty(m_difficulty);
    const int currentKind = m_boardKinds.value(boardIndex(row, col), -1);
    if (currentKind < 0) {
        return false;
    }

    int horizontal = 1;
    for (int left = col - 1; left >= 0; --left) {
        if (m_boardKinds.at(boardIndex(row, left)) != currentKind) {
            break;
        }
        ++horizontal;
    }
    for (int right = col + 1; right < config.cols; ++right) {
        if (m_boardKinds.at(boardIndex(row, right)) != currentKind) {
            break;
        }
        ++horizontal;
    }

    if (horizontal >= 3) {
        return true;
    }

    int vertical = 1;
    for (int up = row - 1; up >= 0; --up) {
        if (m_boardKinds.at(boardIndex(up, col)) != currentKind) {
            break;
        }
        ++vertical;
    }
    for (int down = row + 1; down < config.rows; ++down) {
        if (m_boardKinds.at(boardIndex(down, col)) != currentKind) {
            break;
        }
        ++vertical;
    }

    return vertical >= 3;
}

bool GamePage::isAdjacent(const QPoint &first, const QPoint &second) const
{
    return qAbs(first.x() - second.x()) + qAbs(first.y() - second.y()) == 1;
}

QVector<QPoint> GamePage::collectMatches() const
{
    const auto config = configForDifficulty(m_difficulty);
    QSet<int> indexes;

    for (int row = 0; row < config.rows; ++row) {
        int streakStart = 0;
        while (streakStart < config.cols) {
            const int kind = m_boardKinds.at(boardIndex(row, streakStart));
            int streakEnd = streakStart + 1;
            while (streakEnd < config.cols && m_boardKinds.at(boardIndex(row, streakEnd)) == kind) {
                ++streakEnd;
            }
            if (kind >= 0 && streakEnd - streakStart >= 3) {
                for (int col = streakStart; col < streakEnd; ++col) {
                    indexes.insert(boardIndex(row, col));
                }
            }
            streakStart = streakEnd;
        }
    }

    for (int col = 0; col < config.cols; ++col) {
        int streakStart = 0;
        while (streakStart < config.rows) {
            const int kind = m_boardKinds.at(boardIndex(streakStart, col));
            int streakEnd = streakStart + 1;
            while (streakEnd < config.rows && m_boardKinds.at(boardIndex(streakEnd, col)) == kind) {
                ++streakEnd;
            }
            if (kind >= 0 && streakEnd - streakStart >= 3) {
                for (int row = streakStart; row < streakEnd; ++row) {
                    indexes.insert(boardIndex(row, col));
                }
            }
            streakStart = streakEnd;
        }
    }

    QVector<QPoint> matches;
    matches.reserve(indexes.size());
    for (const int index : indexes) {
        matches.push_back(QPoint(index / config.cols, index % config.cols));
    }
    return matches;
}

void GamePage::handleCellClicked(int row, int col)
{
    if (m_movesLeft <= 0) {
        m_statusMessage = QStringLiteral("本局步数已用完，请点击“重新开局”。");
        updateHeaderText();
        return;
    }

    const QPoint clicked(row, col);
    if (m_selectedCell == QPoint(-1, -1)) {
        m_selectedCell = clicked;
        m_statusMessage = QStringLiteral("已选中一个元素，请点击相邻元素进行交换。");
        updateBoardView();
        updateHeaderText();
        return;
    }

    if (m_selectedCell == clicked) {
        m_selectedCell = QPoint(-1, -1);
        m_statusMessage = QStringLiteral("已取消选中。");
        updateBoardView();
        updateHeaderText();
        return;
    }

    if (!isAdjacent(m_selectedCell, clicked)) {
        m_selectedCell = clicked;
        m_statusMessage = QStringLiteral("只能和上下左右相邻元素交换，已切换选中格子。");
        updateBoardView();
        updateHeaderText();
        return;
    }

    const QPoint first = m_selectedCell;
    m_selectedCell = QPoint(-1, -1);
    swapCells(first, clicked);
    updateBoardView();

    QVector<QPoint> matches = collectMatches();
    if (matches.isEmpty()) {
        swapCells(first, clicked);
        m_statusMessage = QStringLiteral("这次交换没有形成三连，已自动换回。");
        updateBoardView();
        updateHeaderText();
        return;
    }

    --m_movesLeft;
    resolveMatches(matches);

    if (!hasPlayableState()) {
        m_statusMessage += QStringLiteral(" 当前棋盘无有效交换，系统已为下一局逻辑预留重开。");
    }

    updateBoardView();
    updateHeaderText();
}

void GamePage::swapCells(const QPoint &first, const QPoint &second)
{
    const int firstIndex = boardIndex(first.x(), first.y());
    const int secondIndex = boardIndex(second.x(), second.y());
    qSwap(m_boardKinds[firstIndex], m_boardKinds[secondIndex]);
}

void GamePage::resolveMatches(const QVector<QPoint> &matches)
{
    QVector<QPoint> currentMatches = matches;
    int chain = 1;

    while (!currentMatches.isEmpty()) {
        for (const QPoint &point : currentMatches) {
            m_boardKinds[boardIndex(point.x(), point.y())] = -1;
        }

        m_score += currentMatches.size() * kScorePerTile * chain;
        refillBoard();

        const QVector<QPoint> nextMatches = collectMatches();
        if (nextMatches.isEmpty()) {
            m_statusMessage = QStringLiteral("成功消除了 %1 个元素，获得 %2 分。")
                                  .arg(currentMatches.size())
                                  .arg(currentMatches.size() * kScorePerTile * chain);
            break;
        }

        m_statusMessage = QStringLiteral("触发连锁消除，第 %1 段继续中。").arg(chain + 1);
        currentMatches = nextMatches;
        ++chain;
    }
}

void GamePage::refillBoard()
{
    const auto config = configForDifficulty(m_difficulty);

    for (int col = 0; col < config.cols; ++col) {
        QVector<int> keptKinds;
        keptKinds.reserve(config.rows);

        for (int row = config.rows - 1; row >= 0; --row) {
            const int kind = m_boardKinds.at(boardIndex(row, col));
            if (kind >= 0) {
                keptKinds.push_back(kind);
            }
        }

        int writeRow = config.rows - 1;
        for (int keptIndex = 0; keptIndex < keptKinds.size(); ++keptIndex) {
            m_boardKinds[boardIndex(writeRow, col)] = keptKinds.at(keptIndex);
            --writeRow;
        }

        while (writeRow >= 0) {
            m_boardKinds[boardIndex(writeRow, col)] = randomKind();
            --writeRow;
        }
    }
}

int GamePage::boardIndex(int row, int col) const
{
    const auto config = configForDifficulty(m_difficulty);
    return row * config.cols + col;
}

int GamePage::randomKind() const
{
    return QRandomGenerator::global()->bounded(kTileKindCount);
}

bool GamePage::hasPlayableState() const
{
    const auto config = configForDifficulty(m_difficulty);
    QVector<int> backup = m_boardKinds;

    for (int row = 0; row < config.rows; ++row) {
        for (int col = 0; col < config.cols; ++col) {
            const QVector<QPoint> directions = {QPoint(row + 1, col), QPoint(row, col + 1)};

            for (const QPoint &other : directions) {
                if (other.x() >= config.rows || other.y() >= config.cols) {
                    continue;
                }

                QVector<int> probe = backup;
                const int firstIndex = row * config.cols + col;
                const int secondIndex = other.x() * config.cols + other.y();
                qSwap(probe[firstIndex], probe[secondIndex]);

                auto hasMatchFromProbe = [&](int targetRow, int targetCol) {
                    const int currentKind = probe.at(targetRow * config.cols + targetCol);
                    int horizontal = 1;
                    for (int left = targetCol - 1; left >= 0; --left) {
                        if (probe.at(targetRow * config.cols + left) != currentKind) {
                            break;
                        }
                        ++horizontal;
                    }
                    for (int right = targetCol + 1; right < config.cols; ++right) {
                        if (probe.at(targetRow * config.cols + right) != currentKind) {
                            break;
                        }
                        ++horizontal;
                    }
                    if (horizontal >= 3) {
                        return true;
                    }

                    int vertical = 1;
                    for (int up = targetRow - 1; up >= 0; --up) {
                        if (probe.at(up * config.cols + targetCol) != currentKind) {
                            break;
                        }
                        ++vertical;
                    }
                    for (int down = targetRow + 1; down < config.rows; ++down) {
                        if (probe.at(down * config.cols + targetCol) != currentKind) {
                            break;
                        }
                        ++vertical;
                    }
                    return vertical >= 3;
                };

                if (hasMatchFromProbe(row, col) || hasMatchFromProbe(other.x(), other.y())) {
                    return true;
                }
            }
        }
    }

    return false;
}
