#include <string>
#include <vector>
#include <map>

#include "raylib.h"
#include "raylib-tileson.h"
#include "tileson.hpp"

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
    if (data->textures.count(imagepath) == 0) {
        const char* image;
        if (TextLength(baseDir) > 0) {
            image = TextFormat("%s/%s", baseDir, imagepath.c_str());
        } else {
            image = imagepath.c_str();
        }

        Image loadedImage = LoadImage(image);
        ImageFormat(&loadedImage, PIXELFORMAT_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

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

    // Check if it parsed okay.
    if(map->getStatus() != tson::ParseStatus::OK) {
        TraceLog(LOG_WARNING, "TILESON: Map parse error: %s", map->getStatusMessage().c_str());
        struct Map output;
        output.data = NULL;
        return output;
    }

    // Only support Ortogonal
    if (map->getOrientation() != "orthogonal") {
        TraceLog(LOG_WARNING, "TILESON: Only support for orthogonal maps");
        struct Map output;
        output.data = NULL;
        return output;
    }

    // Load all the images
    RaylibTilesonData* data = new RaylibTilesonData();
    for (const auto& layer : map->getLayers()) {
        if (layer.getType() == tson::LayerType::ImageLayer) {
            LoadTiledImage(data, baseDir, layer.getImage(), layer.getTransparentcolor());
        }
    }
    for (const auto& tileset : map->getTilesets()) {
        LoadTiledImage(data, baseDir, tileset.getImage(), tileset.getTransparentColor());
    }

    // Save the Map pointer.
    data->map = map.release();
    struct Map output;
    output.data = data;

    TraceLog(LOG_INFO, "TILESON: Map parsed successfully");
    return output;
}

Map LoadTiled(const char* fileName) {
    unsigned int bytesRead;
    unsigned char* data = LoadFileData(fileName, &bytesRead);
    if (data == NULL || bytesRead == 0) {
        TraceLog(LOG_ERROR, "TILESON: Failed to read file data");
        struct Map output;
        output.data = NULL;
        return output;
    }

    const char* baseDir = GetDirectoryPath(fileName);
    Map map = LoadTiledFromMemory(data, bytesRead, baseDir);
    UnloadFileData(data);
    return map;
}

bool IsTiledReady(Map map) {
    return map.data != NULL;
}

void drawTileLayer(tson::Layer& layer, RaylibTilesonData* data, int posX, int posY, Color tint) {
    for (const auto& [pos, tileObject] : layer.getTileObjects()) {
        tson::Tile* tile = tileObject.getTile();
        tson::Tileset *tileset = tileObject.getTile()->getTileset();
        std::string image = tileset->getImage();
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

void drawImageLayer(tson::Layer& layer, RaylibTilesonData* data, int posX, int posY, Color tint) {
    auto image = layer.getImage();
    if (data->textures.count(image) == 0) {
        return;
    }

    Texture texture = data->textures[image];
    auto offset = layer.getOffset();

    DrawTexture(texture, posX + offset.x, posY + offset.y, tint);
}

void drawObjectLayer(tson::Layer &layer, RaylibTilesonData* data, int posX, int posY, Color tint) {

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

void drawLayer(tson::Layer &layer, RaylibTilesonData* data, int posX, int posY, Color tint) {
    switch (layer.getType()) {
        case tson::LayerType::TileLayer:
            drawTileLayer(layer, data, posX, posY, tint);
            break;

        case tson::LayerType::ObjectGroup:
            drawObjectLayer(layer, data, posX, posY, tint);
            break;

        case tson::LayerType::ImageLayer:
            drawImageLayer(layer, data, posX, posY, tint);
            break;

        case tson::LayerType::Group:
            for(auto &l : layer.getLayers())
                drawLayer(l, data, posX, posY, tint);
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
    std::vector<tson::Layer> layers = tsonMap->getLayers();

    // TODO: Draw the background color.

    for (auto &layer : layers) {
        drawLayer(layer, data, posX, posY, tint);
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
