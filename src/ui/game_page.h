#pragma once

#include <QPoint>
#include <QVector>
#include <QWidget>

#include "app/app_state.h"

class QLabel;
class QGridLayout;
class QPushButton;

struct Match3RoundResult {
    bool clearedTarget = false;
    int baseScore = 0;
    int moveBonusScore = 0;
    int finalScore = 0;
    int coinReward = 0;
    int movesUsed = 0;
    int movesLeft = 0;
    Difficulty difficulty = Difficulty::Easy;
    QVector<int> clearedCounts;
    QVector<CharacterKind> activeKinds;
};

Q_DECLARE_METATYPE(Match3RoundResult)

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr);

    void setDifficulty(Difficulty difficulty);
    void setCoins(int coins);
    void startNewRound();

signals:
    void backToHomeRequested();
    void backToDifficultyRequested();
    void roundFinished(const Match3RoundResult &result);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    struct Match3DisplayConfig {
        int rows;
        int cols;
        int moveLimit;
        int targetScore;
    };

    void rebuildBoardWidgets();
    void initializeBoard();
    void updateBoardView();
    void updateHeaderText();
    QString tileSymbol(int kind) const;
    QString tileColor(int kind) const;
    Match3DisplayConfig configForDifficulty(Difficulty difficulty) const;
    bool hasMatchAt(int row, int col) const;
    bool isAdjacent(const QPoint &first, const QPoint &second) const;
    QVector<QPoint> collectMatches() const;
    void handleCellClicked(int row, int col);
    void swapCells(const QPoint &first, const QPoint &second);
    void resolveMatches(const QVector<QPoint> &matches);
    void refillBoard();
    void finishRound(bool clearedTarget);
    int boardIndex(int row, int col) const;
    int randomKind() const;
    CharacterKind kindForIndex(int kind) const;
    bool hasPlayableState() const;

    Difficulty m_difficulty;
    int m_coins;
    int m_score;
    int m_movesLeft;
    QLabel *m_titleLabel;
    QLabel *m_metaLabel;
    QLabel *m_goalLabel;
    QLabel *m_hintLabel;
    QGridLayout *m_boardLayout;
    QVector<QPushButton *> m_boardCells;
    QVector<int> m_boardKinds;
    QVector<int> m_clearedCounts;
    QPoint m_selectedCell;
    QString m_statusMessage;
    bool m_roundFinished;
};
