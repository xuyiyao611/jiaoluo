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
    void setFoodInventory(const QVector<FoodInventory> &foods);
    void setActionMessage(const QString &message);

signals:
    void startRequested();
    void newGameRequested();
    void buyFoodRequested(FoodKind kind);
    void feedCharacterRequested(CharacterKind kind, FoodKind food);
    void breakthroughRequested(CharacterKind kind);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int foodCount(FoodKind kind) const;

    QLabel *m_coinLabel;
    QLabel *m_lastRoundLabel;
    QLabel *m_actionMessageLabel;
    QGridLayout *m_characterGrid;
    QGridLayout *m_shopGrid;
    QVector<CharacterProgress> m_characters;
    QVector<FoodInventory> m_foods;
};
