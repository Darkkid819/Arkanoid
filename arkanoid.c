#include "raylib.h"

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

int main(void) {

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "PROJECT: ARKANOID");

    Texture2D texLogo = LoadTexture("resources/raylib_logo.png");
    Texture2D texBall = LoadTexture("resources/ball.png");
    Texture2D texPaddle = LoadTexture("resources/paddle.png");
    Texture2D texBrick = LoadTexture("resources/brick.png");

    // game required variables
    GameScreen screen = LOGO;

    int framesCounter = 0;
    int gameResult = -1;
    bool gamePaused = false;

    Player player = {0};
    Ball ball = {0};
    Brick bricks[BRICKS_LINES][BRICKS_PER_LINE] = {0};

    // initialize player
    player.position = (Vector2){screenWidth/2, screenHeight*7/8};
    player.speed = (Vector2){8.0f, 0.0f};
    player.size = (Vector2){100, 24};
    player.lives = PLAYER_LIVES;

    // initialize ball
    ball.radius = 10.0f;
    ball.active = false;
    ball.position = (Vector2){player.position.x + player.size.x/2, player.position.y - ball.radius*2};
    ball.speed = (Vector2){4.0f, 4.0f};

    // initialize bricks
    for (int i = 0; i < BRICKS_LINES; i++) {
        for (int j = 0; j < BRICKS_PER_LINE; j++) {
            bricks[i][j].size = (Vector2){screenWidth/BRICKS_PER_LINE, 20};
            bricks[i][j].position = (Vector2){j * bricks[i][j].size.x, i * bricks[i][j].size.y + BRICKS_POSITION_Y};
            bricks[i][j].bounds = (Rectangle){bricks[i][j].position.x, bricks[i][j].position.y, bricks[i][j].size.x, bricks[i][j].size.y};
            bricks[i][j].active = true;
        }
    }

    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        switch(screen) {
            case LOGO: {
                framesCounter++;

                if (framesCounter > 180) {
                    screen = TITLE;
                    framesCounter = 0;
                }
            } break;
            case TITLE: {
                framesCounter++;

                if (IsKeyPressed(KEY_ENTER)) screen = GAMEPLAY;
            } break;
            case GAMEPLAY: {
                if (!gamePaused) {
                    // player movement
                    if (IsKeyDown(KEY_LEFT)) {
                        player.position.x -= player.speed.x;
                    }
                    if (IsKeyDown(KEY_RIGHT)) {
                        player.position.x += player.speed.x;
                    }
                    
                    if (player.position.x <= 0) {
                        player.position.x = 0;
                    }
                    if (player.position.x + player.size.x >= screenWidth) {
                        player.position.x = screenWidth - player.size.x;
                    }

                    player.bounds = (Rectangle){player.position.x, player.position.y, player.size.x, player.size.y};

                    // ball movement
                    if (ball.active) {
                        ball.position.x += ball.speed.x;
                        ball.position.y += ball.speed.y;

                        if (ball.position.x + ball.radius >= screenWidth || ball.position.x - ball.radius <= 0) {
                            ball.speed.x *= -1;
                        }
                        if (ball.position.y - ball.radius <= 0) {
                            ball.speed.y *= -1;
                        }

                        if (CheckCollisionCircleRec(ball.position, ball.radius, player.bounds)) {
                            ball.speed.y *= -1;
                            ball.speed.x = (ball.position.x - (player.position.x + player.size.x / 2)) / player.size.x * 5.0f;
                        }

                        for (int i = 0; i < BRICKS_LINES; i++) {
                            for (int j = 0; j < BRICKS_PER_LINE; j++) {
                                if (bricks[i][j].active && CheckCollisionCircleRec(ball.position, ball.radius, bricks[i][j].bounds)) {
                                    bricks[i][j].active = false;
                                    ball.speed.y *= -1;

                                    break;
                                }
                            }
                        }

                        // game ending logic
                        if (ball.position.y + ball.radius >= screenHeight) {
                            ball.position.x = player.position.x + player.size.x / 2;
                            ball.position.y = player.position.y - ball.radius - 1.0f;
                            ball.speed = (Vector2){0, 0};
                            ball.active = false;

                            player.lives--;
                        }

                        if (player.lives < 0) {
                            screen = ENDING;
                            player.lives = 5;
                            framesCounter = 0;
                        }
                    } else {
                        ball.position.x = player.position.x + player.size.x/2;

                        if (IsKeyPressed(KEY_SPACE)) {
                            ball.active = true;
                            ball.speed = (Vector2){0, -5.0f};
                        }
                    }
                }

            } break;
            case ENDING: {
                framesCounter++;

                if(IsKeyPressed(KEY_ENTER)) {
                    // TODO: replay/exit game logic
                    screen = TITLE;
                }
            } break;
            default: break;
        }

        BeginDrawing();
            {
                ClearBackground(RAYWHITE);

                switch (screen) {
                    case LOGO: {
                        DrawTexture(texLogo, screenWidth / 2 - texLogo.width / 2, screenHeight / 2 - texLogo.height / 2, WHITE);
                    } break;
                    case TITLE: {
                        DrawText("ARKANOID", 20, 20, 40, DARKGREEN);
                        if((framesCounter / 30) % 2 == 0) {
                            DrawText("PRESS [ENTER] TO START", 
                                GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO START", 20) / 2,
                                GetScreenHeight() / 2 + 60,
                                20,
                                DARKGRAY);
                        }
                    } break;
                    case GAMEPLAY: {
                        #ifdef SHAPES
                            // draw player and ball
                            DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, BLACK);
                            DrawCircleV(ball.position, ball.radius, MAROON);

                            // draw bricks
                            for (int i = 0; i < BRICKS_LINES; i++) {
                                for (int j = 0; j < BRICKS_PER_LINE; j++) {
                                    if (bricks[i][j].active) {
                                        if ((i + j) % 2 == 0) {
                                            DrawRectangle(bricks[i][j].position.x, bricks[i][j].position.y, bricks[i][j].size.x, bricks[i][j].size.y, GRAY);
                                        } else {
                                            DrawRectangle(bricks[i][j].position.x, bricks[i][j].position.y, bricks[i][j].size.x, bricks[i][j].size.y, DARKGRAY);
                                        }
                                    }
                                }
                            }
                        #elif defined TEXTURES
                            DrawTextureEx(texPaddle, player.position, 0.0f, 1.0f, WHITE);
                            DrawTexture(texBall, ball.position.x - ball.radius / 2, ball.position.y - ball.radius / 2, MAROON);

                            for (int i = 0; i < BRICKS_LINES; i++) {
                                for (int j = 0; j < BRICKS_PER_LINE; j++) {
                                    if (bricks[i][j].active) {
                                        if ((i + j) % 2 == 0) {
                                            DrawTextureEx(texBrick, bricks[i][j].position, 0.0f, 1.0f, GRAY);
                                        } else {
                                            DrawTextureEx(texBrick, bricks[i][j].position, 0.0f, 1.0f, DARKGRAY);
                                        }
                                    }
                                }
                            }
                        #endif

                        // draw player lives
                        for (int i = 0; i < player.lives; i++) {
                            DrawRectangle(20 + 40 * i, screenHeight - 30, 35, 10, LIGHTGRAY);
                        }

                        // draw pause screen if needed
                        if (gamePaused) {
                            DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40), screenHeight / 2 + 60, 40, GRAY);
                        }
                    } break;
                    case ENDING: {
                        DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                        
                        if ((framesCounter) % 2 == 0) {
                            DrawText("PRESS [ENTER] TO PLAY AGAIN", 
                                GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
                                GetScreenHeight() / 2 + 80,
                                20,
                                GRAY);
                        }
                    } break;
                    default: break;
                }
            }
        EndDrawing();

    }

    UnloadTexture(texBall);
    UnloadTexture(texPaddle);
    UnloadTexture(texBrick);
    UnloadTexture(texLogo);

    CloseWindow();

    return 0;
}