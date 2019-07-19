#include "pch.h"
#include "TileTexture.h"


TileTexture::TileTexture()
{
}


TileTexture::~TileTexture()
{
}

void TileTexture::self_draw()
{
	G_Draw(texture, &position);
}
void TileTexture::self_load(const char* file, enum TileType ttype)
{
	texture = G_LoadImage(file);
	position = { 0,0,100,100 };
	type = ttype;
}