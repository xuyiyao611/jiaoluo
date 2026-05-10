#pragma once

#include <QWidget>

class QLabel;
class QPushButton;

class LandingPage : public QWidget
{
    Q_OBJECT

public:
    explicit LandingPage(QWidget *parent = nullptr);

signals:
    void startRequested();
    void newGameRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void buildUi();

    QLabel *m_titleLabel;
    QPushButton *m_startButton;
    QPushButton *m_newGameButton;
};
