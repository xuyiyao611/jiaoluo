#pragma once

#include <QFrame>
#include <QWidget>

#include "app/app_state.h"

class DifficultySelectPage : public QWidget
{
    Q_OBJECT

public:
    explicit DifficultySelectPage(QWidget *parent = nullptr);

    void setSelectedDifficulty(Difficulty difficulty);

signals:
    void backRequested();
    void difficultyChosen(Difficulty difficulty);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void updateCardStyles();

    QFrame *m_easyCard;
    QFrame *m_hardCard;
    Difficulty m_selectedDifficulty;
};
