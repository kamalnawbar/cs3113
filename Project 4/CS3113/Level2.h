#ifndef LEVEL2_H
#define LEVEL2_H

#include "Scene.h"

constexpr int LEVEL_2_WIDTH        = 46,
              LEVEL_2_HEIGHT       = 8,
              LEVEL_2_ENEMY_COUNT  = 8; // 3 Spikes + 3 Saws flottantes + 2 Spike Heads

class Level2 : public Scene
{
private:
    unsigned int mLevelData[LEVEL_2_WIDTH * LEVEL_2_HEIGHT] = {
        // Row 0 — ciel
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        // Row 1 — ciel
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        // Row 2 — ciel
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        // Row 3 — ciel
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        // Row 4 — plateformes interactives + pierres de passage dans les gaps
        // col5=pierre gap1 | cols8-10=plateforme (passer au-dessus de la saw)
        // col12=pierre gap2 | cols19-20=pierres gap3 | col24=pierre gap4
        // cols27-29=plateforme (passer au-dessus de la saw + spike head)
        // cols31-32=pierres gap5 | col38=pierre gap6 | cols41-43=plateforme (eviter spike head final)
        0, 0, 0, 0, 0, 2, 0, 0, 2, 2, 2, 0, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 2, 2, 2, 0, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 2, 2, 0, 0,
        // Row 5 — fills sous les tuiles de row 4
        0, 0, 0, 0, 0, 3, 0, 0, 3, 3, 3, 0, 3, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 3, 0, 0, 3, 3, 3, 0, 3, 3, 0, 0, 0, 0, 0, 3, 0, 0, 3, 3, 3, 0, 0,
        // Row 6 — sol avec 6 gaps (cols 4-6, 12-13, 18-20, 23-25, 31-33, 38-39)
        2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 0, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2,
        // Row 7 — fill sol
        3, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 0, 0, 0, 3, 3, 0, 0, 0, 3, 3, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3,
    };

    PlayerState mPlayerState = IDLE_STATE;
    Vector2     mStartPosition;
    Vector2     mEnemyStartPositions[LEVEL_2_ENEMY_COUNT];
    float       mHitCooldown = 0.0f;

    void updatePlayerAnimation();
    void checkEnemyCollisions();

public:
    static constexpr float TILE_DIMENSION          = 75.0f,
                           ACCELERATION_OF_GRAVITY = 1100.0f,
                           HIT_COOLDOWN_DURATION   = 1.5f;

    Level2();
    Level2(Vector2 origin, const char *bgHexCode);
    ~Level2();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVEL2_H