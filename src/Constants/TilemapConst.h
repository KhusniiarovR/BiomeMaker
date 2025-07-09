#ifndef TILEMAP_CONST_H
#define TILEMAP_CONST_H

// world tilemap
const int worldTileSize = 16;       // every tile in the world
const int worldSourceTileSize = 16; // tile size in the tilemap
const int worldTilesPerRow = 10;    // connected with sourceTileSizes tilemap.width = 160 / 16 = 10
const float worldPadding = 0.01f;   // to avoid bugging lines

// inventory
const int invPadding = 5;             // horizontal space between slots
const int invColumns = 10;            // number of slots in one row
const int invRows = 2;                // number of slots in one column
const int invSlotSize = 32;           // size of 1 slot
const Vector2 invPosition = {120, 5}; // coord of the first slot
const int slotCount = 20;

// entity tilemap
const int entityTileSize = 32;   // tile size in the tilemap
const int entityTilesPerRow = 4; // number of tiles in one row

#endif // TILEMAP_CONST_H