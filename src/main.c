#include "raylib.h"
#include "paddle.h"
#include "stdlib.h"
#include "stdio.h"
#include "ball.h"
int main(void) {
    int playerscore = 0;
    int enemyscore = 0;
    Paddle *player = (Paddle *)malloc(sizeof(Paddle));
    player->speed = 5;
    player->x = 0;
    player->y = 480;
    Paddle *enemy = (Paddle *)malloc(sizeof(Paddle));
    enemy->speed = 5;
    enemy->x = 580;
    enemy->y = 0;
    Ball *ball = (Ball *)malloc(sizeof(Ball));
    ball->speedx = 1;
    ball->speedy = 2;
    ball->x = 300;
    ball->y = 300;
    if (player == NULL || enemy == NULL || ball == NULL) {
        printf("Failed to allocate memory for the game objects\n");
    }


    const int screenWidth = 600;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Pong: Remastered");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_W) && player->y > 0) {
            player->y -= player->speed;
            
        }
        if (IsKeyDown(KEY_S) && player->y < screenHeight - 120) {
            player->y += player->speed;
        }
        if (IsKeyDown(KEY_UP) && enemy->y > 0) {
            enemy->y -= enemy->speed;

        }
        if (IsKeyDown(KEY_DOWN) && enemy->y < screenHeight - 120) {
            enemy->y += enemy->speed;
        }
        if (ball->x >= 600) {
            ball->speedx *= -1;
        }

        ball->x += ball->speedx;
        ball->y += ball->speedy;

        BeginDrawing();
        ClearBackground(GREEN);
        DrawCircle(ball->x, ball->y, 15, RED);
        DrawRectangle(player->x, player->y, 20, 120, WHITE);
        DrawRectangle(enemy->x, enemy->y, 20, 120, BLACK);

        
        EndDrawing();

    }

    CloseWindow();
    return 0;

}