#include "home_page.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLinearGradient>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

namespace {

constexpr int kCharacterColumns = 2;
constexpr int kShopColumns = 3;

QPixmap scaledPixmap(const QString &path, int width, int height)
{
    return QPixmap(path).scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

} // namespace

HomePage::HomePage(QWidget *parent)
    : QWidget(parent),
      m_coinLabel(new QLabel(this)),
      m_lastRoundLabel(new QLabel(this)),
      m_actionMessageLabel(new QLabel(this)),
      m_characterGrid(new QGridLayout()),
      m_shopGrid(new QGridLayout())
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->viewport()->setAutoFillBackground(false);
    rootLayout->addWidget(scrollArea);

    auto *contentWidget = new QWidget(scrollArea);
    contentWidget->setObjectName(QStringLiteral("homeScrollContent"));
    scrollArea->setWidget(contentWidget);

    auto *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(48, 34, 48, 32);
    contentLayout->setSpacing(24);

    auto *heroLayout = new QHBoxLayout();
    heroLayout->setSpacing(22);

    auto *leftCard = new QFrame(contentWidget);
    leftCard->setObjectName(QStringLiteral("homeHeroCard"));
    auto *leftLayout = new QVBoxLayout(leftCard);
    leftLayout->setContentsMargins(40, 36, 40, 36);
    leftLayout->setSpacing(18);

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
    buttonRow->setSpacing(14);

    auto *startButton = new QPushButton(QStringLiteral("开始游戏"), leftCard);
    startButton->setObjectName(QStringLiteral("primaryButton"));
    startButton->setCursor(Qt::PointingHandCursor);

    auto *newGameButton = new QPushButton(QStringLiteral("开新游戏"), leftCard);
    newGameButton->setObjectName(QStringLiteral("secondaryButton"));
    newGameButton->setCursor(Qt::PointingHandCursor);

    buttonRow->addWidget(startButton, 0);
    buttonRow->addWidget(newGameButton, 0);
    buttonRow->addStretch();

    leftLayout->addWidget(badgeLabel, 0, Qt::AlignLeft);
    leftLayout->addWidget(titleLabel);
    leftLayout->addWidget(subtitleLabel);
    leftLayout->addSpacing(2);
    leftLayout->addWidget(descLabel);
    leftLayout->addSpacing(8);
    leftLayout->addLayout(buttonRow);
    leftLayout->addStretch();

    auto *rightCard = new QFrame(contentWidget);
    rightCard->setObjectName(QStringLiteral("homeStatusCard"));
    auto *rightLayout = new QVBoxLayout(rightCard);
    rightLayout->setContentsMargins(30, 28, 30, 28);
    rightLayout->setSpacing(14);

    m_coinLabel->setObjectName(QStringLiteral("coinPill"));
    setCoins(10);

    m_lastRoundLabel->setObjectName(QStringLiteral("lastRoundLabel"));
    m_lastRoundLabel->setWordWrap(true);
    setLastRoundSummary(QStringLiteral("最近一局：还没有结算记录，先开始一局吧。"));

    m_actionMessageLabel->setObjectName(QStringLiteral("actionMessage"));
    m_actionMessageLabel->setWordWrap(true);
    setActionMessage(QStringLiteral("当前可在主页购买食物、喂食角色，并在好感度满 20 时执行突破。"));

    auto *statusTitle = new QLabel(QStringLiteral("旅程进度"), rightCard);
    statusTitle->setObjectName(QStringLiteral("featureTitle"));

    auto *infoLabel = new QLabel(
        QStringLiteral("Qt 迁移阶段：已接通三消、结算、碎片、角色收集、商店、喂食和突破主流程。"),
        rightCard);
    infoLabel->setWordWrap(true);
    infoLabel->setObjectName(QStringLiteral("statusText"));

    rightLayout->addWidget(m_coinLabel, 0, Qt::AlignLeft);
    rightLayout->addWidget(statusTitle);
    rightLayout->addWidget(infoLabel);
    rightLayout->addWidget(m_lastRoundLabel);
    rightLayout->addWidget(m_actionMessageLabel);
    rightLayout->addStretch();

    heroLayout->addWidget(leftCard, 5);
    heroLayout->addWidget(rightCard, 4);
    contentLayout->addLayout(heroLayout);

    auto *shopCard = new QFrame(contentWidget);
    shopCard->setObjectName(QStringLiteral("featureCard"));
    auto *shopLayout = new QVBoxLayout(shopCard);
    shopLayout->setContentsMargins(28, 24, 28, 24);
    shopLayout->setSpacing(16);

