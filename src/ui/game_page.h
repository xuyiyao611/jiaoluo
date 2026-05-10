#pragma once

#include <QColor>
#include <QPoint>
#include <QRect>
#include <QSet>
#include <QVector>
#include <QWidget>

#include "app/app_state.h"

class QLabel;
class QGridLayout;
class QPushButton;
class QWidget;
class QMediaPlayer;
class QAudioOutput;
class QSoundEffect;
class QTimer;

struct Match3RoundResult {
    bool clearedTarget = false;
    int baseScore = 0;
    int moveBonusScore = 0;
    int specialBonusScore = 0;
    int finalScore = 0;
    int coinReward = 0;
    int movesUsed = 0;
    int movesLeft = 0;
    Difficulty difficulty = Difficulty::Easy;
    QVector<int> clearedCounts;
    QVector<CharacterKind> activeKinds;
    int remainingSpecialTiles = 0;
    int activatedSpecialCount = 0;
};

Q_DECLARE_METATYPE(Match3RoundResult)

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr);

    void setDifficulty(Difficulty difficulty);
    void setCoins(int coins);
    void startNewRound();

signals:
    void backToHomeRequested();
    void backToDifficultyRequested();
    void roundFinished(const Match3RoundResult &result);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    enum class SpecialType {
        None,
        RowClear,
        ColClear,
        Bomb
    };

    struct TileData {
        int kind = 0;
        SpecialType special = SpecialType::None;
    };

    struct Match3DisplayConfig {
        int rows;
        int cols;
        int moveLimit;
        int targetScore;
    };

    struct MatchGroup {
        QVector<QPoint> positions;
        bool horizontal = true;
    };

    struct SpecialSpawn {
        QPoint position;
        int kind = 0;
        SpecialType special = SpecialType::None;
    };

    struct FallingStep {
        int fromRow = 0;
        int toRow = 0;
        int col = 0;
    };

    struct BeamAnimation {
        QRect rect;
        bool horizontal = true;
        qreal progress = 0.0;
        QColor color;
    };

    void rebuildBoardWidgets();
    void initializeBoard();
    void updateBoardView();
    void updateHeaderText();
    QString tileColor(int kind) const;
    QRect cellRect(int row, int col) const;
    void syncBoardCellGeometry();
    Match3DisplayConfig configForDifficulty(Difficulty difficulty) const;
    bool hasImmediateMatchAt(const QVector<TileData> &board, int row, int col) const;
    bool isAdjacent(const QPoint &first, const QPoint &second) const;
    QVector<MatchGroup> collectMatchGroups(const QVector<TileData> &board) const;
    void animateClearedTiles(const QSet<int> &matchedIndexes);
    void animateFallingTiles(const QVector<FallingStep> &steps);
    void handleCellClicked(int row, int col);
    void swapCells(QVector<TileData> &board, const QPoint &first, const QPoint &second) const;
    void resolveMove(const QPoint &preferredSpecialPosition, int &generatedSpecialCount, int &activatedSpecialCount);
    QVector<SpecialSpawn> createSpecialSpawns(
        const QVector<TileData> &board,
        const QVector<MatchGroup> &groups,
        const QPoint &preferredPosition) const;
    int applySpecialCreation(QVector<TileData> &board, QSet<int> &matchedIndexes, const QVector<SpecialSpawn> &spawns) const;
    int expandSpecialEffects(
        const QVector<TileData> &board,
        QSet<int> &matchedIndexes,
        QVector<SpecialSpawn> *triggeredSpecials = nullptr) const;
    QVector<FallingStep> collapseBoard(const QSet<int> &matchedIndexes);
    void finishRound(bool clearedTarget);
    int boardIndex(int row, int col) const;
    int randomKind() const;
    CharacterKind kindForIndex(int kind) const;
    bool hasPlayableState() const;
    int settleRemainingSpecialTiles(int &activatedSpecialCount) const;
    void playMatchSound();
    void playLaserSound();
    void triggerBeamAnimations(const QVector<SpecialSpawn> &triggeredSpecials);
    void advanceBeamAnimations();
    void renderBeamOverlay();

    Difficulty m_difficulty;
    int m_coins;
    int m_score;
    int m_movesLeft;
    QLabel *m_titleLabel;
    QLabel *m_metaLabel;
    QLabel *m_goalLabel;
    QLabel *m_hintLabel;
    QWidget *m_boardWidget;
    QLabel *m_beamOverlay;
    QGridLayout *m_boardLayout;
    QVector<QPushButton *> m_boardCells;
    QVector<TileData> m_boardTiles;
    QVector<int> m_clearedCounts;
    QPoint m_selectedCell;
    QString m_statusMessage;
    bool m_roundFinished;
    bool m_animating;
    int m_specialTilesLeft;
    QMediaPlayer *m_bgmPlayer;
    QAudioOutput *m_bgmOutput;
    QSoundEffect *m_matchSound;
    QSoundEffect *m_laserSound;
    QTimer *m_beamTimer;
    QVector<BeamAnimation> m_beamAnimations;
};
