#pragma once

#include <QDialog>

#include "ui/game_page.h"

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(const Match3RoundResult &result, QWidget *parent = nullptr);
};
