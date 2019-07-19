#pragma once
#include "Genio.h"
enum TileType { GRASS, WATER, ROAD, RAIL, TREE, GATE };
class TileTexture
{
protected:
	TileType type;
	G_Rect position;
	G_Texture* texture;
public:
	
	TileTexture();
	~TileTexture();
	void self_draw();
	void self_load(const char* file, enum TileType ttype);
	friend class Game;
};

