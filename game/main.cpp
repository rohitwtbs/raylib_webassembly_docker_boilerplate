

// Polished Space Shooter Game in Raylib (C++)
#include "raylib.h"
#include <vector>

struct Bullet {
    Vector2 pos;
    bool active;
};

struct Enemy {
    Vector2 pos;
    bool active;
    float speed;
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Space Shooter WebAssembly");
    SetTargetFPS(60);

    // Player
    Vector2 playerPos = {screenWidth/2.0f, screenHeight - 40};
    const float playerSpeed = 6.0f;
    int playerLives = 3;
    bool gameOver = false;

    // Bullets
    std::vector<Bullet> bullets;
    const float bulletSpeed = 10.0f;
    int shootTimer = 0;

    // Enemies
    std::vector<Enemy> enemies;
    int wave = 1;
    int enemiesPerWave = 6;
    int enemiesKilled = 0;

    int score = 0;

    // Sound (optional, will not error if not supported)
    InitAudioDevice();
    Sound shootSound = LoadSound("shoot.wav"); // Optional: add shoot.wav to game folder
    Sound hitSound = LoadSound("hit.wav");   // Optional: add hit.wav to game folder

    // Spawn enemies
    auto spawnEnemies = [&](int count, int waveNum) {
        enemies.clear();
        for (int i = 0; i < count; i++) {
            float speed = 2.0f + waveNum * 0.5f + GetRandomValue(0, 10) * 0.05f;
            enemies.push_back({Vector2{(float)(GetRandomValue(40, screenWidth-40)), (float)GetRandomValue(-200, -40)}, true, speed});
        }
    };
    spawnEnemies(enemiesPerWave, wave);

    while (!WindowShouldClose()) {
        if (!gameOver) {
            // Player movement (smooth)
            if (IsKeyDown(KEY_LEFT) && playerPos.x > 40) playerPos.x -= playerSpeed;
            if (IsKeyDown(KEY_RIGHT) && playerPos.x < screenWidth-40) playerPos.x += playerSpeed;

            // Shooting (rate limit)
            if (shootTimer > 0) shootTimer--;
            if (IsKeyDown(KEY_SPACE) && shootTimer == 0) {
                bullets.push_back({Vector2{playerPos.x, playerPos.y-20}, true});
                PlaySound(shootSound);
                shootTimer = 10;
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
                    e.pos.y += e.speed;
                    e.pos.x += sinf(e.pos.y * 0.03f) * 2.0f; // Sway movement
                    if (e.pos.y > screenHeight+40) {
                        e.active = false;
                        playerLives--;
                        if (playerLives <= 0) gameOver = true;
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
                        e.active = false;
                        PlaySound(hitSound);
                        score += 10;
                        enemiesKilled++;
                    }
                }
            }

            // Next wave
            if (enemiesKilled >= enemiesPerWave) {
                wave++;
                enemiesPerWave += 2;
                enemiesKilled = 0;
                spawnEnemies(enemiesPerWave, wave);
            }

            // Player-enemy collision
            for (auto &e : enemies) {
                if (!e.active) continue;
                if (CheckCollisionCircles(playerPos, 20, e.pos, 20)) {
                    e.active = false;
                    playerLives--;
                    PlaySound(hitSound);
                    if (playerLives <= 0) gameOver = true;
                }
            }
        } else {
            // Restart game
            if (IsKeyPressed(KEY_ENTER)) {
                playerPos = {screenWidth/2.0f, screenHeight - 40};
                playerLives = 3;
                score = 0;
                wave = 1;
                enemiesPerWave = 6;
                enemiesKilled = 0;
                bullets.clear();
                spawnEnemies(enemiesPerWave, wave);
                gameOver = false;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        // Player
        if (!gameOver) {
            DrawCircleV(playerPos, 20, BLUE);
            DrawTriangle(
                Vector2{playerPos.x, playerPos.y-20},
                Vector2{playerPos.x-15, playerPos.y+20},
                Vector2{playerPos.x+15, playerPos.y+20},
                SKYBLUE
            );
        }

        // Bullets
        for (auto &b : bullets) {
            if (b.active) DrawCircleV(b.pos, 8, YELLOW);
        }

        // Enemies
        for (auto &e : enemies) {
            if (e.active) DrawCircleV(e.pos, 20, RED);
        }

        // UI
        DrawText(TextFormat("Score: %d", score), 10, 10, 24, WHITE);
        DrawText(TextFormat("Lives: %d", playerLives), 10, 40, 24, LIGHTGRAY);
        DrawText(TextFormat("Wave: %d", wave), 10, 70, 24, SKYBLUE);
        DrawText("SPACE: Shoot | LEFT/RIGHT: Move", 10, screenHeight-30, 20, LIGHTGRAY);

        if (gameOver) {
            DrawText("GAME OVER", screenWidth/2-100, screenHeight/2-40, 40, RED);
            DrawText("Press ENTER to Restart", screenWidth/2-140, screenHeight/2+10, 30, WHITE);
        }

        EndDrawing();
    }
    UnloadSound(shootSound);
    UnloadSound(hitSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