    auto *shopTitle = new QLabel(QStringLiteral("暖心商店"), shopCard);
    shopTitle->setObjectName(QStringLiteral("featureTitle"));
    m_shopGrid->setHorizontalSpacing(18);
    m_shopGrid->setVerticalSpacing(18);
    m_shopGrid->setContentsMargins(0, 4, 0, 0);
    shopLayout->addWidget(shopTitle);
    shopLayout->addLayout(m_shopGrid);

    auto *characterCard = new QFrame(contentWidget);
    characterCard->setObjectName(QStringLiteral("featureCard"));
    auto *characterLayout = new QVBoxLayout(characterCard);
    characterLayout->setContentsMargins(28, 24, 28, 24);
    characterLayout->setSpacing(16);

    auto *characterTitle = new QLabel(QStringLiteral("角色图鉴"), characterCard);
    characterTitle->setObjectName(QStringLiteral("featureTitle"));
    m_characterGrid->setHorizontalSpacing(18);
    m_characterGrid->setVerticalSpacing(18);
    m_characterGrid->setContentsMargins(0, 4, 0, 0);
    characterLayout->addWidget(characterTitle);
    characterLayout->addLayout(m_characterGrid);

    contentLayout->addWidget(shopCard);
    contentLayout->addWidget(characterCard);
    contentLayout->addStretch();

    connect(startButton, &QPushButton::clicked, this, &HomePage::startRequested);
    connect(newGameButton, &QPushButton::clicked, this, &HomePage::newGameRequested);

