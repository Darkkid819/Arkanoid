#include "raylib.h"

typedef enum GameScreen {
    LOGO,
    TITLE,
    GAMEPLAY,
    ENDING
} GameScreen;

int main(void) {

    // initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "PROJECT: ARKANOID");

    GameScreen screen = LOGO;

    int framesCounter = 0;
    int gameResult = -1;
    bool gamePaused = false;

    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        // update
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

                if (IsKeyPressed(KEY_ENTER)) screen = ENDING;
            } break;
            case ENDING: {
                framesCounter++;

                if (IsKeyPressed(KEY_ENTER)) screen = TITLE;
            } break;
            default: break;
        }

        // draw
        BeginDrawing();
            {
                ClearBackground(RAYWHITE);

                switch (screen) {
                    case LOGO: {
                        DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
                        DrawText("Wait for 3 SECONDS...", 290, 220, 20, GRAY);
                    } break;
                    case TITLE: {
                        DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                        DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
                        DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, DARKGREEN);
                    } break;
                    case GAMEPLAY: {
                        DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
                        DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                        DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
                    } break;
                    case ENDING: {
                        DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                        DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                        DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, DARKBLUE);
                    } break;
                    default: break;
                }
            }
        EndDrawing();

    }

    // deinitialization
    CloseWindow();

    return 0;
}