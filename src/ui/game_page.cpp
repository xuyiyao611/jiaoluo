#include "game_page.h"

#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLinearGradient>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QRandomGenerator>
#include <QSet>
#include <QSizePolicy>
#include <QVBoxLayout>

namespace {

constexpr int kScorePerTile = 20;
constexpr int kScorePerMoveLeft = 100;
constexpr int kScoreToCoinDivisor = 100;
constexpr int kBoardCellSize = 48;
constexpr int kTileIconSize = 30;

QString positionKey(const QPoint &point)
{
    return QStringLiteral("%1:%2").arg(point.x()).arg(point.y());
}

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
      m_boardWidget(new QWidget(this)),
      m_boardLayout(new QGridLayout()),
      m_selectedCell(-1, -1),
      m_roundFinished(false),
      m_specialTilesLeft(0)
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(20, 14, 20, 16);
    rootLayout->setSpacing(10);

    auto *headerCard = new QFrame(this);
    headerCard->setObjectName(QStringLiteral("gameHeaderCard"));
    auto *headerLayout = new QVBoxLayout(headerCard);
    headerLayout->setContentsMargins(20, 16, 20, 16);
    headerLayout->setSpacing(6);

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
    boardOuterLayout->setContentsMargins(14, 12, 14, 14);
    boardOuterLayout->setSpacing(6);

    auto *boardHeadline = new QLabel(QStringLiteral("9 x 9 正方形棋盘"), boardCard);
    boardHeadline->setObjectName(QStringLiteral("boardHeadline"));
    boardOuterLayout->addWidget(boardHeadline);

    m_boardLayout->setHorizontalSpacing(0);
    m_boardLayout->setVerticalSpacing(0);
    m_boardLayout->setContentsMargins(0, 0, 0, 0);
    m_boardWidget->setObjectName(QStringLiteral("boardWidget"));
    m_boardWidget->setLayout(m_boardLayout);

    auto *boardCenterLayout = new QHBoxLayout();
    boardCenterLayout->setContentsMargins(0, 0, 0, 0);
    boardCenterLayout->addStretch();
    boardCenterLayout->addWidget(m_boardWidget, 0, Qt::AlignCenter);
    boardCenterLayout->addStretch();
    boardOuterLayout->addLayout(boardCenterLayout);

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
        "  border: 1px solid rgba(190, 174, 161, 92);"
        "  border-radius: 24px;"
        "}"
        "#boardWidget {"
        "  background: rgba(255, 248, 240, 220);"
        "  border: 1px solid rgba(201, 187, 177, 72);"
        "  border-radius: 26px;"
        "}"
        "#gameEyebrow {"
        "  color: #b98a72;"
        "  font-size: 11px;"
        "  font-weight: 700;"
        "  letter-spacing: 2px;"
        "}"
        "#gameTitle {"
        "  color: #6b605c;"
        "  font-size: 26px;"
        "  font-weight: 800;"
        "}"
        "#gameMeta, #gameGoal, #gameHint {"
        "  color: #887a73;"
        "  font-size: 12px;"
        "  line-height: 1.6;"
        "}"
        "#boardHeadline {"
        "  color: #6f615b;"
        "  font-size: 15px;"
        "  font-weight: 800;"
        "}"
        "#ghostButton, #accentButton {"
        "  min-height: 38px;"
        "  padding: 8px 16px;"
        "  border-radius: 15px;"
        "  border: none;"
        "  font-size: 13px;"
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
    m_specialTilesLeft = 0;
    m_clearedCounts.fill(0, tileKindsForDifficulty(m_difficulty).size());
    m_selectedCell = QPoint(-1, -1);
    m_roundFinished = false;
    m_statusMessage = QStringLiteral("选中一个格子，再点相邻格子交换。四连会生成行/列消，五连会生成炸弹。");

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
    m_boardWidget->setFixedSize(config.cols * kBoardCellSize, config.rows * kBoardCellSize);

    for (int index = 0; index < total; ++index) {
        auto *cell = new QPushButton(this);
        cell->setCursor(Qt::PointingHandCursor);
        cell->setMinimumSize(kBoardCellSize, kBoardCellSize);
        cell->setMaximumSize(kBoardCellSize, kBoardCellSize);
        cell->setIconSize(QSize(kTileIconSize, kTileIconSize));
        cell->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        const int row = index / config.cols;
        const int col = index % config.cols;
        connect(cell, &QPushButton::clicked, this, [this, row, col]() {
            handleCellClicked(row, col);
        });

        m_boardLayout->addWidget(cell, row, col);
        m_boardCells.push_back(cell);
    }

