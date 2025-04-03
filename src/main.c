#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "ball.h"
#include "string.h"
#include "paddle.h"

int main(void) {
    int playerscore = 0;
    int enemyscore = 0;
    Paddle *player = (Paddle *)malloc(sizeof(Paddle));
    player->speed = 5;
    player->x = 0;
    player->y = 480;
    Paddle *enemy = (Paddle *)malloc(sizeof(Paddle));
    enemy->x = 580;
    enemy->y = 0;
    Ball *ball = (Ball *)malloc(sizeof(Ball));
    ball->speedx = 3;
    ball->speedy = 5;
    ball->x = 300;
    ball->y = 300;
    if (player == NULL || enemy == NULL || ball == NULL) {
        printf("Failed to allocate memory for the game objects\n");
        return 1;
    }
    const int screenWidth = 600;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Pong: Remastered");
    SetTargetFPS(60);

    enum GameState {
        SPLASH,
        GAME,
        GAMEOVER
    } gameState = SPLASH;

    enum Difficulty {
        EASY,
        MEDIUM,
        HARD
    } difficulty = MEDIUM;

    float splashTimer = 10.0f;
    int selectedDifficulty = 1;

    while (!WindowShouldClose()) {
        switch (gameState) {
            case SPLASH:
                splashTimer -= GetFrameTime();
                if (IsKeyPressed(KEY_UP)) {
                    selectedDifficulty = (selectedDifficulty - 1 + 3) % 3;
                }
                if (IsKeyPressed(KEY_DOWN)) {
                    selectedDifficulty = (selectedDifficulty + 1) % 3;
                }
                if (IsKeyPressed(KEY_SPACE)) {
                    difficulty = (enum Difficulty)selectedDifficulty;
                    switch (difficulty) {
                        case EASY:
                            enemy->speed = 3;
                            break;
                        case MEDIUM:
                            enemy->speed = 4;
                            break;
                        case HARD:
                            enemy->speed = 6;
                            break;
                    }
                    gameState = GAME;
                }
                BeginDrawing();
                ClearBackground(BLACK);
                DrawText("Pong: Remastered", screenWidth / 2 - MeasureText("Pong: Remastered", 40) / 2, screenHeight / 2 - 80, 40, WHITE);
                DrawText("Difficulty:", screenWidth / 2 - MeasureText("Difficulty:", 20) / 2, screenHeight / 2 - 20, 20, WHITE);

                const char *difficultyNames[] = {"Easy", "Medium", "Hard"};
                for (int i = 0; i < 3; i++) {
                    Color textColor = (i == selectedDifficulty) ? YELLOW : WHITE;
                    DrawText(difficultyNames[i], screenWidth / 2 - MeasureText(difficultyNames[i], 20) / 2, screenHeight / 2 + (i * 25), 20, textColor);
                }
                DrawText("Press SPACE to start", screenWidth / 2 - MeasureText("Press SPACE to start", 20) / 2, screenHeight / 2 + 100, 20, WHITE);
                EndDrawing();
                break;
            case GAME:
                if (IsKeyDown(KEY_W) && player->y > 0) {
                    player->y -= player->speed;
                }
                if (IsKeyDown(KEY_S) && player->y < screenHeight - 120) {
                    player->y += player->speed;
                }
                if (ball->x >= screenWidth || ball->x <= 0) {
                    ball->speedx *= -1;
                    if (ball->x >= screenWidth) {
                        playerscore++;
                        ball->x = screenWidth / 2;
                        ball->y = screenHeight / 2;
                    } else if (ball->x <= 0) {
                        enemyscore++;
                        ball->x = screenWidth / 2;
                        ball->y = screenHeight / 2;
                    }
                }
                if (ball->y >= screenHeight || ball->y <= 0) {
                    ball->speedy *= -1;
                }
                ball->x += ball->speedx;
                ball->y += ball->speedy;
                if (enemy->y + 60 < ball->y && enemy->y < screenHeight - 120) {
                    enemy->y += enemy->speed;
                } else if (enemy->y + 60 > ball->y && enemy->y > 0) {
                    enemy->y -= enemy->speed;
                }

                Rectangle playerRect = {player->x, player->y, 20, 120};
                Rectangle enemyRect = {enemy->x, enemy->y, 20, 120};
                Vector2 ballPos = {ball->x, ball->y};

                if (CheckCollisionCircleRec(ballPos, 15, playerRect) || CheckCollisionCircleRec(ballPos, 15, enemyRect)) {
                    ball->speedx *= -1;
                }

                BeginDrawing();
                ClearBackground(GREEN);
                DrawCircle(ball->x, ball->y, 15, RED);
                DrawRectangle(player->x, player->y, 20, 120, WHITE);
                DrawRectangle(enemy->x, enemy->y, 20, 120, BLACK);
                char scoreText[50];
                sprintf(scoreText, "Player: %d | Enemy: %d", playerscore, enemyscore);
                DrawText(scoreText, screenWidth / 2 - MeasureText(scoreText, 20) / 2, 10, 20, WHITE);
                EndDrawing();

                if (playerscore >= 5 || enemyscore >= 5) {
                    gameState = GAMEOVER;
                }

                break;
            case GAMEOVER:
                BeginDrawing();
                ClearBackground(BLACK);
                char gameOverText[100];
                if (playerscore > enemyscore) {
                    sprintf(gameOverText, "Player Wins!");
                } else {
                    sprintf(gameOverText, "Enemy Wins!");
                }
                DrawText(gameOverText, screenWidth / 2 - MeasureText(gameOverText, 40) / 2, screenHeight / 2 - 20, 40, WHITE);
                sprintf(scoreText, "Player: %d | Enemy: %d", playerscore, enemyscore);
                DrawText(scoreText, screenWidth / 2 - MeasureText(scoreText, 20) / 2, screenHeight / 2 + 30, 20, WHITE);
                if (IsKeyPressed(KEY_SPACE)) {
                    playerscore = 0;
                    enemyscore = 0;
                    ball->x = screenWidth / 2;
                    ball->y = screenHeight / 2;
                    gameState = GAME;
                }
                EndDrawing();
                break;
        }
    }
    CloseWindow();
    free(player);
    free(enemy);
    free(ball);
    return 0;
}