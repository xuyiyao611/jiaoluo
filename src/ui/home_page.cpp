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
      m_coinLabel(new QLabel(this)),
      m_lastRoundLabel(new QLabel(this)),
      m_characterGrid(new QGridLayout())
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
        QStringLiteral("挑战成功后会把本局消除数量转成对应角色碎片。累计到 200 碎片即可视为兑换角色，超出的碎片每 20 个自动转成 1 金币。"),
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

    m_lastRoundLabel->setObjectName(QStringLiteral("lastRoundLabel"));
    m_lastRoundLabel->setWordWrap(true);
    setLastRoundSummary(QStringLiteral("最近一局：还没有结算记录，先开始一局吧。"));

    auto *infoLabel = new QLabel(QStringLiteral("Qt 迁移阶段：已接通主页、难度选择、三消结算、碎片累计与角色兑换展示。"), rightCard);
    infoLabel->setWordWrap(true);
    infoLabel->setObjectName(QStringLiteral("statusHeadline"));

    auto *collectionTitle = new QLabel(QStringLiteral("角色收集"), rightCard);
    collectionTitle->setObjectName(QStringLiteral("sectionTitle"));

    m_characterGrid->setHorizontalSpacing(12);
    m_characterGrid->setVerticalSpacing(12);

    rightLayout->addWidget(m_coinLabel, 0, Qt::AlignLeft);
    rightLayout->addWidget(infoLabel);
    rightLayout->addWidget(m_lastRoundLabel);
    rightLayout->addWidget(collectionTitle);
    rightLayout->addLayout(m_characterGrid);
    rightLayout->addStretch();

    heroLayout->addWidget(leftCard, 3);
    heroLayout->addWidget(rightCard, 2);
    rootLayout->addLayout(heroLayout);

    auto *featureGrid = new QGridLayout();
    featureGrid->setHorizontalSpacing(16);
    featureGrid->setVerticalSpacing(16);

    const QStringList titles = {
        QStringLiteral("当前已完成"),
        QStringLiteral("奖励规则"),
        QStringLiteral("下一步迁移")
    };
    const QStringList descriptions = {
        QStringLiteral("9 x 9 棋盘、6 元素池、真实交换、三连消除、补位、局内结算。"),
        QStringLiteral("只有挑战成功后才发金币和碎片；每消除 1 个元素就获得 1 个对应碎片。"),
        QStringLiteral("后续继续迁移商店、喂食、好感度、突破与角色插图。")
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
        "#homeDesc, #featureDesc, #statusHeadline {"
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
        "#statusHeadline, #sectionTitle {"
        "  color: #716662;"
        "  font-size: 17px;"
        "  font-weight: 700;"
        "}"
        "#lastRoundLabel {"
        "  padding: 12px 14px;"
        "  border-radius: 18px;"
        "  background: rgba(255, 243, 226, 210);"
        "  color: #7a665c;"
        "  font-size: 14px;"
        "  font-weight: 600;"
        "}"
        "#statusItemCard, #featureCard, #characterCard {"
        "  background: rgba(255, 255, 255, 208);"
        "  border: 1px solid rgba(195, 181, 169, 95);"
        "  border-radius: 22px;"
        "}"
        "#featureTitle, #characterName {"
        "  color: #716662;"
        "  font-size: 18px;"
        "  font-weight: 700;"
        "}"
        "#characterSymbol {"
        "  color: #8b7264;"
        "  font-size: 22px;"
        "  font-weight: 800;"
        "}"
        "#characterMeta {"
        "  color: #887a73;"
        "  font-size: 13px;"
        "}"
        "#characterLock {"
        "  color: #b89f92;"
        "  font-size: 13px;"
        "  font-weight: 700;"
        "}"
    ));

    setCharacterProgress(createInitialCharacterProgress());
}

void HomePage::setCoins(int coins)
{
    m_coinLabel->setText(QStringLiteral("当前金币：%1").arg(coins));
}

void HomePage::setLastRoundSummary(const QString &summary)
{
    m_lastRoundLabel->setText(summary);
}

void HomePage::setCharacterProgress(const QVector<CharacterProgress> &characters)
{
    while (QLayoutItem *item = m_characterGrid->takeAt(0)) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    for (int index = 0; index < characters.size(); ++index) {
        const CharacterProgress &progress = characters.at(index);

        auto *card = new QFrame(this);
        card->setObjectName(QStringLiteral("characterCard"));
        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(14, 14, 14, 14);
        cardLayout->setSpacing(6);

        auto *symbolLabel = new QLabel(characterSymbol(progress.kind), card);
        symbolLabel->setObjectName(QStringLiteral("characterSymbol"));

        auto *nameLabel = new QLabel(characterName(progress.kind), card);
        nameLabel->setObjectName(QStringLiteral("characterName"));

        auto *fragmentLabel = new QLabel(
            QStringLiteral("碎片：%1 / 200").arg(progress.fragments),
            card);
        fragmentLabel->setObjectName(QStringLiteral("characterMeta"));

        auto *statusLabel = new QLabel(
            progress.unlocked ? QStringLiteral("已兑换") : QStringLiteral("未兑换"),
            card);
        statusLabel->setObjectName(progress.unlocked ? QStringLiteral("characterMeta") : QStringLiteral("characterLock"));

        cardLayout->addWidget(symbolLabel);
        cardLayout->addWidget(nameLabel);
        cardLayout->addWidget(fragmentLabel);
        cardLayout->addWidget(statusLabel);

        m_characterGrid->addWidget(card, index / 4, index % 4);
    }
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