    m_boardTiles.fill(TileData{}, total);
}

void GamePage::initializeBoard()
{
    const auto config = configForDifficulty(m_difficulty);
    const int total = config.rows * config.cols;
    m_boardTiles.fill(TileData{-1, SpecialType::None}, total);

    for (int row = 0; row < config.rows; ++row) {
        for (int col = 0; col < config.cols; ++col) {
            const int index = boardIndex(row, col);
            TileData tile;
            tile.kind = randomKind();
            m_boardTiles[index] = tile;

            while (hasImmediateMatchAt(m_boardTiles, row, col)) {
                tile.kind = randomKind();
                m_boardTiles[index] = tile;
            }
        }
    }

    while (!hasPlayableState()) {
        for (int index = 0; index < total; ++index) {
            m_boardTiles[index] = TileData{randomKind(), SpecialType::None};
        }

        for (int row = 0; row < config.rows; ++row) {
            for (int col = 0; col < config.cols; ++col) {
                while (hasImmediateMatchAt(m_boardTiles, row, col)) {
                    m_boardTiles[boardIndex(row, col)] = TileData{randomKind(), SpecialType::None};
                }
            }
        }
    }
}

void GamePage::updateBoardView()
{
    const auto config = configForDifficulty(m_difficulty);
    m_specialTilesLeft = 0;

    for (int row = 0; row < config.rows; ++row) {
        for (int col = 0; col < config.cols; ++col) {
            const int index = boardIndex(row, col);
            auto *cell = m_boardCells.at(index);
            const TileData &tile = m_boardTiles.at(index);
            const bool isSelected = m_selectedCell == QPoint(row, col);

            if (auto *existingEffect = cell->graphicsEffect()) {
                existingEffect->deleteLater();
                cell->setGraphicsEffect(nullptr);
            }

            cell->setText(QString());
            cell->setIcon(QIcon(characterImagePath(kindForIndex(tile.kind))));

            if (tile.special != SpecialType::None) {
                ++m_specialTilesLeft;

                auto *glow = new QGraphicsDropShadowEffect(cell);
                glow->setOffset(0, 0);
                glow->setBlurRadius(tile.special == SpecialType::Bomb ? 24 : 18);
                glow->setColor(tile.special == SpecialType::Bomb
                    ? QColor(255, 218, 110, 220)
                    : QColor(129, 191, 255, 225));
                cell->setGraphicsEffect(glow);
            }

            const QString selectedBorder = isSelected
                ? QStringLiteral("3px solid rgba(231, 148, 110, 235)")
                : QStringLiteral("1px solid rgba(255, 255, 255, 120)");

            cell->setStyleSheet(QStringLiteral(
                "QPushButton {"
                "  background: %1;"
                "  border: %2;"
                "  border-radius: 14px;"
                "  padding: 5px;"
                "  qproperty-flat: false;"
                "}"
                "QPushButton:hover {"
                "  background: rgba(255, 255, 255, 0.92);"
                "}"
            ).arg(tileColor(tile.kind) + QStringLiteral(";"), selectedBorder));
        }
    }
}

void GamePage::updateHeaderText()
{
    const auto config = configForDifficulty(m_difficulty);
    const QString difficultyText = difficultyLabel(m_difficulty);

    m_titleLabel->setText(QStringLiteral("角落消消 - %1").arg(difficultyText));
    m_metaLabel->setText(
        QStringLiteral("棋盘：%1 x %2    剩余步数：%3    当前分数：%4    金币：%5    特殊元素：%6")
            .arg(config.rows)
            .arg(config.cols)
            .arg(m_movesLeft)
            .arg(m_score)
            .arg(m_coins)
            .arg(m_specialTilesLeft));
    m_goalLabel->setText(QStringLiteral("目标分数：%1").arg(config.targetScore));

    QString hint = m_statusMessage;
    if (m_score >= config.targetScore && !m_roundFinished) {
        hint += QStringLiteral(" 当前已达到目标分数，再进行一次有效交换就会结算。");
    } else if (m_movesLeft == 0 && !m_roundFinished) {
        hint += QStringLiteral(" 步数已用完，本局将按失败结算。");
    }

    m_hintLabel->setText(hint);
}

