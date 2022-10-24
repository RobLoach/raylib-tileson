#include "raylib.h"

#define RAYLIB_TILESON_IMPLEMENTATION
#include "raylib-tileson.h"

int main(int argc, char* argv[]) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    Vector2 position = {0, 0};

    // Make sure we're running in the correct directory.
    const char* dir = GetDirectoryPath(argv[0]);
    if (!ChangeDirectory(dir)) {
        TraceLog(LOG_ERROR, "Unable to change directory to: %s", dir);
        return 1;
    }

    InitWindow(screenWidth, screenHeight, "[raylib-tileson] example");

    SetTargetFPS(60);

    Map map = LoadTiled("resources/desert.json");     // Load the map
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        //--------------------------------------------------------------------------------------

        // Move around with the keyboard
        if (IsKeyDown(KEY_LEFT)) {
            position.x += 2;
        }
        if (IsKeyDown(KEY_UP)) {
            position.y += 2;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            position.x -= 2;
        }
        if (IsKeyDown(KEY_DOWN)) {
            position.y -= 2;
        }

        // Move based on mouse left click
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            position.x += delta.x;
            position.y += delta.y;
        }

        // Keep the map within screen bounds
        if (position.x > 0) {
            position.x = 0;
        }
        if (position.y > 0) {
            position.y = 0;
        }
        if (position.x - GetScreenWidth() < -map.width * map.tileWidth) {
            position.x = -map.width * map.tileWidth + GetScreenWidth();
        }
        if (position.y - GetScreenHeight() < -map.height * map.tileHeight) {
            position.y = -map.height * map.tileHeight + GetScreenHeight();
        }

        // Draw
        //----------------------------------------------------------------------------------

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawTiled(map, position.x, position.y, WHITE);
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMap(map);

    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
