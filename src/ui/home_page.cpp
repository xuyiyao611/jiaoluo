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
      m_characterGrid(new QGridLayout()),
      m_shopGrid(new QGridLayout())
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
        QStringLiteral("挑战成功后收集角色碎片，再用金币购买食物，喂给喜欢的角色，累积好感度并完成突破。"),
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

    auto *infoLabel = new QLabel(
        QStringLiteral("Qt 迁移阶段：已接通三消、结算、碎片、角色收集、商店、喂食和突破主流程。"),
        rightCard);
    infoLabel->setWordWrap(true);
    infoLabel->setObjectName(QStringLiteral("statusHeadline"));

    rightLayout->addWidget(m_coinLabel, 0, Qt::AlignLeft);
    rightLayout->addWidget(infoLabel);
    rightLayout->addWidget(m_lastRoundLabel);
    rightLayout->addStretch();

    heroLayout->addWidget(leftCard, 3);
    heroLayout->addWidget(rightCard, 2);
    rootLayout->addLayout(heroLayout);

    auto *shopCard = new QFrame(this);
    shopCard->setObjectName(QStringLiteral("featureCard"));
    auto *shopLayout = new QVBoxLayout(shopCard);
    shopLayout->setContentsMargins(20, 18, 20, 18);
    shopLayout->setSpacing(10);
    auto *shopTitle = new QLabel(QStringLiteral("暖心商店"), shopCard);
    shopTitle->setObjectName(QStringLiteral("featureTitle"));
    m_shopGrid->setHorizontalSpacing(14);
    m_shopGrid->setVerticalSpacing(14);
    shopLayout->addWidget(shopTitle);
    shopLayout->addLayout(m_shopGrid);

    auto *characterCard = new QFrame(this);
    characterCard->setObjectName(QStringLiteral("featureCard"));
    auto *characterLayout = new QVBoxLayout(characterCard);
    characterLayout->setContentsMargins(20, 18, 20, 18);
    characterLayout->setSpacing(10);
    auto *characterTitle = new QLabel(QStringLiteral("角色图鉴"), characterCard);
    characterTitle->setObjectName(QStringLiteral("featureTitle"));
    m_characterGrid->setHorizontalSpacing(12);
    m_characterGrid->setVerticalSpacing(12);
    characterLayout->addWidget(characterTitle);
    characterLayout->addLayout(m_characterGrid);

    rootLayout->addWidget(shopCard);
    rootLayout->addWidget(characterCard);
    rootLayout->addStretch();

    connect(startButton, &QPushButton::clicked, this, &HomePage::startRequested);
    connect(newGameButton, &QPushButton::clicked, this, &HomePage::newGameRequested);

    setStyleSheet(QStringLiteral(
        "#homeHeroCard, #homeStatusCard, #featureCard, #shopCard, #characterCard {"
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
        "#homeDesc, #statusHeadline, #characterMeta, #foodMeta {"
        "  color: #887a73;"
        "  font-size: 15px;"
        "  line-height: 1.6;"
        "}"
        "#primaryButton, #secondaryButton, #ghostButton {"
        "  min-height: 44px;"
        "  padding: 10px 18px;"
        "  border: none;"
        "  border-radius: 18px;"
        "  font-size: 15px;"
        "  font-weight: 700;"
        "}"
        "#primaryButton {"
        "  background: #edab84;"
        "  color: white;"
        "}"
        "#secondaryButton, #ghostButton {"
        "  background: #f5eadf;"
        "  color: #6d6862;"
        "}"
        "#coinPill {"
        "  padding: 8px 16px;"
        "  border-radius: 20px;"
        "  background: #ffe4af;"
        "  color: #8c5b1d;"
        "  font-size: 16px;"
        "  font-weight: 800;"
        "}"
        "#statusHeadline, #featureTitle {"
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
        "#itemCard {"
        "  background: rgba(255, 255, 255, 208);"
        "  border: 1px solid rgba(195, 181, 169, 95);"
        "  border-radius: 22px;"
        "}"
        "#itemTitle {"
        "  color: #716662;"
        "  font-size: 18px;"
        "  font-weight: 700;"
        "}"
        "#itemSymbol {"
        "  color: #8b7264;"
        "  font-size: 22px;"
        "  font-weight: 800;"
        "}"
    ));
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
    m_characters = characters;

    while (QLayoutItem *item = m_characterGrid->takeAt(0)) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    for (int index = 0; index < m_characters.size(); ++index) {
        const CharacterProgress &progress = m_characters.at(index);

        auto *card = new QFrame(this);
        card->setObjectName(QStringLiteral("itemCard"));
        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(14, 14, 14, 14);
        cardLayout->setSpacing(6);

        auto *symbolLabel = new QLabel(characterSymbol(progress.kind), card);
        symbolLabel->setObjectName(QStringLiteral("itemSymbol"));

        auto *nameLabel = new QLabel(characterName(progress.kind), card);
        nameLabel->setObjectName(QStringLiteral("itemTitle"));

        auto *fragmentLabel = new QLabel(QStringLiteral("碎片：%1 / 200").arg(progress.fragments), card);
        fragmentLabel->setObjectName(QStringLiteral("characterMeta"));

        auto *affectionLabel = new QLabel(
            progress.affection >= 20
                ? QStringLiteral("好感度已满")
                : QStringLiteral("好感度：%1 / 20").arg(progress.affection),
            card);
        affectionLabel->setObjectName(QStringLiteral("characterMeta"));

        auto *favoriteLabel = new QLabel(
            QStringLiteral("偏好食物：%1").arg(foodName(favoriteFoodForCharacter(progress.kind))),
            card);
        favoriteLabel->setObjectName(QStringLiteral("characterMeta"));

        auto *descLabel = new QLabel(characterDescription(progress.kind), card);
        descLabel->setWordWrap(true);
        descLabel->setObjectName(QStringLiteral("characterMeta"));

        cardLayout->addWidget(symbolLabel);
        cardLayout->addWidget(nameLabel);
        cardLayout->addWidget(fragmentLabel);
        cardLayout->addWidget(affectionLabel);
        cardLayout->addWidget(favoriteLabel);
        cardLayout->addWidget(descLabel);

        if (progress.unlocked) {
            for (const FoodKind food : allFoodKinds()) {
                auto *feedButton = new QPushButton(
                    QStringLiteral("喂%1 (%2)").arg(foodName(food)).arg(foodCount(food)),
                    card);
                feedButton->setObjectName(QStringLiteral("ghostButton"));
                feedButton->setEnabled(foodCount(food) > 0);
                connect(feedButton, &QPushButton::clicked, this, [this, progress, food]() {
                    emit feedCharacterRequested(progress.kind, food);
                });
                cardLayout->addWidget(feedButton);
            }

            if (!progress.brokenThrough && progress.affection >= 20) {
                auto *breakthroughButton = new QPushButton(QStringLiteral("突破"), card);
                breakthroughButton->setObjectName(QStringLiteral("primaryButton"));
                connect(breakthroughButton, &QPushButton::clicked, this, [this, progress]() {
                    emit breakthroughRequested(progress.kind);
                });
                cardLayout->addWidget(breakthroughButton);
            } else if (progress.brokenThrough) {
                auto *statusLabel = new QLabel(QStringLiteral("状态：已突破"), card);
                statusLabel->setObjectName(QStringLiteral("characterMeta"));
                cardLayout->addWidget(statusLabel);
            }
        } else {
            auto *lockLabel = new QLabel(QStringLiteral("状态：未兑换"), card);
            lockLabel->setObjectName(QStringLiteral("characterMeta"));
            cardLayout->addWidget(lockLabel);
        }

        m_characterGrid->addWidget(card, index / 4, index % 4);
    }
}

