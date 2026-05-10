#include "landing_page.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLinearGradient>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

LandingPage::LandingPage(QWidget *parent)
    : QWidget(parent),
      m_titleLabel(new QLabel(QStringLiteral("角落消消"), this)),
      m_startButton(new QPushButton(QStringLiteral("Tap to Start"), this)),
      m_newGameButton(new QPushButton(QStringLiteral("开新游戏"), this))
{
    buildUi();
}

void LandingPage::buildUi()
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(40, 40, 40, 24);
    rootLayout->setSpacing(0);

    auto *card = new QWidget(this);
    card->setObjectName(QStringLiteral("landingCard"));

    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(48, 38, 48, 38);
    cardLayout->setSpacing(16);

    auto *subtitleLabel = new QLabel(QStringLiteral("Cozy Match"), card);
    subtitleLabel->setObjectName(QStringLiteral("landingSubtitle"));
    subtitleLabel->setAlignment(Qt::AlignCenter);

    m_titleLabel->setObjectName(QStringLiteral("landingTitle"));
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_startButton->setObjectName(QStringLiteral("landingStartButton"));
    m_startButton->setCursor(Qt::PointingHandCursor);

    cardLayout->addWidget(subtitleLabel);
    cardLayout->addWidget(m_titleLabel);
    cardLayout->addWidget(m_startButton, 0, Qt::AlignCenter);

    rootLayout->addStretch();
    rootLayout->addWidget(card, 0, Qt::AlignCenter);
    rootLayout->addStretch();

    auto *bottomRow = new QHBoxLayout();
    bottomRow->addStretch();
    bottomRow->addWidget(m_newGameButton);
    rootLayout->addLayout(bottomRow);

    m_newGameButton->setObjectName(QStringLiteral("landingNewGameButton"));
    m_newGameButton->setCursor(Qt::PointingHandCursor);

    connect(m_startButton, &QPushButton::clicked, this, &LandingPage::startRequested);
    connect(m_newGameButton, &QPushButton::clicked, this, &LandingPage::newGameRequested);

    setStyleSheet(QStringLiteral(
        "#landingCard {"
        "  background: rgba(255, 250, 245, 215);"
        "  border: 1px solid rgba(188, 165, 150, 120);"
        "  border-radius: 30px;"
        "}"
        "#landingSubtitle {"
        "  color: #ba8a73;"
        "  font-size: 15px;"
        "  font-weight: 700;"
        "  letter-spacing: 4px;"
        "}"
        "#landingTitle {"
        "  color: #705f59;"
        "  font-size: 56px;"
        "  font-weight: 800;"
        "  padding: 6px 0 10px 0;"
        "}"
        "#landingStartButton {"
        "  min-width: 220px;"
        "  min-height: 52px;"
        "  padding: 10px 26px;"
        "  border: none;"
        "  border-radius: 26px;"
        "  background: #f7d7c6;"
        "  color: #8f6256;"
        "  font-size: 20px;"
        "  font-weight: 700;"
        "}"
        "#landingStartButton:hover {"
        "  background: #f4c9b3;"
        "}"
        "#landingNewGameButton {"
        "  min-height: 42px;"
        "  padding: 8px 18px;"
        "  border-radius: 21px;"
        "  border: 1px solid rgba(167, 150, 136, 150);"
        "  background: rgba(255, 250, 245, 220);"
        "  color: #846d65;"
        "  font-size: 15px;"
        "  font-weight: 600;"
        "}"
    ));
}

void LandingPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient backgroundGradient(rect().topLeft(), rect().bottomLeft());
    backgroundGradient.setColorAt(0.0, QColor(252, 248, 244));
    backgroundGradient.setColorAt(1.0, QColor(248, 242, 234));
    painter.fillRect(rect(), backgroundGradient);

    painter.setOpacity(0.18);
    painter.drawPixmap(rect(), QPixmap(QStringLiteral(":/images/landing-background.jpg")));
    painter.setOpacity(1.0);

    QPen stripePen(QColor(245, 226, 213, 90), 2.0);
    painter.setPen(stripePen);
    for (int x = -height(); x < width(); x += 36) {
        painter.drawLine(x, 0, x + height(), height());
    }
}
