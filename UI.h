#pragma once
#include "SDLexe.h"
class UI
{
protected:
	G_Rect position;
	G_Texture* texture;
public:

	UI();
	virtual ~UI();
	friend class ConsoleCtrl;
	virtual void self_load(const char*file, int pos1, int pos2, int pos3, int pos4);
	int getPos(char dim);
	void self_draw();
	void self_draw(G_Rect pos);
	void settexture(G_Texture* txture);
	G_Texture* Get_texture() { return texture; }
};