#pragma once

#include <QVector>
#include <QWidget>

#include "app/app_state.h"

class QLabel;
class QGridLayout;

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);

    void setCoins(int coins);
    void setLastRoundSummary(const QString &summary);
    void setCharacterProgress(const QVector<CharacterProgress> &characters);

signals:
    void startRequested();
    void newGameRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *m_coinLabel;
    QLabel *m_lastRoundLabel;
    QGridLayout *m_characterGrid;
};
