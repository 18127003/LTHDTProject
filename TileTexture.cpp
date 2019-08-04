#include "pch.h"
#include "TileTexture.h"


TileTexture::TileTexture()
{
	texture.reserve(3);
}


TileTexture::~TileTexture()
{
	vector <G_Texture*>().swap(texture);
}

void TileTexture::self_draw()
{
	G_Draw(texture[skin], &position);
}
void TileTexture::self_load(const char* file, enum TileType ttype)
{
	texture.push_back(G_LoadImage(file));
	position = { 0,0,100,100 };
	type = ttype;
}