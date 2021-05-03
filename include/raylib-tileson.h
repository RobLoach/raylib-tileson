#ifndef RAYLIB_TILESON_H_
#define RAYLIB_TILESON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "raylib.h"

typedef struct Map {
    void* data;
} Map;

Map LoadTiledFromMemory(const unsigned char *fileData, int dataSize, const char* baseDir);
Map LoadTiled(const char* fileName);
bool IsTiledReady(Map map);
void DrawTiled(Map map, int posX, int posY, Color tint);
void UnloadMap(Map map);

// TODO: Add World support with LoadTiledWorld()

#ifdef __cplusplus
}
#endif

#endif  // RAYLIB_TILESON_H_
