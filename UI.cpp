#include "pch.h"
#include "UI.h"


UI::UI()
{
}


UI::~UI()
{
}
void UI::self_load(G_Font* font, const char* cont, int ft1, int ft2, int ft3, int pos1, int pos2, int pos3, int pos4)
{
	texture = G_LoadFont(font, cont, ft1, ft2, ft3);
	position = { pos1,pos2,pos3,pos4 };
}
void UI::self_load(const char*file, int pos1, int pos2, int pos3, int pos4)
{
	texture = G_LoadImage(file);
	position = { pos1,pos2,pos3,pos4 };
}
int UI::getPos(char dim)
{
	if (dim == 'y') return position.y;
	return position.x;
}
void UI::self_update(G_Font *font, char* chars)
{
	G_DestroyTexture(texture);
	texture = G_LoadFont(font, chars, 255, 255, 255);
}
void UI::self_draw()
{
	G_Draw(texture, &position);
}
void UI::self_draw(G_Rect pos)
{
	G_Draw(texture, &pos);
}
void UI::settexture(G_Texture* txture)
{
	texture = txture;
}