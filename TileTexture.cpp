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
