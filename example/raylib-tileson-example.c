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
