#pragma once

#include <QWidget>

class QLabel;

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);

    void setCoins(int coins);

signals:
    void startRequested();
    void newGameRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *m_coinLabel;
};
