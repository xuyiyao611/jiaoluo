#include "difficulty_select_page.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLinearGradient>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

DifficultySelectPage::DifficultySelectPage(QWidget *parent)
    : QWidget(parent),
      m_easyCard(new QFrame(this)),
      m_hardCard(new QFrame(this)),
      m_selectedDifficulty(Difficulty::Easy)
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(36, 28, 36, 28);
    rootLayout->setSpacing(18);

    auto *headerCard = new QFrame(this);
    headerCard->setObjectName(QStringLiteral("difficultyHeaderCard"));
    auto *headerLayout = new QVBoxLayout(headerCard);
    headerLayout->setContentsMargins(26, 22, 26, 22);
    headerLayout->setSpacing(8);

    auto *eyebrowLabel = new QLabel(QStringLiteral("P1 难度选择"), headerCard);
    eyebrowLabel->setObjectName(QStringLiteral("difficultyEyebrow"));

    auto *titleLabel = new QLabel(QStringLiteral("为 角落消消 选择难度"), headerCard);
    titleLabel->setObjectName(QStringLiteral("difficultyTitle"));

    auto *descLabel = new QLabel(
        QStringLiteral("难度会切换目标分数、步数限制和本局刷新元素，棋盘统一为 9 x 9。"),
        headerCard);
    descLabel->setWordWrap(true);
    descLabel->setObjectName(QStringLiteral("difficultyDesc"));

    headerLayout->addWidget(eyebrowLabel);
    headerLayout->addWidget(titleLabel);
    headerLayout->addWidget(descLabel);

    auto *cardLayout = new QHBoxLayout();
    cardLayout->setSpacing(18);

    auto buildCard = [this](QFrame *container, Difficulty difficulty) {
        auto *layout = new QVBoxLayout(container);
        layout->setContentsMargins(22, 20, 22, 20);
        layout->setSpacing(12);

        auto *title = new QLabel(difficultyLabel(difficulty), container);
        title->setObjectName(QStringLiteral("difficultyCardTitle"));

        auto *desc = new QLabel(difficultyDescription(difficulty), container);
        desc->setWordWrap(true);
        desc->setObjectName(QStringLiteral("difficultyCardDesc"));

        auto *chooseButton = new QPushButton(QStringLiteral("选择此难度"), container);
        chooseButton->setObjectName(QStringLiteral("difficultyChooseButton"));
        chooseButton->setCursor(Qt::PointingHandCursor);

        layout->addWidget(title);
        layout->addWidget(desc);
        layout->addStretch();
        layout->addWidget(chooseButton, 0, Qt::AlignLeft);

        connect(chooseButton, &QPushButton::clicked, this, [this, difficulty]() {
            m_selectedDifficulty = difficulty;
            updateCardStyles();
            emit difficultyChosen(difficulty);
        });
    };

    m_easyCard->setObjectName(QStringLiteral("difficultyCard"));
    m_hardCard->setObjectName(QStringLiteral("difficultyCard"));
    buildCard(m_easyCard, Difficulty::Easy);
    buildCard(m_hardCard, Difficulty::Hard);

    cardLayout->addWidget(m_easyCard);
    cardLayout->addWidget(m_hardCard);

    auto *backRow = new QHBoxLayout();
    backRow->addStretch();
    auto *backButton = new QPushButton(QStringLiteral("返回主页"), this);
    backButton->setObjectName(QStringLiteral("difficultyBackButton"));
    backButton->setCursor(Qt::PointingHandCursor);
    backRow->addWidget(backButton);

    rootLayout->addWidget(headerCard);
    rootLayout->addLayout(cardLayout);
    rootLayout->addStretch();
    rootLayout->addLayout(backRow);

    connect(backButton, &QPushButton::clicked, this, &DifficultySelectPage::backRequested);

    setStyleSheet(QStringLiteral(
        "#difficultyHeaderCard {"
        "  background: rgba(255, 251, 246, 225);"
        "  border: 1px solid rgba(190, 174, 161, 110);"
        "  border-radius: 28px;"
        "}"
        "#difficultyEyebrow {"
        "  color: #b98a72;"
        "  font-size: 13px;"
        "  font-weight: 700;"
        "  letter-spacing: 2px;"
        "}"
        "#difficultyTitle {"
        "  color: #6b605c;"
        "  font-size: 34px;"
        "  font-weight: 800;"
        "}"
        "#difficultyDesc, #difficultyCardDesc {"
        "  color: #887a73;"
        "  font-size: 15px;"
        "  line-height: 1.6;"
        "}"
        "#difficultyCardTitle {"
        "  color: #6f615b;"
        "  font-size: 22px;"
        "  font-weight: 800;"
        "}"
        "#difficultyChooseButton {"
        "  min-height: 42px;"
        "  padding: 8px 18px;"
        "  border-radius: 18px;"
        "  border: 1px solid rgba(183, 143, 111, 96);"
        "  background: rgba(251, 236, 220, 235);"
        "  color: #8f664d;"
        "  font-size: 15px;"
        "  font-weight: 700;"
        "}"
        "#difficultyChooseButton:hover {"
        "  background: rgba(247, 227, 207, 245);"
        "  color: #7f5a45;"
        "}"
        "#difficultyBackButton {"
        "  min-height: 46px;"
        "  padding: 10px 20px;"
        "  border-radius: 18px;"
        "  border: none;"
        "  background: #f1e8de;"
        "  color: #6f6761;"
        "  font-size: 16px;"
        "  font-weight: 700;"
        "}"
    ));

    updateCardStyles();
}

void DifficultySelectPage::setSelectedDifficulty(Difficulty difficulty)
{
    m_selectedDifficulty = difficulty;
    updateCardStyles();
}

void DifficultySelectPage::updateCardStyles()
{
    const QString inactiveStyle =
        QStringLiteral("background: rgba(255, 255, 255, 210);"
                       "border: 1px solid rgba(191, 170, 154, 70);"
                       "border-radius: 26px;");
    const QString activeStyle =
        QStringLiteral("background: rgba(255, 247, 236, 235);"
                       "border: 2px solid rgba(221, 165, 124, 170);"
                       "border-radius: 26px;");

    m_easyCard->setStyleSheet(m_selectedDifficulty == Difficulty::Easy ? activeStyle : inactiveStyle);
    m_hardCard->setStyleSheet(m_selectedDifficulty == Difficulty::Hard ? activeStyle : inactiveStyle);
}

void DifficultySelectPage::paintEvent(QPaintEvent *event)
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
