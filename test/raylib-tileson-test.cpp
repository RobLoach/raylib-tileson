#include "raylib.h"
#include "raylib-tileson.h"
#include "raylib-assert.h"

int main(int argc, char *argv[]) {
    // Initialization
    SetTraceLogLevel(LOG_ALL);
    TraceLog(LOG_INFO, "================================");
    TraceLog(LOG_INFO, "raylib-tileson-test");
    TraceLog(LOG_INFO, "================================");

    InitWindow(640, 480, "raylib-tileson-tests");
    Assert(IsWindowReady());

    // Make sure we're running in the correct directory.
    Assert(argc > 0);
    const char* dir = GetDirectoryPath(argv[0]);
    Assert(ChangeDirectory(dir));

    {
        Map map = LoadTiled("resources/desert.json");

        Assert(IsTiledReady(map));
        AssertEqual(map.width, 40);

        BeginDrawing();
        DrawTiled(map, 10, 10, WHITE);
        EndDrawing();

        UnloadMap(map);
    }

    {
        int bytesRead;
        unsigned char* data = LoadFileData("resources/desert.json", &bytesRead);
        Map map = LoadTiledFromMemory(data, bytesRead, "resources");

        Assert(IsTiledReady(map));
        AssertEqual(map.width, 40);

        UnloadMap(map);
    }

    CloseWindow();
    TraceLog(LOG_INFO, "================================");
    TraceLog(LOG_INFO, "raylib-tileson tests succesful");
    TraceLog(LOG_INFO, "================================");

    return 0;
}