QString GamePage::tileColor(int kind) const
{
    static const QStringList colors = {
        QStringLiteral("qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #fff8e8, stop:1 #f6e8c9)"),
        QStringLiteral("qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f9efe3, stop:1 #edd8c4)"),
        QStringLiteral("qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eef8ea, stop:1 #d9ebd0)"),
        QStringLiteral("qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #edf5ff, stop:1 #d8e8fb)"),
        QStringLiteral("qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f4efff, stop:1 #e2d7f8)"),
        QStringLiteral("qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #fff0f1, stop:1 #f7dadd)")
    };

    return colors.value(kind % colors.size(), QStringLiteral("qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #f4f4f4)"));
}

GamePage::Match3DisplayConfig GamePage::configForDifficulty(Difficulty difficulty) const
{
    if (difficulty == Difficulty::Hard) {
        return {9, 9, 27, 4500};
    }

    return {9, 9, 32, 3200};
}

bool GamePage::hasImmediateMatchAt(const QVector<TileData> &board, int row, int col) const
{
    const auto config = configForDifficulty(m_difficulty);
    const int currentKind = board.value(boardIndex(row, col)).kind;
    if (currentKind < 0) {
        return false;
    }

    if (col >= 2
        && board.at(boardIndex(row, col - 1)).kind == currentKind
        && board.at(boardIndex(row, col - 2)).kind == currentKind) {
        return true;
    }

    if (row >= 2
        && board.at(boardIndex(row - 1, col)).kind == currentKind
        && board.at(boardIndex(row - 2, col)).kind == currentKind) {
        return true;
    }

    return false;
}

bool GamePage::isAdjacent(const QPoint &first, const QPoint &second) const
{
    return qAbs(first.x() - second.x()) + qAbs(first.y() - second.y()) == 1;
}

QVector<GamePage::MatchGroup> GamePage::collectMatchGroups(const QVector<TileData> &board) const
{
    const auto config = configForDifficulty(m_difficulty);
    QVector<MatchGroup> groups;

    for (int row = 0; row < config.rows; ++row) {
        int startCol = 0;
        while (startCol < config.cols) {
            const int kind = board.at(boardIndex(row, startCol)).kind;
            int endCol = startCol + 1;
            while (endCol < config.cols && board.at(boardIndex(row, endCol)).kind == kind) {
                ++endCol;
            }

            if (kind >= 0 && endCol - startCol >= 3) {
                MatchGroup group;
                group.horizontal = true;
                for (int col = startCol; col < endCol; ++col) {
                    group.positions.push_back(QPoint(row, col));
                }
                groups.push_back(group);
            }

            startCol = endCol;
        }
    }

    for (int col = 0; col < config.cols; ++col) {
        int startRow = 0;
        while (startRow < config.rows) {
            const int kind = board.at(boardIndex(startRow, col)).kind;
            int endRow = startRow + 1;
            while (endRow < config.rows && board.at(boardIndex(endRow, col)).kind == kind) {
                ++endRow;
            }

            if (kind >= 0 && endRow - startRow >= 3) {
                MatchGroup group;
                group.horizontal = false;
                for (int row = startRow; row < endRow; ++row) {
                    group.positions.push_back(QPoint(row, col));
                }
                groups.push_back(group);
            }

            startRow = endRow;
        }
    }

    return groups;
}

