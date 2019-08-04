#pragma once
#include <vector>
#include "SDLexe.h"
using namespace std;
enum TileType { GRASS, WATER, ROAD, RAIL, TREE, GATE };
class TileTexture
{
protected:
	TileType type;
	G_Rect position;
	vector<G_Texture*> texture;
	int skin = 0;
public:

	TileTexture();
	~TileTexture();
	void self_draw();
	void self_load(const char* file, enum TileType ttype);
	friend class Game;
};

