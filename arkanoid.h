#ifndef ARKANOID_H
#define ARKANOID_H

#define TEXTURES // Alternative: SHAPES

#define PLAYER_LIVES 5
#define BRICKS_LINES 5
#define BRICKS_PER_LINE 20
#define BRICKS_POSITION_Y 50

typedef enum GameScreen {
    LOGO,
    TITLE,
    GAMEPLAY,
    ENDING
} GameScreen;

typedef struct Player {
    Vector2 position;
    Vector2 speed;
    Vector2 size;
    Rectangle bounds;
    int lives;
} Player;

typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    float radius;
    bool active;
} Ball;

typedef struct Brick {
    Vector2 position;
    Vector2 size;
    Rectangle bounds;
    int resistance;
    bool active;
} Brick;

const int screenWidth = 800;
const int screenHeight = 450;

int framesCounter = 0;
bool gamePaused = false;

GameScreen screen = LOGO;
Player player = {0};
Ball ball = {0};
Brick bricks[BRICKS_LINES][BRICKS_PER_LINE] = {0};

Texture2D texLogo;
Texture2D texBall;
Texture2D texPaddle;
Texture2D texBrick;

Font font;

Sound fxStart;
Sound fxBounce;
Sound fxExplosion;

void InitGame(void);
void InitResources(void);
void InitVariables(void);
void UpdateGame(void);
void UpdateLogo(void);
void UpdateTitle(void);
void UpdateGameplay(void);
void UpdateMovement(void);
void UpdatePlayerMovement(void);
void UpdateBallMovement(void);
void CheckCollisions(void);
void CheckEnding(void);
void UpdateEnding(void);
void ResetBall(void);
void DrawFrame(void);
void DrawLogo(void);
void DrawTitle(void);
void DrawGamePlay(void);
void DrawShapes(void);
void DrawTextures(void);
void DrawPlayerLives(void);
void DrawPauseScreen(void);
void DrawEnding(void);
void DeInitGame(void);

#endif