void GamePage::handleCellClicked(int row, int col)
{
    if (m_roundFinished) {
        m_statusMessage = QStringLiteral("本局已经结算，请重新开局或返回主页。");
        updateHeaderText();
        return;
    }

    if (m_movesLeft <= 0) {
        finishRound(false);
        return;
    }

    const QPoint clicked(row, col);
    const TileData &clickedTile = m_boardTiles.at(boardIndex(row, col));

    if (m_selectedCell == QPoint(-1, -1) && clickedTile.special == SpecialType::Bomb) {
        QSet<int> matchedIndexes;
        matchedIndexes.insert(boardIndex(row, col));
        const int activatedSpecialCount = expandSpecialEffects(m_boardTiles, matchedIndexes);

        for (const int index : matchedIndexes) {
            const int kindIndex = m_boardTiles.at(index).kind;
            if (kindIndex >= 0 && kindIndex < m_clearedCounts.size()) {
                m_clearedCounts[kindIndex] += 1;
            }
        }

        m_score += matchedIndexes.size() * kScorePerTile;
        collapseBoard(matchedIndexes);
        --m_movesLeft;
        m_statusMessage = QStringLiteral("已触发九宫格炸弹，消除了 %1 个元素。").arg(matchedIndexes.size());
        if (activatedSpecialCount > 0) {
            m_statusMessage += QStringLiteral(" 连锁触发特殊元素 %1 次。").arg(activatedSpecialCount);
        }

        updateBoardView();
        updateHeaderText();

        if (m_score >= configForDifficulty(m_difficulty).targetScore) {
            finishRound(true);
        } else if (m_movesLeft <= 0) {
            finishRound(false);
        }
        return;
    }

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

    QVector<TileData> probe = m_boardTiles;
    swapCells(probe, m_selectedCell, clicked);
    if (collectMatchGroups(probe).isEmpty()) {
        m_selectedCell = clicked;
        m_statusMessage = QStringLiteral("这次交换没有形成三连，已切换当前选中元素。");
        updateBoardView();
        updateHeaderText();
        return;
    }

    swapCells(m_boardTiles, m_selectedCell, clicked);
    m_selectedCell = QPoint(-1, -1);

    int generatedSpecialCount = 0;
    int activatedSpecialCount = 0;
    resolveMove(clicked, generatedSpecialCount, activatedSpecialCount);
    --m_movesLeft;

    if (generatedSpecialCount > 0) {
        m_statusMessage = QStringLiteral("成功生成了 %1 个特殊元素。").arg(generatedSpecialCount);
    } else if (activatedSpecialCount > 0) {
        m_statusMessage = QStringLiteral("触发了 %1 个特殊元素。").arg(activatedSpecialCount);
    }

    if (!hasPlayableState()) {
        m_statusMessage += QStringLiteral(" 当前棋盘无有效交换，建议重新开局。");
    }

    updateBoardView();
    updateHeaderText();

    if (m_score >= configForDifficulty(m_difficulty).targetScore) {
        finishRound(true);
    } else if (m_movesLeft <= 0) {
        finishRound(false);
    }
}

void GamePage::swapCells(QVector<TileData> &board, const QPoint &first, const QPoint &second) const
{
    const int firstIndex = boardIndex(first.x(), first.y());
    const int secondIndex = boardIndex(second.x(), second.y());
    qSwap(board[firstIndex], board[secondIndex]);
}

void GamePage::resolveMove(const QPoint &preferredSpecialPosition, int &generatedSpecialCount, int &activatedSpecialCount)
{
    QVector<TileData> currentBoard = m_boardTiles;
    QPoint currentPreferredPosition = preferredSpecialPosition;
    int totalCleared = 0;
    generatedSpecialCount = 0;
    activatedSpecialCount = 0;

    while (true) {
        const QVector<MatchGroup> groups = collectMatchGroups(currentBoard);
        if (groups.isEmpty()) {
            break;
        }

        QSet<int> matchedIndexes;
        for (const MatchGroup &group : groups) {
            for (const QPoint &position : group.positions) {
                matchedIndexes.insert(boardIndex(position.x(), position.y()));
            }
        }

        const QVector<SpecialSpawn> spawns = createSpecialSpawns(currentBoard, groups, currentPreferredPosition);
        generatedSpecialCount += applySpecialCreation(currentBoard, matchedIndexes, spawns);
        activatedSpecialCount += expandSpecialEffects(currentBoard, matchedIndexes);

        for (const int index : matchedIndexes) {
            const int kindIndex = currentBoard.at(index).kind;
            if (kindIndex >= 0 && kindIndex < m_clearedCounts.size()) {
                m_clearedCounts[kindIndex] += 1;
            }
        }

        totalCleared += matchedIndexes.size();
        m_boardTiles = currentBoard;
        collapseBoard(matchedIndexes);
        currentBoard = m_boardTiles;
        currentPreferredPosition = QPoint(-1, -1);
    }

    m_score += totalCleared * kScorePerTile;
    if (totalCleared > 0 && generatedSpecialCount == 0 && activatedSpecialCount == 0) {
        m_statusMessage = QStringLiteral("成功消除了 %1 个元素，当前基础分 %2。").arg(totalCleared).arg(m_score);
    }
}

