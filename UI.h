#pragma once
#include "Genio.h"
class UI
{
private:
	G_Rect position;
	G_Texture* texture;
public:

	UI();
	~UI();
	friend class Game;
	friend class ConsoleCtrl;
	void self_load(G_Font* font,const char* cont, int ft1, int ft2, int ft3, int pos1, int pos2, int pos3, int pos4);
	void self_load(const char*file, int pos1, int pos2, int pos3, int pos4);
	int getPos(char dim);
	void self_update(G_Font* font, char* chars);
	void self_draw();
	void self_draw(G_Rect pos);
	void settexture(G_Texture* txture);
	G_Texture* Get_texture() { return texture; }
};

