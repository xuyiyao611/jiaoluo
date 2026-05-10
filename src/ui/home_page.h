#pragma once

#include <QWidget>

class QLabel;

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};