QVector<GamePage::SpecialSpawn> GamePage::createSpecialSpawns(
    const QVector<TileData> &board,
    const QVector<MatchGroup> &groups,
    const QPoint &preferredPosition) const
{
    QVector<MatchGroup> sortedGroups = groups;
    std::sort(sortedGroups.begin(), sortedGroups.end(), [](const MatchGroup &left, const MatchGroup &right) {
        return left.positions.size() > right.positions.size();
    });

    QVector<SpecialSpawn> spawns;
    QSet<QString> usedPositions;

    for (const MatchGroup &group : sortedGroups) {
        if (group.positions.size() < 4) {
            continue;
        }

        QPoint spawnPosition = group.positions.at(group.positions.size() / 2);
        for (const QPoint &position : group.positions) {
            if (position == preferredPosition) {
                spawnPosition = position;
                break;
            }
        }

        const QString key = positionKey(spawnPosition);
        if (usedPositions.contains(key)) {
            continue;
        }

        usedPositions.insert(key);
        SpecialSpawn spawn;
        spawn.position = spawnPosition;
        spawn.kind = board.at(boardIndex(spawnPosition.x(), spawnPosition.y())).kind;
        spawn.special = group.positions.size() >= 5
            ? SpecialType::Bomb
            : (group.horizontal ? SpecialType::RowClear : SpecialType::ColClear);
        spawns.push_back(spawn);
    }

    return spawns;
}

int GamePage::applySpecialCreation(QVector<TileData> &board, QSet<int> &matchedIndexes, const QVector<SpecialSpawn> &spawns) const
{
    int generatedCount = 0;

    for (const SpecialSpawn &spawn : spawns) {
        const int index = boardIndex(spawn.position.x(), spawn.position.y());
        matchedIndexes.remove(index);
        board[index].special = spawn.special;
        board[index].kind = spawn.kind;
        ++generatedCount;
    }

    return generatedCount;
}

