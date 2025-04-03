#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "ball.h"
#include "string.h"
#include "paddle.h"
#include <time.h>

int main(void) {
    srand(time(NULL));
    int playerscore = 0;
    int enemyscore = 0;
    Paddle *player = (Paddle *)malloc(sizeof(Paddle));
    player->speed = 5;
    player->x = 0;
    player->y = 480;
    player->height = 120;
    player->width = 20;
    Paddle *enemy = (Paddle *)malloc(sizeof(Paddle));
    enemy->x = 580;
    enemy->y = 0;
    enemy->height = 120;
    enemy->width = 20;
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
        MENU,
        GAME,
        GAMEOVER
    } gameState = MENU;

    enum Difficulty {
        EASY,
        MEDIUM,
        HARD
    } difficulty = MEDIUM;

    int selectedDifficulty = 1;

    int bigPaddleTimer = 0;
    int bigPaddleDuration = 300;
    bool bigPaddleActive = false;
    Paddle *bigPaddleUser = NULL;

    Color ballColor = WHITE;
    Color paddleColor = WHITE;
    Color backgroundColor = BLACK;

    int selectedThemeOption = 0;
    int selectedBallColorIndex = 0;
    int selectedPaddleColorIndex = 0;
    int selectedBackgroundColorIndex = 0;
    Color colorOptions[] = {WHITE, RED, BLUE, YELLOW, GREEN, PURPLE};
    const char *colorNames[] = {"White", "Red", "Blue", "Yellow", "Green", "Purple"};

    while (!WindowShouldClose()) {
        switch (gameState) {
            case MENU:
                if (IsKeyPressed(KEY_UP)) {
                    selectedDifficulty = (selectedDifficulty - 1 + 3) % 3;
                }
                if (IsKeyPressed(KEY_DOWN)) {
                    selectedDifficulty = (selectedDifficulty + 1) % 3;
                }
                if (IsKeyPressed(KEY_LEFT)) {
                    selectedThemeOption = (selectedThemeOption - 1 + 3) % 3;
                }
                if (IsKeyPressed(KEY_RIGHT)) {
                    selectedThemeOption = (selectedThemeOption + 1) % 3;
                }
                if (IsKeyPressed(KEY_A)) {
                    switch (selectedThemeOption) {
                        case 0:
                            selectedBallColorIndex = (selectedBallColorIndex - 1 + 6) % 6;
                            break;
                        case 1:
                            selectedPaddleColorIndex = (selectedPaddleColorIndex - 1 + 6) % 6;
                            break;
                        case 2:
                            selectedBackgroundColorIndex = (selectedBackgroundColorIndex - 1 + 6) % 6;
                            break;
                    }
                }
                if (IsKeyPressed(KEY_D)) {
                    switch (selectedThemeOption) {
                        case 0:
                            selectedBallColorIndex = (selectedBallColorIndex + 1) % 6;
                            break;
                        case 1:
                            selectedPaddleColorIndex = (selectedPaddleColorIndex + 1) % 6;
                            break;
                        case 2:
                            selectedBackgroundColorIndex = (selectedBackgroundColorIndex + 1) % 6;
                            break;
                    }
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
                ballColor = colorOptions[selectedBallColorIndex];
                paddleColor = colorOptions[selectedPaddleColorIndex];
                backgroundColor = colorOptions[selectedBackgroundColorIndex];

                BeginDrawing();
                ClearBackground(BLACK);
                DrawText("Pong: Remastered", screenWidth / 2 - MeasureText("Pong: Remastered", 40) / 2, screenHeight / 2 - 180, 40, WHITE);
                DrawText("Difficulty:", screenWidth / 2 - MeasureText("Difficulty:", 20) / 2, screenHeight / 2 - 120, 20, WHITE);

                const char *difficultyNames[] = {"Easy", "Medium", "Hard"};
                for (int i = 0; i < 3; i++) {
                    Color textColor = (i == selectedDifficulty) ? YELLOW : WHITE;
                    DrawText(difficultyNames[i], screenWidth / 2 - MeasureText(difficultyNames[i], 20) / 2, screenHeight / 2 - 95 + (i * 25), 20, textColor);
                }
                DrawText("Theme:", screenWidth / 2 - MeasureText("Theme:", 20) / 2, screenHeight / 2 - 20, 20, WHITE);
                const char *themeOptions[] = {"Ball", "Paddles", "Background"};
                for (int i = 0; i < 3; i++) {
                    Color textColor = (i == selectedThemeOption) ? YELLOW : WHITE;
                    DrawText(themeOptions[i], screenWidth / 2 - MeasureText(themeOptions[i], 20) / 2, screenHeight / 2 + 5 + (i * 25), 20, textColor);
                }
                DrawText(colorNames[selectedBallColorIndex], screenWidth / 2 - MeasureText(colorNames[selectedBallColorIndex], 20) / 2, screenHeight / 2 + 100, 20, ballColor);
                DrawText(colorNames[selectedPaddleColorIndex], screenWidth / 2 - MeasureText(colorNames[selectedPaddleColorIndex], 20) / 2, screenHeight / 2 + 125, 20, paddleColor);
                DrawText(colorNames[selectedBackgroundColorIndex], screenWidth / 2 - MeasureText(colorNames[selectedBackgroundColorIndex], 20) / 2, screenHeight / 2 + 150, 20, backgroundColor);
                DrawText("Press SPACE to start", screenWidth / 2 - MeasureText("Press SPACE to start", 20) / 2, screenHeight / 2 + 180, 20, WHITE);
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
                    if (rand() % 10 == 0 && (playerscore > 2 || enemyscore > 2)) {
                        if (rand() % 2 == 0 && playerscore > 2) {
                            bigPaddleUser = player;
                        } else if (enemyscore > 2) {
                            bigPaddleUser = enemy;
                        }
                        if (bigPaddleUser != NULL) {
                            bigPaddleActive = true;
                            bigPaddleTimer = 0;
                        }
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

                Rectangle playerRect = {player->x, player->y, player->width, player->height};
                Rectangle enemyRect = {enemy->x, enemy->y, enemy->width, enemy->height};
                Vector2 ballPos = {ball->x, ball->y};

                if (CheckCollisionCircleRec(ballPos, 15, playerRect) || CheckCollisionCircleRec(ballPos, 15, enemyRect)) {
                    ball->speedx *= -1;
                }

                BeginDrawing();
                ClearBackground(backgroundColor);
                DrawCircle(ball->x, ball->y, 15, ballColor);
                DrawRectangle(player->x, player->y, player->width, bigPaddleActive && bigPaddleUser == player ? player->height * 2 : player->height, paddleColor);
                DrawRectangle(enemy->x, enemy->y, enemy->width, bigPaddleActive && bigPaddleUser == enemy ? enemy->height * 2 : enemy->height, paddleColor);
                char scoreText[50];
                sprintf(scoreText, "Player: %d | Enemy: %d", playerscore, enemyscore);
                DrawText(scoreText, screenWidth / 2 - MeasureText(scoreText, 20) / 2, 10, 20, paddleColor);
                EndDrawing();

                if (bigPaddleActive) {
                    bigPaddleTimer++;
                    if (bigPaddleTimer > bigPaddleDuration) {
                        bigPaddleActive = false;
                        bigPaddleUser = NULL;
                    }
                }

                if (playerscore >= 8 || enemyscore >= 8) {
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
                    gameState = MENU;
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