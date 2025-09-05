// Simple Raylib C++ WebAssembly sample game
#include "raylib.h"

int main() {
    InitWindow(800, 450, "Raylib WebAssembly Sample");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello, WebAssembly!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
