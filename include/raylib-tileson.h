#ifndef RAYLIB_TILESON_H_
#define RAYLIB_TILESON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "raylib.h"

/**
 * The Map data.
 */
typedef struct Map {
    /**
     * The amount of horizontal tiles.
     */
    int width;
    /**
     * The amount of vertical tiles.
     */
    int height;

    /**
     * The width of each tile.
     */
    int tileWidth;

    /**
     * The height of each tile.
     */
    int tileHeight;

    /**
     * The internal pointer to the associated RaylibTilesonData.
     */
    void* data;
} Map;

/**
 * Loads a Map from the given map file data.
 *
 * @param fileData The data for the map file.
 * @param dataSize The size of the data in memory.
 * @param baseDir The base directory that's used to load any external map data.
 */
Map LoadTiledFromMemory(const unsigned char *fileData, int dataSize, const char* baseDir);

/**
 * Loads the given Tiled file.
 *
 * @param fileName The filename of the Tiled map to load.
 */
Map LoadTiled(const char* fileName);

/**
 * Retrieves whether or not the Tiled map was loaded correctly.
 *
 * @param map The given Map to check against.
 * @return True when the given Map was loaded correctly, false otherwise.
 */
bool IsTiledReady(Map map);

/**
 * Draws the given Tiled map.
 */
void DrawTiled(Map map, int posX, int posY, Color tint);

/**
 * Unloads the given map.
 */
void UnloadMap(Map map);

// TODO: Add World support with LoadTiledWorld()

#ifdef __cplusplus
}
#endif

#endif  // RAYLIB_TILESON_H_
