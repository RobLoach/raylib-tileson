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
     *
     * This is referenced as a void* so that it can be used from C, even though it requires reference to a C++ class.
     *
     * @see RaylibTilesonData
     */
    void* data;
} Map;

/**
 * Loads a Map from the given map file data.
 *
 * @param fileData The data for the map file.
 * @param dataSize The size of the data in memory.
 * @param baseDir The base directory that's used to load any external map data.
 *
 * @return The loaded Map data.
 */
Map LoadTiledFromMemory(const unsigned char *fileData, int dataSize, const char* baseDir);

/**
 * Loads the given Tiled file.
 *
 * @param fileName The filename of the Tiled map to load.
 *
 * @return The loaded Map data.
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
 *
 * @param map The map to draw.
 * @param posX The X position of where to draw the map.
 * @param posY The Y position of where to draw the map.
 * @param tint The color of which to tint. Use WHITE if you're unsure.
 */
void DrawTiled(Map map, int posX, int posY, Color tint);

/**
 * Unloads the given map.
 *
 * @param map The map data to unload.
 */
void UnloadMap(Map map);

#ifdef __cplusplus
}
#endif

#endif  // RAYLIB_TILESON_H_
