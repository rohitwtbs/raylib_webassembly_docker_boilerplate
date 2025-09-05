
// Simple Space Shooter Game in Raylib (C++)
#include "raylib.h"
#include <vector>

struct Bullet {
    Vector2 pos;
    bool active;
};

struct Enemy {
    Vector2 pos;
    bool active;
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Space Shooter WebAssembly");
    SetTargetFPS(60);

    // Player
    Vector2 playerPos = {screenWidth/2.0f, screenHeight - 40};
    const float playerSpeed = 5.0f;

    // Bullets
    std::vector<Bullet> bullets;
    const float bulletSpeed = 8.0f;

    // Enemies
    std::vector<Enemy> enemies;
    const int maxEnemies = 5;
    const float enemySpeed = 2.0f;

    int score = 0;

    // Spawn initial enemies
    for (int i = 0; i < maxEnemies; i++) {
        enemies.push_back({Vector2{(float)(GetRandomValue(40, screenWidth-40)), (float)GetRandomValue(-150, -40)}, true});
    }

    while (!WindowShouldClose()) {
        // Player movement
        if (IsKeyDown(KEY_LEFT) && playerPos.x > 40) playerPos.x -= playerSpeed;
        if (IsKeyDown(KEY_RIGHT) && playerPos.x < screenWidth-40) playerPos.x += playerSpeed;

        // Shooting
        if (IsKeyPressed(KEY_SPACE)) {
            bullets.push_back({Vector2{playerPos.x, playerPos.y-20}, true});
        }

        // Update bullets
        for (auto &b : bullets) {
            if (b.active) {
                b.pos.y -= bulletSpeed;
                if (b.pos.y < 0) b.active = false;
            }
        }

        // Update enemies
        for (auto &e : enemies) {
            if (e.active) {
                e.pos.y += enemySpeed;
                if (e.pos.y > screenHeight+40) {
                    e.pos = { (float)GetRandomValue(40, screenWidth-40), (float)GetRandomValue(-150, -40) };
                }
            }
        }

        // Bullet-enemy collision
        for (auto &b : bullets) {
            if (!b.active) continue;
            for (auto &e : enemies) {
                if (!e.active) continue;
                if (CheckCollisionCircles(b.pos, 8, e.pos, 20)) {
                    b.active = false;
                    e.pos = { (float)GetRandomValue(40, screenWidth-40), (float)GetRandomValue(-150, -40) };
                    score++;
                }
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        // Player
        DrawCircleV(playerPos, 20, BLUE);
        DrawTriangle(
            Vector2{playerPos.x, playerPos.y-20},
            Vector2{playerPos.x-15, playerPos.y+20},
            Vector2{playerPos.x+15, playerPos.y+20},
            SKYBLUE
        );

        // Bullets
        for (auto &b : bullets) {
            if (b.active) DrawCircleV(b.pos, 8, YELLOW);
        }

        // Enemies
        for (auto &e : enemies) {
            if (e.active) DrawCircleV(e.pos, 20, RED);
        }

        DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
        DrawText("SPACE: Shoot | LEFT/RIGHT: Move", 10, screenHeight-30, 20, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
