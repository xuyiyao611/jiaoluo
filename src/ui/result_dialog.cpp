#include "result_dialog.h"

#include <QDialogButtonBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

#include "app/app_state.h"

namespace {

QLabel *createImageLabel(const QString &path, QWidget *parent)
{
    auto *label = new QLabel(parent);
    label->setFixedSize(52, 52);
    label->setAlignment(Qt::AlignCenter);
    label->setPixmap(QPixmap(path).scaled(44, 44, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    return label;
}

} // namespace

ResultDialog::ResultDialog(const Match3RoundResult &result, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(result.clearedTarget ? QStringLiteral("挑战成功") : QStringLiteral("挑战失败"));
    setModal(true);
    resize(760, 620);
    setMinimumSize(680, 560);

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(26, 24, 26, 24);
    rootLayout->setSpacing(16);

    auto *headerCard = new QFrame(this);
    headerCard->setObjectName(QStringLiteral("resultCard"));
    auto *headerLayout = new QVBoxLayout(headerCard);
    headerLayout->setContentsMargins(22, 20, 22, 20);
    headerLayout->setSpacing(8);

    auto *titleLabel = new QLabel(
        result.clearedTarget ? QStringLiteral("角落消消挑战成功") : QStringLiteral("角落消消挑战失败"),
        headerCard);
    titleLabel->setObjectName(QStringLiteral("resultTitle"));

    auto *summaryLabel = new QLabel(
        result.clearedTarget
            ? QStringLiteral("本局奖励已经结算，下面是分数与碎片明细。")
            : QStringLiteral("本局未达成目标分数，因此不会发放金币与碎片奖励。"),
        headerCard);
    summaryLabel->setWordWrap(true);
    summaryLabel->setObjectName(QStringLiteral("resultSubtitle"));

    headerLayout->addWidget(titleLabel);
    headerLayout->addWidget(summaryLabel);

    auto *scoreCard = new QFrame(this);
    scoreCard->setObjectName(QStringLiteral("resultCard"));
    auto *scoreLayout = new QGridLayout(scoreCard);
    scoreLayout->setContentsMargins(22, 20, 22, 20);
    scoreLayout->setHorizontalSpacing(24);
    scoreLayout->setVerticalSpacing(14);

    const auto addScoreRow = [scoreLayout](int row, const QString &labelText, const QString &valueText) {
        auto *label = new QLabel(labelText);
        label->setObjectName(QStringLiteral("scoreLabel"));
        auto *value = new QLabel(valueText);
        value->setObjectName(QStringLiteral("scoreValue"));
        scoreLayout->addWidget(label, row, 0);
        scoreLayout->addWidget(value, row, 1);
    };

    addScoreRow(0, QStringLiteral("基础分"), QString::number(result.baseScore));
    addScoreRow(1, QStringLiteral("步数奖励"), QString::number(result.moveBonusScore));
    addScoreRow(2, QStringLiteral("特殊加分"), QString::number(result.specialBonusScore));
    addScoreRow(3, QStringLiteral("最终总分"), QString::number(result.finalScore));
    addScoreRow(4, QStringLiteral("金币奖励"), result.clearedTarget ? QStringLiteral("+%1").arg(result.coinReward) : QStringLiteral("0"));

    auto *fragmentCard = new QFrame(this);
    fragmentCard->setObjectName(QStringLiteral("resultCard"));
    auto *fragmentLayout = new QVBoxLayout(fragmentCard);
    fragmentLayout->setContentsMargins(22, 20, 22, 20);
    fragmentLayout->setSpacing(14);

    auto *fragmentTitle = new QLabel(QStringLiteral("碎片结算"), fragmentCard);
    fragmentTitle->setObjectName(QStringLiteral("sectionTitle"));
    fragmentLayout->addWidget(fragmentTitle);

    auto *fragmentGrid = new QGridLayout();
    fragmentGrid->setHorizontalSpacing(14);
    fragmentGrid->setVerticalSpacing(14);

    for (int index = 0; index < result.activeKinds.size() && index < result.clearedCounts.size(); ++index) {
        const CharacterKind kind = result.activeKinds.at(index);
        const int count = result.clearedTarget ? result.clearedCounts.at(index) : 0;

        auto *itemCard = new QFrame(fragmentCard);
        itemCard->setObjectName(QStringLiteral("fragmentItem"));
        auto *itemLayout = new QHBoxLayout(itemCard);
        itemLayout->setContentsMargins(14, 12, 14, 12);
        itemLayout->setSpacing(10);

        itemLayout->addWidget(createImageLabel(characterImagePath(kind), itemCard));

        auto *textLayout = new QVBoxLayout();
        textLayout->setSpacing(2);

        auto *nameLabel = new QLabel(characterName(kind), itemCard);
        nameLabel->setObjectName(QStringLiteral("fragmentName"));

        auto *countLabel = new QLabel(QStringLiteral("获得碎片：%1").arg(count), itemCard);
        countLabel->setObjectName(QStringLiteral("fragmentCount"));

        textLayout->addWidget(nameLabel);
        textLayout->addWidget(countLabel);
        itemLayout->addLayout(textLayout);

        fragmentGrid->addWidget(itemCard, index / 2, index % 2);
    }

    fragmentLayout->addLayout(fragmentGrid);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    rootLayout->addWidget(headerCard);
    rootLayout->addWidget(scoreCard);
    rootLayout->addWidget(fragmentCard);
    rootLayout->addWidget(buttonBox);

    setStyleSheet(QStringLiteral(
        "QDialog {"
        "  background: #f8f3ea;"
        "}"
        "#resultCard {"
        "  background: rgba(255, 251, 246, 236);"
        "  border: 1px solid rgba(190, 174, 161, 92);"
        "  border-radius: 24px;"
        "}"
        "#resultTitle {"
        "  color: #6b605c;"
        "  font-size: 28px;"
        "  font-weight: 800;"
        "}"
        "#resultSubtitle, #scoreLabel, #fragmentCount {"
        "  color: #887a73;"
        "  font-size: 14px;"
        "}"
        "#scoreValue, #fragmentName, #sectionTitle {"
        "  color: #6f615b;"
        "  font-size: 18px;"
        "  font-weight: 700;"
        "}"
        "#fragmentItem {"
        "  background: rgba(255, 255, 255, 216);"
        "  border: 1px solid rgba(195, 181, 169, 72);"
        "  border-radius: 18px;"
        "}"
        "QPushButton {"
        "  min-height: 40px;"
        "  min-width: 96px;"
        "  padding: 8px 18px;"
        "  border: none;"
        "  border-radius: 15px;"
        "  background: #efaf7d;"
        "  color: white;"
        "  font-size: 14px;"
        "  font-weight: 700;"
        "}"
    ));
}