void HomePage::setFoodInventory(const QVector<FoodInventory> &foods)
{
    m_foods = foods;

    while (QLayoutItem *item = m_shopGrid->takeAt(0)) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    const QVector<FoodKind> kinds = allFoodKinds();
    for (int index = 0; index < kinds.size(); ++index) {
        const FoodKind kind = kinds.at(index);

        auto *card = new QFrame(this);
        card->setObjectName(QStringLiteral("itemCard"));
        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(14, 14, 14, 14);
        cardLayout->setSpacing(6);

        auto *symbolLabel = new QLabel(foodName(kind).left(1), card);
        symbolLabel->setObjectName(QStringLiteral("itemSymbol"));

        auto *nameLabel = new QLabel(foodName(kind), card);
        nameLabel->setObjectName(QStringLiteral("itemTitle"));

        auto *priceLabel = new QLabel(QStringLiteral("价格：%1 金币").arg(foodPrice(kind)), card);
        priceLabel->setObjectName(QStringLiteral("foodMeta"));

        auto *countLabel = new QLabel(QStringLiteral("库存：%1").arg(foodCount(kind)), card);
        countLabel->setObjectName(QStringLiteral("foodMeta"));

        auto *buyButton = new QPushButton(QStringLiteral("购买"), card);
        buyButton->setObjectName(QStringLiteral("secondaryButton"));
        connect(buyButton, &QPushButton::clicked, this, [this, kind]() {
            emit buyFoodRequested(kind);
        });

        cardLayout->addWidget(symbolLabel);
        cardLayout->addWidget(nameLabel);
        cardLayout->addWidget(priceLabel);
        cardLayout->addWidget(countLabel);
        cardLayout->addWidget(buyButton);

        m_shopGrid->addWidget(card, 0, index);
    }
}

int HomePage::foodCount(FoodKind kind) const
{
    for (const FoodInventory &food : m_foods) {
        if (food.kind == kind) {
            return food.count;
        }
    }
    return 0;
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
