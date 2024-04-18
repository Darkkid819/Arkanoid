#include "raylib.h"

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
    Vector2 position, speed, size;
    Rectangle bounds;
    int lives;
} Player;

typedef struct Ball {
    Vector2 position, speed;
    float radius;
    bool active;
} Ball;

typedef struct Brick {
    Vector2 position, size;
    Rectangle bounds;
    int resistance;
    bool active;
} Brick;

int main(void) {

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "PROJECT: ARKANOID");

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
    ball.position = (Vector2){player.position.x + player.position.x/2, player.position.y - ball.radius*2};
    ball.speed = (Vector2){4.0f, 4.0f};

    // initialize bricks
    for (int i = 0; i < BRICKS_LINES; i++) {
        for (int j = 0; j < BRICKS_PER_LINE; j++) {
            bricks[i][j].size = (Vector2){screenWidth/BRICKS_PER_LINE, 20};
            bricks[i][j].position = (Vector2){i*bricks[i][j].size.x, i*bricks[i][j].size.y + BRICKS_POSITION_Y};
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
                    // TODO: gameplay logic
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
                        DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
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

                        // draw player lives
                        for (int i = 0; i < PLAYER_LIVES; i++) {
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

    CloseWindow();

    return 0;
}