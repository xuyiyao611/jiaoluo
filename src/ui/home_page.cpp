#include "home_page.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLinearGradient>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent),
      m_coinLabel(new QLabel(this))
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(36, 28, 36, 28);
    rootLayout->setSpacing(20);

    auto *heroLayout = new QHBoxLayout();
    heroLayout->setSpacing(18);

    auto *leftCard = new QFrame(this);
    leftCard->setObjectName(QStringLiteral("homeHeroCard"));
    auto *leftLayout = new QVBoxLayout(leftCard);
    leftLayout->setContentsMargins(28, 26, 28, 26);
    leftLayout->setSpacing(12);

    auto *badgeLabel = new QLabel(QStringLiteral("治愈收集 / 三消养成"), leftCard);
    badgeLabel->setObjectName(QStringLiteral("homeBadge"));

    auto *titleLabel = new QLabel(QStringLiteral("角落消消"), leftCard);
    titleLabel->setObjectName(QStringLiteral("homeTitle"));

    auto *subtitleLabel = new QLabel(QStringLiteral("今天也陪它们慢慢长大"), leftCard);
    subtitleLabel->setObjectName(QStringLiteral("homeSubtitle"));

    auto *descLabel = new QLabel(
        QStringLiteral("挑战成功后领取金币与碎片奖励，喂喜欢的零食、累积好感度，再逐步迁移完整玩法到 Qt / C++ 版本。"),
        leftCard);
    descLabel->setWordWrap(true);
    descLabel->setObjectName(QStringLiteral("homeDesc"));

    auto *buttonRow = new QHBoxLayout();
    buttonRow->setSpacing(12);

    auto *startButton = new QPushButton(QStringLiteral("开始游戏"), leftCard);
    startButton->setObjectName(QStringLiteral("primaryButton"));
    startButton->setCursor(Qt::PointingHandCursor);

    auto *newGameButton = new QPushButton(QStringLiteral("开新游戏"), leftCard);
    newGameButton->setObjectName(QStringLiteral("secondaryButton"));
    newGameButton->setCursor(Qt::PointingHandCursor);

    buttonRow->addWidget(startButton);
    buttonRow->addWidget(newGameButton);
    buttonRow->addStretch();

    leftLayout->addWidget(badgeLabel, 0, Qt::AlignLeft);
    leftLayout->addWidget(titleLabel);
    leftLayout->addWidget(subtitleLabel);
    leftLayout->addWidget(descLabel);
    leftLayout->addLayout(buttonRow);
    leftLayout->addStretch();

    auto *rightCard = new QFrame(this);
    rightCard->setObjectName(QStringLiteral("homeStatusCard"));
    auto *rightLayout = new QVBoxLayout(rightCard);
    rightLayout->setContentsMargins(24, 24, 24, 24);
    rightLayout->setSpacing(14);

    m_coinLabel->setObjectName(QStringLiteral("coinPill"));
    setCoins(10);

    auto *infoLabel = new QLabel(QStringLiteral("Qt 迁移第二步：主页、难度页和基础状态流转"), rightCard);
    infoLabel->setWordWrap(true);
    infoLabel->setObjectName(QStringLiteral("statusHeadline"));

    auto *grid = new QGridLayout();
    grid->setHorizontalSpacing(12);
    grid->setVerticalSpacing(12);

    const QStringList items = {
        QStringLiteral("启动页已迁移"),
        QStringLiteral("主页已迁移"),
        QStringLiteral("难度页待进入"),
        QStringLiteral("三消玩法待迁移")
    };

    for (int index = 0; index < items.size(); ++index) {
        auto *card = new QFrame(rightCard);
        card->setObjectName(QStringLiteral("statusItemCard"));

        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(16, 14, 16, 14);
        cardLayout->setSpacing(4);

        auto *indexLabel = new QLabel(QStringLiteral("S%1").arg(index + 1), card);
        indexLabel->setObjectName(QStringLiteral("statusItemIndex"));

        auto *textLabel = new QLabel(items.at(index), card);
        textLabel->setWordWrap(true);
        textLabel->setObjectName(QStringLiteral("statusItemText"));

        cardLayout->addWidget(indexLabel);
        cardLayout->addWidget(textLabel);
        grid->addWidget(card, index / 2, index % 2);
    }

    rightLayout->addWidget(m_coinLabel, 0, Qt::AlignLeft);
    rightLayout->addWidget(infoLabel);
    rightLayout->addLayout(grid);
    rightLayout->addStretch();

    heroLayout->addWidget(leftCard, 3);
    heroLayout->addWidget(rightCard, 2);
    rootLayout->addLayout(heroLayout);

    auto *featureGrid = new QGridLayout();
    featureGrid->setHorizontalSpacing(16);
    featureGrid->setVerticalSpacing(16);

    const QStringList titles = {
        QStringLiteral("安静的小棋盘"),
        QStringLiteral("赢了才有礼物"),
        QStringLiteral("喂食与突破")
    };
    const QStringList descriptions = {
        QStringLiteral("9 x 9 棋盘配合 6 类元素，后续会在 Qt 版逐步恢复完整三消体验。"),
        QStringLiteral("只有挑战成功后才会结算金币和碎片奖励，保留原项目的核心规则。"),
        QStringLiteral("角色碎片、商店、好感度与突破系统会在后续阶段按模块迁移。")
    };

    for (int index = 0; index < titles.size(); ++index) {
        auto *card = new QFrame(this);
        card->setObjectName(QStringLiteral("featureCard"));
        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(20, 18, 20, 18);
        cardLayout->setSpacing(8);

        auto *title = new QLabel(titles.at(index), card);
        title->setObjectName(QStringLiteral("featureTitle"));

        auto *desc = new QLabel(descriptions.at(index), card);
        desc->setObjectName(QStringLiteral("featureDesc"));
        desc->setWordWrap(true);

        cardLayout->addWidget(title);
        cardLayout->addWidget(desc);
        featureGrid->addWidget(card, 0, index);
    }

    rootLayout->addLayout(featureGrid);
    rootLayout->addStretch();

    connect(startButton, &QPushButton::clicked, this, &HomePage::startRequested);
    connect(newGameButton, &QPushButton::clicked, this, &HomePage::newGameRequested);

    setStyleSheet(QStringLiteral(
        "#homeHeroCard, #homeStatusCard {"
        "  background: rgba(255, 251, 246, 225);"
        "  border: 1px solid rgba(190, 174, 161, 110);"
        "  border-radius: 28px;"
        "}"
        "#homeBadge {"
        "  color: #b98a72;"
        "  font-size: 13px;"
        "  font-weight: 700;"
        "  letter-spacing: 2px;"
        "}"
        "#homeTitle {"
        "  color: #6b605c;"
        "  font-size: 40px;"
        "  font-weight: 800;"
        "}"
        "#homeSubtitle {"
        "  color: #7f6e67;"
        "  font-size: 28px;"
        "  font-weight: 700;"
        "}"
        "#homeDesc, #featureDesc, #statusHeadline, #statusItemText {"
        "  color: #887a73;"
        "  font-size: 15px;"
        "  line-height: 1.6;"
        "}"
        "#primaryButton {"
        "  min-height: 48px;"
        "  padding: 10px 22px;"
        "  border: none;"
        "  border-radius: 20px;"
        "  background: #edab84;"
        "  color: white;"
        "  font-size: 16px;"
        "  font-weight: 700;"
        "}"
        "#secondaryButton {"
        "  min-height: 48px;"
        "  padding: 10px 22px;"
        "  border: none;"
        "  border-radius: 20px;"
        "  background: #f5eadf;"
        "  color: #6d6862;"
        "  font-size: 16px;"
        "  font-weight: 700;"
        "}"
        "#coinPill {"
        "  padding: 8px 16px;"
        "  border-radius: 20px;"
        "  background: #ffe4af;"
        "  color: #8c5b1d;"
        "  font-size: 16px;"
        "  font-weight: 800;"
        "}"
        "#statusHeadline {"
        "  color: #716662;"
        "  font-size: 17px;"
        "  font-weight: 700;"
        "}"
        "#statusItemCard, #featureCard {"
        "  background: rgba(255, 255, 255, 208);"
        "  border: 1px solid rgba(195, 181, 169, 95);"
        "  border-radius: 22px;"
        "}"
        "#statusItemIndex {"
        "  color: #b88c77;"
        "  font-size: 12px;"
        "  font-weight: 700;"
        "}"
        "#statusItemText, #featureTitle {"
        "  color: #716662;"
        "  font-size: 18px;"
        "  font-weight: 700;"
        "}"
        "#featureTitle {"
        "  color: #6f615b;"
        "}"
    ));
}

void HomePage::setCoins(int coins)
{
    m_coinLabel->setText(QStringLiteral("当前金币：%1").arg(coins));
}

void HomePage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient backgroundGradient(rect().topLeft(), rect().bottomLeft());
    backgroundGradient.setColorAt(0.0, QColor(251, 247, 240));
    backgroundGradient.setColorAt(1.0, QColor(246, 241, 231));
    painter.fillRect(rect(), backgroundGradient);

    painter.setOpacity(0.24);
    painter.drawPixmap(rect(), QPixmap(QStringLiteral(":/images/page-background.jpg")));
}