    setStyleSheet(QStringLiteral(
        "QScrollArea {"
        "  background: transparent;"
        "  border: none;"
        "}"
        "QScrollArea > QWidget > QWidget, #homeScrollContent {"
        "  background: transparent;"
        "}"
        "#homeHeroCard, #homeStatusCard, #featureCard {"
        "  background: rgba(255, 251, 246, 232);"
        "  border: 1px solid rgba(190, 174, 161, 96);"
        "  border-radius: 32px;"
        "}"
        "#homeBadge {"
        "  color: #b98a72;"
        "  font-size: 12px;"
        "  font-weight: 700;"
        "  letter-spacing: 4px;"
        "}"
        "#homeTitle {"
        "  color: #6b605c;"
        "  font-size: 42px;"
        "  font-weight: 800;"
        "}"
        "#homeSubtitle {"
        "  color: #7f6e67;"
        "  font-size: 22px;"
        "  font-weight: 700;"
        "}"
        "#homeDesc, #statusText {"
        "  color: #887a73;"
        "  font-size: 14px;"
        "  line-height: 1.75;"
        "}"
        "#characterMeta, #foodMeta {"
        "  color: #887a73;"
        "  font-size: 13px;"
        "  line-height: 1.65;"
        "}"
        "#primaryButton, #secondaryButton, #ghostButton {"
        "  min-height: 48px;"
        "  padding: 10px 22px;"
        "  border: none;"
        "  border-radius: 18px;"
        "  font-size: 15px;"
        "  font-weight: 700;"
        "}"
        "#miniButton {"
        "  min-height: 38px;"
        "  padding: 8px 10px;"
        "  border: none;"
        "  border-radius: 15px;"
        "  background: #f5eadf;"
        "  color: #6d6862;"
        "  font-size: 13px;"
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
        "#primaryButton:disabled, #secondaryButton:disabled, #ghostButton:disabled, #miniButton:disabled {"
        "  background: #e6ddd4;"
        "  color: #a79a90;"
        "}"
        "#coinPill {"
        "  padding: 10px 18px;"
        "  border-radius: 22px;"
        "  background: #ffe4af;"
        "  color: #8c5b1d;"
        "  font-size: 16px;"
        "  font-weight: 800;"
        "}"
        "#featureTitle {"
        "  color: #716662;"
        "  font-size: 22px;"
        "  font-weight: 700;"
        "}"
        "#lastRoundLabel, #actionMessage {"
        "  padding: 14px 16px;"
        "  border-radius: 18px;"
        "  font-size: 13px;"
        "  line-height: 1.65;"
        "  font-weight: 600;"
        "}"
        "#lastRoundLabel {"
        "  background: rgba(255, 243, 226, 210);"
        "  color: #7a665c;"
        "}"
        "#actionMessage {"
        "  background: rgba(240, 247, 239, 220);"
        "  color: #63735f;"
        "}"
        "#itemCard {"
        "  background: rgba(255, 255, 255, 214);"
        "  border: 1px solid rgba(195, 181, 169, 85);"
        "  border-radius: 26px;"
        "}"
        "#itemTitle {"
        "  color: #716662;"
        "  font-size: 18px;"
        "  font-weight: 700;"
        "}"
        "#itemSymbol {"
        "  color: #8b7264;"
        "  font-size: 26px;"
        "  font-weight: 800;"
        "}"
        "#statusTag {"
        "  padding: 8px 12px;"
        "  border-radius: 14px;"
        "  background: rgba(246, 236, 224, 220);"
        "  color: #7a665c;"
        "  font-size: 13px;"
        "  font-weight: 700;"
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

void HomePage::setActionMessage(const QString &message)
{
    m_actionMessageLabel->setText(message);
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
        card->setMinimumWidth(340);

        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(20, 20, 20, 20);
        cardLayout->setSpacing(10);

        auto *portraitLabel = new QLabel(card);
        portraitLabel->setAlignment(Qt::AlignCenter);
        portraitLabel->setPixmap(scaledPixmap(characterImagePath(progress.kind, progress.brokenThrough), 108, 108));
        portraitLabel->setMinimumHeight(110);
        if (!progress.unlocked) {
            auto *effect = new QGraphicsOpacityEffect(portraitLabel);
            effect->setOpacity(0.32);
            portraitLabel->setGraphicsEffect(effect);
        }

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

        cardLayout->addWidget(portraitLabel);
        cardLayout->addWidget(nameLabel);
        cardLayout->addWidget(fragmentLabel);
        cardLayout->addWidget(affectionLabel);
        cardLayout->addWidget(favoriteLabel);
        cardLayout->addWidget(descLabel);
        cardLayout->addStretch();

        if (progress.unlocked) {
            auto *feedRow = new QHBoxLayout();
            feedRow->setSpacing(8);

            for (const FoodKind food : allFoodKinds()) {
                auto *feedButton = new QPushButton(
                    QStringLiteral("喂%1(%2)").arg(foodName(food)).arg(foodCount(food)),
                    card);
                feedButton->setObjectName(QStringLiteral("miniButton"));
                feedButton->setEnabled(foodCount(food) > 0);
                connect(feedButton, &QPushButton::clicked, this, [this, progress, food]() {
                    emit feedCharacterRequested(progress.kind, food);
                });
                feedRow->addWidget(feedButton, 1);
            }

            cardLayout->addLayout(feedRow);

            if (!progress.brokenThrough && progress.affection >= 20) {
                auto *breakthroughButton = new QPushButton(QStringLiteral("突破"), card);
                breakthroughButton->setObjectName(QStringLiteral("primaryButton"));
                connect(breakthroughButton, &QPushButton::clicked, this, [this, progress]() {
                    emit breakthroughRequested(progress.kind);
                });
                cardLayout->addWidget(breakthroughButton);
            } else if (progress.brokenThrough) {
                auto *statusLabel = new QLabel(QStringLiteral("状态：已突破"), card);
                statusLabel->setObjectName(QStringLiteral("statusTag"));
                cardLayout->addWidget(statusLabel);
            }
        } else {
            auto *lockLabel = new QLabel(QStringLiteral("状态：未兑换"), card);
            lockLabel->setObjectName(QStringLiteral("statusTag"));
            cardLayout->addWidget(lockLabel);
        }

        m_characterGrid->addWidget(card, index / kCharacterColumns, index % kCharacterColumns);
    }

    for (int column = 0; column < kCharacterColumns; ++column) {
        m_characterGrid->setColumnStretch(column, 1);
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
        card->setMinimumWidth(220);

        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(20, 20, 20, 20);
        cardLayout->setSpacing(10);

        auto *foodLabel = new QLabel(card);
        foodLabel->setAlignment(Qt::AlignCenter);
        foodLabel->setPixmap(scaledPixmap(foodImagePath(kind), 84, 84));
        foodLabel->setMinimumHeight(86);

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

        cardLayout->addWidget(foodLabel);
        cardLayout->addWidget(nameLabel);
        cardLayout->addWidget(priceLabel);
        cardLayout->addWidget(countLabel);
        cardLayout->addStretch();
        cardLayout->addWidget(buyButton);

        m_shopGrid->addWidget(card, index / kShopColumns, index % kShopColumns);
    }

    for (int column = 0; column < kShopColumns; ++column) {
        m_shopGrid->setColumnStretch(column, 1);
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
