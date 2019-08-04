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

void ConsoleCtrl::update_score()
{
	string all = to_string(new_game.getmaxScore());
	strcpy_s(chars, 10, all.c_str());
	ScoreText.self_update(font, chars);
	all = to_string(new_game.getCoin());
	strcpy_s(chars, 10, all.c_str());
	CoinText.self_update(font, chars);
}

void ConsoleCtrl::UIdraw()
{
	ScoreText.self_draw();
	CoinText.self_draw();
	QuitButton.self_draw();
}
void ConsoleCtrl::pause()
{
	G_Rect pauseRect = { (new_game.windowPos.w - 402) / 2,(new_game.windowPos.h - 512) / 2,402,512 };
	PauseButton.self_draw(pauseRect);
	if (QuitButton.clickOnButton(gameEvent, onSound)) new_game.SetState(QUIT);
	else if (gameEvent == G_MOUSEBUTTONDOWN && G_Mouse == G_BUTTON_LEFT)
		new_game.SetState(PLAY);
}

void ConsoleCtrl::game_over()
{
	GameOver.self_draw();
	TryAgainButton.self_draw();
	G_DestroyTexture(TopScoreText.texture);
	string all = "TOP " + to_string(new_game.NewTopScore());
	strcpy_s(chars, 10, all.c_str());
	TopScoreText.texture = G_LoadFont(font, chars, 255, 255, 255);
	TopScoreText.self_draw();
	new_game.SaveScoreToFile("topScore.txt");
	if (TryAgainButton.clickOnButton(gameEvent, onSound))
	{
		new_game.Reload();
	}
	if (QuitButton.clickOnButton(gameEvent, onSound)) new_game.SetState(QUIT);
}

void ConsoleCtrl::start()
{
	Logo.self_draw();
	ClickButton.self_draw();
	PlayerChooseButton.self_draw();
	if (onSound == true) SoundButton.self_draw();
	else OffSoundButton.self_draw();
	if (PlayerChooseButton.clickOnButton(gameEvent, onSound))
		new_game.SetState(CHOOSE_PLAYER);
	else if (OffSoundButton.clickOnButton(gameEvent, onSound) || SoundButton.clickOnButton(gameEvent, onSound));
	else if (QuitButton.clickOnButton(gameEvent, onSound)) new_game.SetState(QUIT);
	else if (gameEvent == G_MOUSEBUTTONDOWN && G_Mouse == G_BUTTON_LEFT)
	{
		new_game.SetState(PLAY);
	}
}

void ConsoleCtrl::choose_player()
{
	int num = 3;
	PlayerButton.position = { (new_game.windowPos.w - (num * 150 - 50)) / 2,(new_game.windowPos.h - 100) / 2,100,100 };
	for (int i = 0; i < num; i++)
	{
		PlayerButton.settexture(new_game.GetPlayerTexture(i));
		if (PlayerButton.clickOnButton(gameEvent, onSound))
		{
			new_game.SetPlayerTexture(PlayerButton.Get_texture());
			new_game.SetState(START);
			break;
		}
		SDL_SetRenderDrawColor(renderer, 0, 196, 255, 255);
		SDL_RenderFillRect(renderer, &PlayerButton.position);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &PlayerButton.position);
		PlayerButton.self_draw();
		PlayerButton.position.x += 150;
	}
}
void ConsoleCtrl::play()
{
	new_game.PlayerMove();


	PauseButton.self_draw();
	if (PauseButton.clickOnButton(gameEvent, onSound)) new_game.SetState(PAUSE);
	if (QuitButton.clickOnButton(gameEvent, onSound)) new_game.SetState(QUIT);
	new_game.updateScore();
}
void ConsoleCtrl::game()
{

	new_game.load();
	srand(time(NULL));
	thread t(&ConsoleCtrl::checkSound, this);
	thread i(&ConsoleCtrl::input, this);
	while (new_game.IsState(QUIT) == false)
	{
		startLoop = G_GetTicks();
		gameEvent = G_Event();
		new_game.draw();
		UIdraw();
		if (new_game.IsState(PAUSE) == false)
		{
			new_game.update();
			update_score();
		}
		if (new_game.IsState(START)) start();
		else if (new_game.IsState(PLAY)) play();
		else if (new_game.IsState(OUT)) new_game.eagle();
		else if (new_game.IsState(PAUSE)) pause();
		else if (new_game.IsState(CHOOSE_PLAYER)) choose_player();
		else if (new_game.IsState(GAME_OVER)) game_over();
		if (gameEvent == G_QUIT) new_game.SetState(QUIT);
		endLoop = G_GetTicks() - startLoop;
		if (endLoop < delay)
		{
			G_Delay(delay - endLoop);
		}
		G_Update();
	}
	t.join();
	i.join();
}
void ConsoleCtrl::checkSound()
{
	while (new_game.IsState(QUIT) == false)
	{
		if (onSound == true)              //Check sound
		{
			if (SoundButton.clickOnButton(gameEvent, onSound))
			{
				onSound = false;
			}
		}
		else if (onSound == false)
		{
			if (OffSoundButton.clickOnButton(gameEvent, onSound))
			{
				onSound = true;
			}
		}
	}
}