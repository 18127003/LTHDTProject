#pragma once
#include"UI.h"
#include "Object.h"
#include "TileTexture.h"
#include "Game.h"
#include <thread>
#include <ctime>
class ConsoleCtrl
{
private:
	Game new_game;
	int startLoop, endLoop;
	int gameEvent;
	G_Font* font;
	char chars[10] = "";
	G_Sound* clickSound = NULL;
	Button TryAgainButton, ClickButton, SoundButton, OffSoundButton, PauseButton, PlayerButton, PlayerChooseButton, QuitButton;
	Icon ScoreText, Logo, CoinText, TopScoreText, GameOver;
public:
	static bool onSound;
	ConsoleCtrl();
	~ConsoleCtrl();
	void loadUI();
	void update_score();
	void UIdraw();
	void pause();
	void game_over();
	void start();
	void choose_player();
	void play();
};

