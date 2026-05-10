#include "home_page.h"

#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QLinearGradient>
#include <QPainter>
#include <QPixmap>
#include <QVBoxLayout>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(36, 28, 36, 28);
    rootLayout->setSpacing(20);

    auto *headerCard = new QFrame(this);
    headerCard->setObjectName(QStringLiteral("headerCard"));

    auto *headerLayout = new QVBoxLayout(headerCard);
    headerLayout->setContentsMargins(26, 22, 26, 22);
    headerLayout->setSpacing(8);

    auto *titleLabel = new QLabel(QStringLiteral("角落消消"), headerCard);
    titleLabel->setObjectName(QStringLiteral("homeTitle"));

    auto *descLabel = new QLabel(
        QStringLiteral("Qt / C++ 迁移工程第一步已完成。当前页面用于承接后续主页、难度选择、三消主玩法与结算界面的逐步迁移。"),
        headerCard);
    descLabel->setWordWrap(true);
    descLabel->setObjectName(QStringLiteral("homeDesc"));

    headerLayout->addWidget(titleLabel);
    headerLayout->addWidget(descLabel);

    auto *statusGrid = new QGridLayout();
    statusGrid->setHorizontalSpacing(14);
    statusGrid->setVerticalSpacing(14);

    const QStringList items = {
        QStringLiteral("Qt Widgets 工程"),
        QStringLiteral("CMake 构建"),
        QStringLiteral("启动页骨架"),
        QStringLiteral("主页骨架"),
        QStringLiteral("下一步迁移玩法"),
        QStringLiteral("下一步迁移资源")
    };

    for (int index = 0; index < items.size(); ++index) {
        auto *itemCard = new QFrame(this);
        itemCard->setObjectName(QStringLiteral("statusCard"));

        auto *itemLayout = new QVBoxLayout(itemCard);
        itemLayout->setContentsMargins(18, 16, 18, 16);
        itemLayout->setSpacing(6);

        auto *indexLabel = new QLabel(QStringLiteral("P%1").arg(index + 1), itemCard);
        indexLabel->setObjectName(QStringLiteral("statusIndex"));

        auto *textLabel = new QLabel(items.at(index), itemCard);
        textLabel->setWordWrap(true);
        textLabel->setObjectName(QStringLiteral("statusText"));

        itemLayout->addWidget(indexLabel);
        itemLayout->addWidget(textLabel);

        statusGrid->addWidget(itemCard, index / 3, index % 3);
    }

    rootLayout->addWidget(headerCard);
    rootLayout->addLayout(statusGrid);
    rootLayout->addStretch();

    setStyleSheet(QStringLiteral(
        "#headerCard {"
        "  background: rgba(255, 251, 246, 225);"
        "  border: 1px solid rgba(190, 174, 161, 110);"
        "  border-radius: 28px;"
        "}"
        "#homeTitle {"
        "  color: #6b605c;"
        "  font-size: 36px;"
        "  font-weight: 800;"
        "}"
        "#homeDesc {"
        "  color: #887a73;"
        "  font-size: 16px;"
        "  line-height: 1.6;"
        "}"
        "#statusCard {"
        "  background: rgba(255, 255, 255, 208);"
        "  border: 1px solid rgba(195, 181, 169, 95);"
        "  border-radius: 22px;"
        "}"
        "#statusIndex {"
        "  color: #b88c77;"
        "  font-size: 13px;"
        "  font-weight: 700;"
        "}"
        "#statusText {"
        "  color: #716662;"
        "  font-size: 18px;"
        "  font-weight: 700;"
        "}"
    ));
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
