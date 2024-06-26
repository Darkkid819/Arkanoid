#include "raylib.h"
#include "arkanoid.h"

int main(void) {

    InitWindow(screenWidth, screenHeight, "ARKANOID");
    InitAudioDevice();
    InitGame();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateGame();
        BeginDrawing(); 
        {
            DrawFrame();
        }
        EndDrawing();
    }

    DeInitGame();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void InitGame(void) {
    InitResources();
    InitVariables();
}

void InitResources(void) {
    texLogo = LoadTexture("resources/raylib_logo.png");
    texBall = LoadTexture("resources/ball.png");
    texPaddle = LoadTexture("resources/paddle.png");
    texBrick = LoadTexture("resources/brick.png");

    font = LoadFont("resources/setback.png");

    fxStart = LoadSound("resources/start.wav");
    fxBounce = LoadSound("resources/bounce.wav");
    fxExplosion = LoadSound("resources/explosion.wav");
}

void InitVariables(void) {
    win = false;

    player.position = (Vector2){screenWidth/2, screenHeight*7/8};
    player.speed = (Vector2){8.0f, 0.0f};
    player.size = (Vector2){100, 24};
    player.lives = PLAYER_LIVES;

    ball.radius = 10.0f;
    ball.active = false;
    ball.position = (Vector2){player.position.x + player.size.x/2, player.position.y - ball.radius};
    ball.speed = (Vector2){4.0f, 4.0f};

    for (int i = 0; i < BRICKS_LINES; i++) {
        for (int j = 0; j < BRICKS_PER_LINE; j++) {
            bricks[i][j].size = (Vector2){screenWidth/BRICKS_PER_LINE, 20};
            bricks[i][j].position = (Vector2){j * bricks[i][j].size.x, i * bricks[i][j].size.y + BRICKS_POSITION_Y};
            bricks[i][j].bounds = (Rectangle){bricks[i][j].position.x, bricks[i][j].position.y, bricks[i][j].size.x, bricks[i][j].size.y};
            bricks[i][j].active = true;
        }
    }
}

void UpdateGame(void) {
    switch(screen) {
        case LOGO: {
            UpdateLogo();
        } break;
        case TITLE: {
            UpdateTitle();
        } break;
        case GAMEPLAY: {
            UpdateGameplay();   
        } break;
        case ENDING:
        case WIN: {
            UpdateEnding(); 
        } break;
        default: break;
    }
}

void UpdateLogo(void) {
    framesCounter++;

    if (framesCounter > 180) {
        screen = TITLE;
        framesCounter = 0;
    }
}

void UpdateTitle(void) {
    framesCounter++;

    if (IsKeyPressed(KEY_ENTER)){
        screen = GAMEPLAY;
        PlaySound(fxStart);
    }
}

void UpdateGameplay(void) {
    if (IsKeyPressed('P')) gamePaused = !gamePaused;

    if (!gamePaused) {
        UpdateMovement();
    }
}

void UpdateMovement(void) {
    UpdatePlayerMovement();
    if (ball.active) {
        UpdateBallMovement();  
        CheckCollisions();
        CheckEnding();
        CheckWin();
    } else {
        ResetBall();
    }     
}

void UpdatePlayerMovement(void) {
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
}

void UpdateBallMovement(void) {
    ball.position.x += ball.speed.x;
    ball.position.y += ball.speed.y;

    if (ball.position.x + ball.radius >= screenWidth || ball.position.x - ball.radius <= 0) {
        ball.speed.x *= -1;
    }
    if (ball.position.y - ball.radius <= 0) {
        ball.speed.y *= -1;
    }
}

void CheckCollisions(void) {
    if (CheckCollisionCircleRec(ball.position, ball.radius, player.bounds)) {
        ball.speed.y *= -1;
        ball.speed.x = (ball.position.x - (player.position.x + player.size.x / 2)) / player.size.x * 5.0f;
        PlaySound(fxBounce);
    }

    for (int i = 0; i < BRICKS_LINES; i++) {
        for (int j = 0; j < BRICKS_PER_LINE; j++) {
            if (bricks[i][j].active && CheckCollisionCircleRec(ball.position, ball.radius, bricks[i][j].bounds)) {
                bricks[i][j].active = false;
                ball.speed.y *= -1;
                PlaySound(fxExplosion);

                break;
            }
        }
    }
}

