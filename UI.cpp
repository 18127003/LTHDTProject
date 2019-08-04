#include "pch.h"
#include "UI.h"

UI::UI()
{
}

UI::~UI()
{
	G_DestroyTexture(texture);
	texture = NULL;
}
Button::~Button()
{
	clickSound = NULL;
}
void Icon::self_load(G_Font* font, const char* cont, int ft1, int ft2, int ft3, int pos1, int pos2, int pos3, int pos4)
{
	texture = G_LoadFont(font, cont, ft1, ft2, ft3);
	position = { pos1,pos2,pos3,pos4 };
}
void UI::self_load(const char* file, int pos1, int pos2, int pos3, int pos4)
{
	texture = G_LoadImage(file);
	position = { pos1,pos2,pos3,pos4 };
}