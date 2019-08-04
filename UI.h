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
class Icon :public UI
{
public:
	void self_load(const char* file, int pos1, int pos2, int pos3, int pos4);
	void self_load(G_Font* font, const char* cont, int ft1, int ft2, int ft3, int pos1, int pos2, int pos3, int pos4);
	void self_update(G_Font* font, char* chars);
};
class Button :public UI
{
private:
	G_Sound* clickSound = NULL;
public:
	~Button();
	bool clickOnButton(int& gameEvent, bool onSound);
	void self_load(const char* file, int pos1, int pos2, int pos3, int pos4, G_Sound* sound);
};