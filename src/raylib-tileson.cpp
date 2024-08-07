#include <string>
#include <vector>
#include <map>

#include "raylib.h"
#include "raylib-tileson.h"
#include "tileson.hpp"

// TODO: Add World support with LoadTiledWorld()

class RaylibTilesonData {
    public:
        std::map<std::string, Texture> textures;
        tson::Map* map;
};

Color ColorFromTiledColor(tson::Colori color) {
    Color output;
    output.r = color.r;
    output.g = color.g;
    output.b = color.b;
    output.a = color.a;
    return output;
}

Rectangle RectangleFromTiledRectangle(tson::Rect rect) {
    Rectangle output;
    output.x = rect.x;
    output.y = rect.y;
    output.width = rect.width;
    output.height = rect.height;
    return output;
}

void LoadTiledImage(RaylibTilesonData* data, const char* baseDir, const std::string& imagepath, tson::Colori transparentColor) {
    // Only load the image if it's not yet loaded.
    if (data->textures.count(imagepath) == 0) {
        const char* image;
        if (TextLength(baseDir) > 0) {
            image = TextFormat("%s/%s", baseDir, imagepath.c_str());
        } else {
            image = imagepath.c_str();
        }

        Image loadedImage = LoadImage(image);
        ImageFormat(&loadedImage, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

        Color trans = ColorFromTiledColor(transparentColor);
        ImageColorReplace(&loadedImage, trans, BLANK);

        // TODO: Support image tint color.

        Texture texture = LoadTextureFromImage(loadedImage);
        UnloadImage(loadedImage);
        data->textures[imagepath] = texture;
    }
}

Map LoadTiledFromMemory(const unsigned char *fileData, int dataSize, const char* baseDir) {
    tson::Tileson t;
    auto map = t.parse(fileData, dataSize);

    if (map == nullptr) {
        TraceLog(LOG_ERROR, "TILESON: Error parsing the given map");
        struct Map output;
        output.data = NULL;
        return output;
    }

    // Check if it parsed okay.
    if(map->getStatus() != tson::ParseStatus::OK) {
        TraceLog(LOG_ERROR, "TILESON: Map parse error: %s", map->getStatusMessage().c_str());
        struct Map output;
        output.data = NULL;
        return output;
    }

    // Only support Ortogonal
    if (map->getOrientation() != "orthogonal") {
        TraceLog(LOG_ERROR, "TILESON: Only support for orthogonal maps");
        struct Map output;
        output.data = NULL;
        return output;
    }

    // Load all the images
    RaylibTilesonData* data = new RaylibTilesonData();
    for (const auto& layer : map->getLayers()) {
        if (layer.getType() == tson::LayerType::ImageLayer) {
            LoadTiledImage(data, baseDir, layer.getImage(), layer.getTransparentColor());
        }
    }
    for (const auto& tileset : map->getTilesets()) {
        LoadTiledImage(data, baseDir, tileset.getImage().string(), tileset.getTransparentColor());
    }

    // Save the Map.
    struct Map output;
    output.width = map->getSize().x;
    output.height = map->getSize().y;
    output.tileWidth = map->getTileSize().x;
    output.tileHeight = map->getTileSize().y;

    data->map = map.release();
    output.data = data;

    TraceLog(LOG_INFO, "TILESON: Map parsed successfully");
    return output;
}

Map LoadTiled(const char* fileName) {
    const char* baseDir = GetDirectoryPath(fileName);
    tson::Tileson t;
    auto map = t.parse(fileName);

    if (map == nullptr) {
        TraceLog(LOG_ERROR, "TILESON: Error parsing the given map");
        struct Map output;
        output.data = NULL;
        return output;
    }

    // Check if it parsed okay.
    if(map->getStatus() != tson::ParseStatus::OK) {
        TraceLog(LOG_ERROR, "TILESON: Map parse error: %s", map->getStatusMessage().c_str());
        struct Map output;
        output.data = NULL;
        return output;
    }

    // Only support Ortogonal
    if (map->getOrientation() != "orthogonal") {
        TraceLog(LOG_ERROR, "TILESON: Only support for orthogonal maps");
        struct Map output;
        output.data = NULL;
        return output;
    }

    // Load all the images
    RaylibTilesonData* data = new RaylibTilesonData();
    for (const auto& layer : map->getLayers()) {
        if (layer.getType() == tson::LayerType::ImageLayer) {
            LoadTiledImage(data, baseDir, layer.getImage(), layer.getTransparentColor());
        }
    }
    for (const auto& tileset : map->getTilesets()) {
        LoadTiledImage(data, baseDir, tileset.getImage().string(), tileset.getTransparentColor());
    }

    // Save the Map.
    struct Map output;
    output.width = map->getSize().x;
    output.height = map->getSize().y;
    output.tileWidth = map->getTileSize().x;
    output.tileHeight = map->getTileSize().y;

    data->map = map.release();
    output.data = data;

    TraceLog(LOG_INFO, "TILESON: Map parsed successfully");
    return output;
}

bool IsTiledReady(Map map) {
    return map.data != NULL;
}

void DrawTileLayer(tson::Layer& layer, RaylibTilesonData* data, int posX, int posY, Color tint) {
    for (auto& [pos, tileObject] : layer.getTileObjects()) {
        const tson::Tileset *tileset = tileObject.getTile()->getTileset();
        std::string image = tileset->getImage().string();
        if (data->textures.count(image) == 0) {
            continue;
        }

        Texture texture = data->textures[image];
        Rectangle drawRect = RectangleFromTiledRectangle(tileObject.getDrawingRect());
        tson::Vector2f position = tileObject.getPosition();
        Vector2 drawPos = {position.x + posX, position.y + posY};
        DrawTextureRec(texture, drawRect, drawPos, tint);
    }
}

void DrawImageLayer(tson::Layer& layer, RaylibTilesonData* data, int posX, int posY, Color tint) {
    auto image = layer.getImage();
    if (data->textures.count(image) == 0) {
        return;
    }

    Texture texture = data->textures[image];
    auto offset = layer.getOffset();

    DrawTexture(texture, posX + offset.x, posY + offset.y, tint);
}

void DrawObjectLayer(tson::Layer &layer, RaylibTilesonData* data, int posX, int posY, Color tint) {

    //tson::Tileset* tileset = m_map->getTileset("demo-tileset");
    auto *map = layer.getMap();
    for(auto &obj : layer.getObjects()) {
        switch(obj.getObjectType()) {
            case tson::ObjectType::Text: {
                if (obj.isVisible()) {
                    auto textObj = obj.getText();
                    const char* text = textObj.text.c_str();
                    auto color = ColorFromTiledColor(textObj.color);
                    auto pos = obj.getPosition();

                    // TODO: Find the font size.
                    DrawText(text, posX + pos.x, posY + pos.y, 16, color);
                }
            } break;

            default:
                break;
        }
    }
}

void DrawLayer(tson::Layer &layer, RaylibTilesonData* data, int posX, int posY, Color tint) {
    switch (layer.getType()) {
        case tson::LayerType::TileLayer:
            DrawTileLayer(layer, data, posX, posY, tint);
            break;

        case tson::LayerType::ObjectGroup:
            DrawObjectLayer(layer, data, posX, posY, tint);
            break;

        case tson::LayerType::ImageLayer:
            DrawImageLayer(layer, data, posX, posY, tint);
            break;

        case tson::LayerType::Group:
            for(auto &l : layer.getLayers()) {
                DrawLayer(l, data, posX, posY, tint);
            }
            break;

        default:
            TraceLog(LOG_TRACE, "TILESON: Unsupported layer type");
            break;
    }
}

void DrawTiled(Map map, int posX, int posY, Color tint) {
    RaylibTilesonData* data = (RaylibTilesonData*)map.data;
    if (data == NULL) {
        TraceLog(LOG_WARNING, "TILESON: Cannot draw empty map");
        return;
    }
    tson::Map* tsonMap = data->map;
    auto &layers = tsonMap->getLayers();

    // TODO: Draw the background color.

    for (auto &layer : layers) {
        DrawLayer(layer, data, posX, posY, tint);
    }
}

void UnloadMap(Map map) {
    RaylibTilesonData* data = (RaylibTilesonData*)map.data;
    if (data != NULL) {
        // Unload Tileson
        if (data->map != NULL) {
            delete data->map;
        }

        // Unload all the Textures
        for (const auto& [name, texture] : data->textures) {
            UnloadTexture(texture);
        }

        // Finally, delete the internal data
        delete data;
    };
}