void CheckEnding(void) {
    if (ball.position.y + ball.radius >= screenHeight) {
        ball.position.x = player.position.x + player.size.x / 2;
        ball.position.y = player.position.y - ball.radius - 1.0f;
        ball.speed = (Vector2){0, 0};
        ball.active = false;

        player.lives--;
    }

    if (player.lives < 0) {
        screen = ENDING;
        InitVariables();
        framesCounter = 0;
    }
}

void UpdateEnding(void) {
    framesCounter++;

    if(IsKeyPressed(KEY_ENTER)) {
        // TODO: replay/exit game logic
        screen = TITLE;
    }
}

void CheckWin(void) {
    win = true;
    for (int i = 0; i < BRICKS_LINES; i++) {
        for (int j = 0; j < BRICKS_PER_LINE; j++) {
            if (bricks[i][j].active) {
                win = false;
                break;
            }
        }
        if (!win) break;
    }

    if (win) {
        screen = WIN;
        InitVariables();
        framesCounter = 0;
    }
}

void ResetBall(void) {
    ball.position.x = player.position.x + player.size.x/2;

    if (IsKeyPressed(KEY_SPACE)) {
        ball.active = true;
        ball.speed = (Vector2){0, -5.0f};
    }
}

void DrawFrame(void) {
    ClearBackground(RAYWHITE);

    switch (screen) {
        case LOGO: {
            DrawLogo();
        } break;
        case TITLE: {
            DrawTitle();
        } break;
        case GAMEPLAY: {
            DrawGamePlay();
        } break;
        case ENDING: {
            DrawEnding();
        } break;
        case WIN: {
            DrawWin();
        }
        default: break;
    }
}

void DrawLogo(void) {
    DrawTexture(texLogo, screenWidth / 2 - texLogo.width / 2, screenHeight / 2 - texLogo.height / 2, WHITE);
}

void DrawTitle(void) {
    DrawTextEx(font, "ARKANOID", (Vector2){screenWidth / 2 - MeasureText("ARKANOID", 80) / 2, 80}, 80, 10, MAROON);
            
    if((framesCounter / 30) % 2 == 0) {
        DrawText("PRESS [ENTER] TO START", 
            GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO START", 20) / 2,
            GetScreenHeight() / 2 + 60,
            20,
            DARKGRAY);
    }
}

void DrawGamePlay(void) {
    #ifdef SHAPES
        DrawShapes();
    #elif defined TEXTURES
        DrawTextures();
    #endif
    DrawPlayerLives();
    DrawPauseScreen();
}

void DrawShapes(void) {
    DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, BLACK);
    DrawCircleV(ball.position, ball.radius, MAROON);

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
}

void DrawTextures(void) {
    DrawTextureEx(texPaddle, player.position, 0.0f, 1.0f, WHITE);
    DrawTexture(texBall, ball.position.x - ball.radius, ball.position.y - ball.radius, MAROON);

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
}

void DrawPlayerLives(void) {
    for (int i = 0; i < player.lives; i++) {
        DrawRectangle(20 + 40 * i, screenHeight - 30, 35, 10, LIGHTGRAY);
    }
}

void DrawPauseScreen(void) {
    if (gamePaused) {
        DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40), screenHeight / 2 + 60, 40, GRAY);
    }
}

void DrawEnding(void) {
    DrawTextEx(font, "YOU LOSE", (Vector2){screenWidth / 2 - MeasureText("YOU LOSE", 80) / 2, 100}, 80, 6, MAROON);
                        
    if ((framesCounter) % 2 == 0) {
        DrawText("PRESS [ENTER] TO PLAY AGAIN", 
            GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
            GetScreenHeight() / 2 + 80,
            20,
            GRAY);
    }
}

void DrawWin(void) {
    DrawTextEx(font, "YOU WON", (Vector2){screenWidth / 2 - MeasureText("YOU WON", 80) / 2, 100}, 80, 6, MAROON);
                        
    if ((framesCounter) % 2 == 0) {
        DrawText("PRESS [ENTER] TO PLAY AGAIN", 
            GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
            GetScreenHeight() / 2 + 80,
            20,
            GRAY);
    }
}

void DeInitGame(void) {
    UnloadTexture(texBall);
    UnloadTexture(texPaddle);
    UnloadTexture(texBrick);
    UnloadTexture(texLogo);

    UnloadFont(font);

    UnloadSound(fxStart);
    UnloadSound(fxBounce);
    UnloadSound(fxExplosion);
}