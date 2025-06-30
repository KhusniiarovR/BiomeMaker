#ifndef TILEMAP_CONST_H
#define TILEMAP_CONST_H

// world tilemap
const int worldTileSize = 16;
const int worldSourceTileSize = 16;
const int worldTilesPerRow = 10; // connected with sourceTileSizes tilemap.width = 160 / 16 = 10
const float worldPadding = 0.01f;

// inventory
const int invPadding = 5;
const int invColumns = 10;
const int invSlotSize = 35;
const Vector2 invPosition = {120, 5};

// entity tilemap
const int entityTileSize = 32;
const int entityTilesPerRow = 4;

#endif // TILEMAP_CONST_H