int GamePage::expandSpecialEffects(const QVector<TileData> &board, QSet<int> &matchedIndexes) const
{
    const auto config = configForDifficulty(m_difficulty);
    QSet<int> activatedIndexes;

    bool changed = true;
    while (changed) {
        changed = false;

        for (int row = 0; row < config.rows; ++row) {
            for (int col = 0; col < config.cols; ++col) {
                const int index = boardIndex(row, col);
                const TileData &tile = board.at(index);

                if (!matchedIndexes.contains(index) || tile.special == SpecialType::None || activatedIndexes.contains(index)) {
                    continue;
                }

                activatedIndexes.insert(index);

                if (tile.special == SpecialType::RowClear) {
                    for (int clearCol = 0; clearCol < config.cols; ++clearCol) {
                        const int target = boardIndex(row, clearCol);
                        if (!matchedIndexes.contains(target)) {
                            matchedIndexes.insert(target);
                            changed = true;
                        }
                    }
                }

                if (tile.special == SpecialType::ColClear) {
                    for (int clearRow = 0; clearRow < config.rows; ++clearRow) {
                        const int target = boardIndex(clearRow, col);
                        if (!matchedIndexes.contains(target)) {
                            matchedIndexes.insert(target);
                            changed = true;
                        }
                    }
                }

                if (tile.special == SpecialType::Bomb) {
                    for (int targetRow = row - 1; targetRow <= row + 1; ++targetRow) {
                        for (int targetCol = col - 1; targetCol <= col + 1; ++targetCol) {
                            if (targetRow < 0 || targetRow >= config.rows || targetCol < 0 || targetCol >= config.cols) {
                                continue;
                            }

                            const int target = boardIndex(targetRow, targetCol);
                            if (!matchedIndexes.contains(target)) {
                                matchedIndexes.insert(target);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
    }

    return activatedIndexes.size();
}

void GamePage::collapseBoard(const QSet<int> &matchedIndexes)
{
    const auto config = configForDifficulty(m_difficulty);
    QVector<TileData> nextBoard(config.rows * config.cols);

    for (int col = 0; col < config.cols; ++col) {
        QVector<TileData> survivors;
        survivors.reserve(config.rows);

        for (int row = config.rows - 1; row >= 0; --row) {
            const TileData tile = m_boardTiles.at(boardIndex(row, col));
            if (!matchedIndexes.contains(boardIndex(row, col))) {
                survivors.push_back(tile);
            }
        }

        int writeRow = config.rows - 1;
        for (const TileData &tile : survivors) {
            nextBoard[boardIndex(writeRow, col)] = tile;
            --writeRow;
        }

        while (writeRow >= 0) {
            nextBoard[boardIndex(writeRow, col)] = TileData{randomKind(), SpecialType::None};
            --writeRow;
        }
    }

    m_boardTiles = nextBoard;
}

void GamePage::finishRound(bool clearedTarget)
{
    if (m_roundFinished) {
        return;
    }

    m_roundFinished = true;
    m_selectedCell = QPoint(-1, -1);

    int settlementActivatedSpecialCount = 0;
    const int remainingSpecialTiles = clearedTarget ? settleRemainingSpecialTiles(settlementActivatedSpecialCount) : 0;
    const int specialBonusScore = clearedTarget ? remainingSpecialTiles * kScorePerTile : 0;

    const auto config = configForDifficulty(m_difficulty);
    Match3RoundResult result;
    result.clearedTarget = clearedTarget;
    result.baseScore = m_score;
    result.moveBonusScore = clearedTarget ? m_movesLeft * kScorePerMoveLeft : 0;
    result.specialBonusScore = specialBonusScore;
    result.finalScore = result.baseScore + result.moveBonusScore + result.specialBonusScore;
    result.coinReward = clearedTarget ? result.finalScore / kScoreToCoinDivisor : 0;
    result.movesLeft = m_movesLeft;
    result.movesUsed = config.moveLimit - m_movesLeft;
    result.difficulty = m_difficulty;
    result.clearedCounts = m_clearedCounts;
    result.activeKinds = tileKindsForDifficulty(m_difficulty);
    result.remainingSpecialTiles = remainingSpecialTiles;
    result.activatedSpecialCount = settlementActivatedSpecialCount;

    if (clearedTarget) {
        m_statusMessage = QStringLiteral("挑战成功，奖励已结算。");
    } else {
        m_statusMessage = QStringLiteral("挑战失败，本局不发放金币奖励。");
    }
    updateBoardView();
    updateHeaderText();

    QString detailText;
    if (clearedTarget) {
        detailText = QStringLiteral(
            "你已完成目标分数。\n\n基础分：%1\n剩余步数加分：%2\n特殊元素加分：%3\n最终总分：%4\n金币奖励：+%5")
                         .arg(result.baseScore)
                         .arg(result.moveBonusScore)
                         .arg(result.specialBonusScore)
                         .arg(result.finalScore)
                         .arg(result.coinReward);
    } else {
        detailText = QStringLiteral(
            "步数已经用完，未达到目标分数。\n\n基础分：%1\n目标分数：%2\n本局金币奖励：0")
                         .arg(result.baseScore)
                         .arg(config.targetScore);
    }

    QMessageBox::information(
        this,
        clearedTarget ? QStringLiteral("挑战成功") : QStringLiteral("挑战失败"),
        detailText);

    emit roundFinished(result);
}

int GamePage::boardIndex(int row, int col) const
{
    const auto config = configForDifficulty(m_difficulty);
    return row * config.cols + col;
}

int GamePage::randomKind() const
{
    return QRandomGenerator::global()->bounded(tileKindsForDifficulty(m_difficulty).size());
}

CharacterKind GamePage::kindForIndex(int kind) const
{
    return tileKindsForDifficulty(m_difficulty).value(kind, CharacterKind::WhiteBear);
}

bool GamePage::hasPlayableState() const
{
    const auto config = configForDifficulty(m_difficulty);

    for (int row = 0; row < config.rows; ++row) {
        for (int col = 0; col < config.cols; ++col) {
            const QVector<QPoint> directions = {QPoint(row + 1, col), QPoint(row, col + 1)};

            for (const QPoint &other : directions) {
                if (other.x() >= config.rows || other.y() >= config.cols) {
                    continue;
                }

                QVector<TileData> probe = m_boardTiles;
                swapCells(probe, QPoint(row, col), other);
                if (!collectMatchGroups(probe).isEmpty()) {
                    return true;
                }
            }
        }
    }

    return false;
}

int GamePage::settleRemainingSpecialTiles(int &activatedSpecialCount) const
{
    QSet<int> matchedIndexes;
    for (int index = 0; index < m_boardTiles.size(); ++index) {
        if (m_boardTiles.at(index).special != SpecialType::None) {
            matchedIndexes.insert(index);
        }
    }

    if (matchedIndexes.isEmpty()) {
        activatedSpecialCount = 0;
        return 0;
    }

    activatedSpecialCount = expandSpecialEffects(m_boardTiles, matchedIndexes);
    return matchedIndexes.size();
}
