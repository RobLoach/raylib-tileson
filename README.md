# raylib-tileson

Use [Tiled](https://www.mapeditor.org) maps in [raylib](https://www.raylib.com/), through [Tileson](https://github.com/SSBMTonberry/tileson).

![raylib-tileson Example Screenshot](example/raylib-tileson-example.png)

## Example

``` c
#include "raylib.h"

#define RAYLIB_TILESON_IMPLEMENTATION
#include "raylib-tileson.h"

int main(int argc, char* argv[]) {
    InitWindow(800, 450, "[raylib-tileson] example");
    SetTargetFPS(60);

    // Load the map
    Map map = LoadTiled("resources/desert.json");

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            // Draw the map
            DrawTiled(map, 0, 0, WHITE);
        }
        EndDrawing();
    }

    // De-Initialization
    UnloadMap(map);

    CloseWindow();
    return 0;
}
```

## Dependencies

- [raylib](https://www.raylib.com/)
- C++17

## API

``` c
Map LoadTiled(const char* fileName);
Map LoadTiledFromMemory(const unsigned char *fileData, int dataSize, const char* baseDir);
bool IsTiledReady(Map map);
void DrawTiled(Map map, int posX, int posY, Color tint);
void UnloadMap(Map map);
```

## Alternatives

While *raylib-tileson* does compile and run from C, it requires compilation with C++17. Mixing C and C++ may not be preferable by all, so there could be a desire to use an alternative...

- [raylib-tmx](https://github.com/RobLoach/raylib-tmx)
- [raylib-tiled](https://github.com/RobLoach/raylib-tiled)

## Development

``` bash
mkdir build
cd build
cmake ..
make
./example/raylib-tileson-example
```

## License

*raylib-tileson* is licensed under the BSD 2-Clause License, see [LICENSE](LICENSE) for more information.
