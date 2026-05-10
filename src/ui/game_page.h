#pragma once

#include <QPoint>
#include <QVector>
#include <QWidget>

#include "app/app_state.h"

class QLabel;
class QGridLayout;
class QPushButton;

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
    int boardIndex(int row, int col) const;
    int randomKind() const;
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
    QPoint m_selectedCell;
    QString m_statusMessage;
};
