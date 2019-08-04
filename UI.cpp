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
void Icon::self_load(const char* file, int pos1, int pos2, int pos3, int pos4)
{
	UI::self_load(file, pos1, pos2, pos3, pos4);
}
int UI::getPos(char dim)
{
	if (dim == 'y') return position.y;
	return position.x;
}
void Icon::self_update(G_Font* font, char* chars)
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
bool Button::clickOnButton(int& gameEvent, bool onSound)
{
	SDL_Point mouse_point = { G_motion.x , G_motion.y };
	if (gameEvent == G_MOUSEBUTTONDOWN && G_Mouse == G_BUTTON_LEFT && SDL_PointInRect(&mouse_point, &position))
	{
		if (onSound == true) G_PlaySound(clickSound, 0);
		return true;
	}
	return false;
}