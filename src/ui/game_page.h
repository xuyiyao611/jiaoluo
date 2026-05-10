#pragma once

#include <QVector>
#include <QWidget>

#include "app/app_state.h"

class QLabel;
class QGridLayout;

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr);

    void setDifficulty(Difficulty difficulty);
    void setCoins(int coins);

signals:
    void backToHomeRequested();
    void backToDifficultyRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void rebuildBoard();
    QString tileSymbol(int index) const;

    Difficulty m_difficulty;
    int m_coins;
    QLabel *m_titleLabel;
    QLabel *m_metaLabel;
    QLabel *m_goalLabel;
    QLabel *m_hintLabel;
    QGridLayout *m_boardLayout;
    QVector<QWidget *> m_boardCells;
};
