#include "ConsoleCtrl.h"

bool ConsoleCtrl::onSound = true;
ConsoleCtrl::ConsoleCtrl()
{
	G_InitSDL();
	new_game.windowPos = { SDL_WINDOWPOS_UNDEFINED ,SDL_WINDOWPOS_UNDEFINED ,1100,800 };
	G_CreateWindow("CROSSY ROAD For Desktop", new_game.windowPos, 255, 255, 255);
	font = G_OpenFont("assets/font/editundo.ttf", 72);
	clickSound = G_LoadSound("assets/sound/click.wav");
	loadUI();
}


ConsoleCtrl::~ConsoleCtrl()
{

	TTF_CloseFont(font);
	G_FreeSound(clickSound);
	G_QuitSDL();
}