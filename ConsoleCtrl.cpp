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


void ConsoleCtrl::loadUI()
{
	ScoreText.self_load(font, "0", 255, 255, 255, 10, 10, 31, 42);
	TopScoreText.self_load(font, "TOP 0", 255, 255, 255, 10, 60, 81, 52);
	CoinText.self_load(font, "0", 253, 230, 75, new_game.windowPos.w - 70, 8, 20, 35);
	Logo.self_load("assets/image/logo.png", (new_game.windowPos.w - 764) / 2, (new_game.windowPos.h - 550) / 3, 764, 350);
	ClickButton.self_load("assets/image/click.png", (new_game.windowPos.w + 275) / 2, Logo.getPos('y') + 260, 100, 172, clickSound);
	TryAgainButton.self_load("assets/image/again.png", (new_game.windowPos.w - 177) / 2, (new_game.windowPos.h / 2) + 150, 177, 89, clickSound);
	PauseButton.self_load("assets/image/pause.png", new_game.windowPos.w - 70, 70, 50, 50, clickSound);
	PlayerChooseButton.self_load("assets/image/settings.png", 20, new_game.windowPos.h - 120, 125, 100, clickSound);
	GameOver.self_load("assets/image/gameover.png", (new_game.windowPos.w - 764) / 2, (new_game.windowPos.h - 144) / 3 - 75, 764, 450);
	QuitButton.self_load("assets/image/quit.png", new_game.windowPos.w - 100, new_game.windowPos.h - 120, 100, 100, clickSound);
	SoundButton.self_load("assets/image/onsound.png", 125, new_game.windowPos.h - 120, 100, 100, clickSound);
	OffSoundButton.self_load("assets/image/offsound.png", 148, new_game.windowPos.h - 120, 100, 100, clickSound);
	cout << "Load UI complete" << endl;
}