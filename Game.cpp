#include "pch.h"
#include "Game.h"


Game::Game()
{
	RetrieveScore("topScore.txt");
	Car = new CAR;
	Stick = new STICK;
	Coin = new COIN;
	Train = new TRAIN;
	Lamp = new LAMP;
	Eagle = new EAGLE;
}


Game::~Game()
{
	while (!objects.empty() && objects.front()->tile.y == rows - 1)
	{
		objects.pop_front();
	}
	for (int i = 0; i < rows; i++)
		delete[] map[i];
	delete[] map;
	TTF_CloseFont(font);
	FreeObject();
	FreeTileTexture();
	FreeUI();
	G_FreeSound(clickSound);
	delete Car;
	delete Stick;
	delete Coin;
	delete Train;
	delete Lamp;
	delete Eagle;
}
void Game::FreeObject()
{
	for (size_t i = 0; i < Player.texture.size(); i++) G_DestroyTexture(Player.texture[i]);
	G_FreeSound(Player.sound);
	for (size_t i = 0; i < Train->texture.size(); i++) G_DestroyTexture(Train->texture[i]);
	G_FreeSound(Train->sound);
	for (size_t i = 0; i < Lamp->texture.size(); i++) G_DestroyTexture(Lamp->texture[i]);
	G_FreeSound(Lamp->sound);
	for (size_t i = 0; i < Stick->texture.size(); i++) G_DestroyTexture(Stick->texture[i]);
	G_FreeSound(Stick->sound);
	for (size_t i = 0; i < Car->texture.size(); i++) G_DestroyTexture(Car->texture[i]);
	G_FreeSound(Car->sound);
	for (size_t i = 0; i < Coin->texture.size(); i++) G_DestroyTexture(Coin->texture[i]);
	G_FreeSound(Coin->sound);
}
void Game::FreeTileTexture()
{
	G_DestroyTexture(grassTile.texture);
	G_DestroyTexture(treeTile.texture);
	G_DestroyTexture(lampTile.texture);
	G_DestroyTexture(waterTile.texture);
	G_DestroyTexture(roadTile.texture);
	G_DestroyTexture(railTile.texture);
	G_DestroyTexture(gateTile.texture);
}
void Game::FreeUI()
{
	G_DestroyTexture(Logo.texture);
	G_DestroyTexture(TryAgainButton.texture);
	G_DestroyTexture(ClickButton.texture);
	G_DestroyTexture(PauseButton.texture);
	G_DestroyTexture(PlayerChooseButton.texture);
	G_DestroyTexture(PlayerButton.texture);
	G_DestroyTexture(ScoreText.texture);
	G_DestroyTexture(CoinText.texture);
	G_DestroyTexture(TopScoreText.texture);
	G_DestroyTexture(GameOver.texture);
	G_DestroyTexture(QuitButton.texture);
	G_DestroyTexture(SoundButton.texture);
	G_DestroyTexture(OffSoundButton.texture);
}
void Game::deleteObjects()
{
	while (!objects.empty() && objects.front()->tile.y == rows - 1)
	{
		objects.pop_front();
	}
}
void Game::destroyTiles() {
	for (int i = 0; i < rows; i++)
		delete[] map[i];

	delete[] map;
}
void Game::drawObjects()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->self_draw();
	}
}
void Game::drawTiles()
{
	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			map[i][j].self_draw();
		}
	}
}
void Game::initTiles()
{
	columns = (int)ceil(windowPos.w / TILE_LENGTH);
	rows = (int)ceil(windowPos.h / TILE_LENGTH) + 1;
	map = new TileTexture*[columns];
	for (int i = 0; i < columns; i++)
		map[i] = new TileTexture[rows];

	for (int y = rows - 1; y >= 0; y--)
	{
		if (y >= rows - 2) {
			for (int x = 0; x < columns; x++) {
				map[x][y] = (x == 0 || x == columns - 1) ? treeTile : grassTile;
				map[x][y].position.x = x * 100;
				map[x][y].position.y = (y - 1) * 100;
			}
		}
		else
		{
			generateTiles(y);
			addObjects(y);
		}
	}
}
void Game::generateTiles(int row)
{
	//TileType rowType = (TileType)(rand() % 4);
	int rowChance = rand() % 100 + 1;
	int Trees = 0;
	bool Gate = false;
	if (score % 50 == 0 && score > 0 && map[columns / 2][row - 1].type != GATE)
	{
		Gate = true;
		//rowType = GATE;
	}
	for (int x = 0; x < columns; x++)
	{
		if (Gate == true)
		{
			if (x == columns / 2) map[x][row] = gateTile;
			else map[x][row] = treeTile;
			//cout << "Gate row"<<endl;
		}
		else
		{
			if (rowChance <= 50 /*rowType == GRASS*/)
			{
				int chance = rand() % 100 + 1;
				if (x == 0 || x == columns - 1)
					map[x][row] = treeTile;
				else {
					if (chance >= 80 && Trees < maxTreesInARow)
					{
						if (rand() % 3 == 1)
						{
							map[x][row] = lampTile;
						}
						else map[x][row] = treeTile;
						Trees++;
					}
					else
					{
						map[x][row] = grassTile;
					}
				}
			}
			else if (rowChance <= 70 /*rowType == ROAD*/)
			{
				map[x][row] = roadTile;
			}
			else if (rowChance <= 75 /*rowType == RAIL*/)
			{
				map[x][row] = railTile;
			}
			else if (rowChance <= 100 /*rowType == WATER*/)
			{
				map[x][row] = waterTile;
			}
		}
		map[x][row].position.x = x * 100;
		map[x][row].position.y = map[x][row + 1].position.y - TILE_LENGTH;
	}
}
void Game::addObjects(int row)
{
	if (map[0][row].type != WATER) addCoins(row);
	switch (map[0][row].type)
	{
	case ROAD:
		addCar(row);
		break;
	case WATER:
		addStick(row);
		break;
	case RAIL:
		addTrain(row);
		break;
	}
}
void Game::addCar(int row)
{
	Direction dir = (Direction)((rand() % 2) + 2);
	Car->dir = dir;
	Car->position.y = map[0][row].position.y;
	Car->tile.y = row;
	Car->moveSpeed = (rand() % 5 + 2) * (dir == RIGHT ? 1 : -1);
	int num = (rand() % 3) + 1;
	int space = (windowPos.w - ((num - 1) * Car->position.w)) / num;
	int randPos = rand() % windowPos.w;
	for (int i = 0; i < num; i++)
	{
		Object* CarDup = new CAR(Car);
		objects.push_back(CarDup);////////////////////////////////////////////////////////
		if (dir == LEFT)
			objects.back()->position.x = windowPos.w + (i*(space + Car->position.w)) - randPos;
		else
			objects.back()->position.x = -Car->position.w - (i*(space + Car->position.w)) + randPos;
		short int tex = rand() % 3;
		switch (tex)
		{
		case 1:
			objects.back()->change_skin(1);
			break;
		case 2:
			objects.back()->change_skin(2);
			break;
		}
	}
}
void Game::addStick(int row)
{
	Stick->position.y = map[0][row].position.y;
	Stick->tile.y = row;
	if (map[0][row + 1].type == WATER) { // Direction of two rivers should be diffrent.
		for (int i = objects.size() - 1; i >= 0; i--) {
			if (objects[i]->tile.y == row + 1 && dynamic_cast<STICK*>(objects[i]) != NULL)
			{
				Stick->dir = (objects[i]->dir == LEFT ? RIGHT : LEFT);
				Stick->isMoving = (objects[i]->isMoving ? rand() % 2 : true); //We Cant Have Two Stopped Rivers
				break;
			}
		}
	}
	else
	{
		Stick->isMoving = rand() % 2;
		Stick->dir = (Direction)((rand() % 2) + 2);
	}

	if (Stick->isMoving)
	{
		Stick->moveSpeed = (rand() % 3 + 1) * (Stick->dir == RIGHT ? 1 : -1);
		int num = (rand() % 4) + 3;
		int space = (windowPos.w - ((num - 1) * Stick->position.w)) / num;
		int randPos = rand() % 200;
		for (int i = 0; i < num; i++)
		{
			Object *StickDup = new STICK(Stick);
			objects.push_back(StickDup);///////////////////////////////////////////////////////////
			if (Stick->dir == LEFT)
				objects.back()->position.x = randPos + (i*(space + Stick->position.w));
			else
				objects.back()->position.x = windowPos.w - randPos - (i*(space + Stick->position.w));
		}
	}
	else
	{
		int num = 0;
		do {
			for (int i = 0; i < columns; i++)
			{
				if (map[i][row + 1].type != TREE) {
					int chance = rand() % 100 + 1;
					if (chance >= 50 && num < maxSticksInARow) {
						num++;
						Stick->position.x = map[i][row].position.x;
						Object *StickDup = new STICK(Stick);
						objects.push_back(StickDup);/////////////////////////////////////////
					}
				}
			}
		} while (num < minSticksInARow);
	}
}
void Game::addTrain(int row)
{
	Direction dir = (Direction)((rand() % 2) + 2);
	Train->dir = dir;
	Train->position.y = map[0][row].position.y;
	Train->tile.y = row;
	Train->timer = ((rand() % 5) + 2) * FPS;
	Object* TrainDup = new TRAIN(Train);
	objects.push_back(TrainDup);////////////////////////////////////////
	if (dir == LEFT)
		objects.back()->position.x = windowPos.w + Train->position.w;
	else
		objects.back()->position.x = -Train->position.w;

	Lamp->tile.y = row;
	Lamp->position.y = map[0][row].position.y + TILE_LENGTH - Lamp->position.h;
	Lamp->position.x = map[0][row].position.x + 20;
	Object *LampDup = new LAMP(Lamp);
	objects.push_back(LampDup);////////////////////////////////////////////////
}
void Game::addCoins(int row)
{
	int num = 0;
	Coin->position.y = map[0][row].position.y;
	Coin->position.w = Coin->position.h = 100;
	Coin->tile.y = row;
	for (int i = 0; i < columns; i++)
	{
		if (map[i][row].type != TREE) {
			int chance = rand() % 101;
			if (chance >= 90 && num < maxCoinsInARow) {
				num++;
				Coin->position.x = map[i][row].position.x;
				Object *CoinDup = new COIN(Coin);
				objects.push_back(CoinDup);//////////////////////////////////////////
			}
		}
	}
}
void Game::checkPlayerStatus()
{
	if (state != PLAY) return;

	if (Player.position.y + 5 >= windowPos.h)
	{
		state = OUT;
		Eagle->position.x = Player.position.x - (Eagle->position.w / 2);
		Eagle->position.y = -Eagle->position.h;
		eagleIntersection = false;
		return;
	}

	if (Player.position.x <= -TILE_LENGTH || Player.position.x >= windowPos.w)
		state = GAME_OVER;

	bool onStick = false;
	G_Rect temp = { Player.position.x + realPlayerClip.x + 5 , Player.position.y + realPlayerClip.y + 5 , realPlayerClip.w - 5,realPlayerClip.h - 5 };
	for (size_t i = 0; i < objects.size(); i++) {
		if (SDL_HasIntersection(&temp, &objects[i]->position) == SDL_TRUE)
		{
			if (dynamic_cast<CAR*>(objects[i]) != NULL || dynamic_cast<TRAIN*>(objects[i]) != NULL)
			{
				state = GAME_OVER;
				G_PlaySound(Player.sound, 0);
			}
			else if (dynamic_cast<STICK*>(objects[i]) != NULL)
			{
				onStick = true;
				if (!Player.isMoving) {
					if (objects[i]->isMoving) Player.position.x += objects[i]->moveSpeed;
					if (Player.position.x >= map[Player.tile.x][0].position.x + TILE_LENGTH) Player.tile.x++;
					else if (Player.position.x <= map[Player.tile.x][0].position.x - TILE_LENGTH) Player.tile.x--;
					if (Player.tile.x >= columns || Player.tile.x < 0) state = GAME_OVER;
				}
			}
			else if (dynamic_cast<COIN*>(objects[i]) != NULL)
			{
				coins++;
				G_PlaySound(Coin->sound, 0);
				updateScore();
				objects.erase(objects.begin() + i);
				i--;
				continue;
			}
		}
	}
	if (!Player.isMoving && !onStick && map[Player.tile.x][Player.tile.y].type == WATER)
		state = GAME_OVER;
}
void Game::updateScore()
{
	if (score > maxScore) maxScore = score;
	string all = to_string(maxScore);
	strcpy_s(chars, 10, all.c_str());

	//Destroy Perivious Texture And Make A New
	//G_DestroyTexture(ScoreText.texture);
	//ScoreText.texture = G_LoadFont(font, chars, 255, 255, 255);
	ScoreText.self_update(font, chars);
	all = to_string(coins);
	strcpy_s(chars, 10, all.c_str());
	CoinText.self_update(font, chars);
	//Destroy Perivious Texture And Make A New
	//G_DestroyTexture(CoinText.texture);
	//CoinText.texture = G_LoadFont(font, chars, 253, 230, 75);
}
//Suspicious
void Game::eagle()
{
	Eagle->position.y += Eagle->moveSpeed;
	G_Draw(Eagle->texture[Eagle->skin], &Eagle->position);
	if (!eagleIntersection && SDL_HasIntersection(&Player.position, &Eagle->position)) {
		Player.position.x = windowPos.w; //Move it to a hidden Area
		G_PlaySound(Eagle->sound, 0);
		eagleIntersection = true;
	}

	if (Eagle->position.y > windowPos.h)
		state = GAME_OVER;
}
void Game::loadTiles()
{
	grassTile.self_load("assets/image/grass.png", GRASS);
	lampTile.self_load("assets/image/streetlamp.png", TREE);
	treeTile.self_load("assets/image/tree.png", TREE);
	waterTile.self_load("assets/image/sea.png", WATER);
	roadTile.self_load("assets/image/street.png", ROAD);
	railTile.self_load("assets/image/rail.png", RAIL);
	gateTile.self_load("assets/image/gate.png", GATE);
	cout << "Load tile complete" << endl;
}
void Game::loadObjects()
{
	Car->self_load("assets/image/car.png", 0, 0, 169, 100, "assets/sound/car-horn.wav", true);
	Car->add_texture("assets/image/car2.png");
	Car->add_texture("assets/image/car3.png");
	//car2Texture = G_LoadImage("assets/image/car2.png");
	//car3Texture = G_LoadImage("assets/image/car3.png");
	Stick->self_load("assets/image/stick1.png", 0, 0, 100, 100, "assets/sound/water.wav", false);
	Train->self_load("assets/image/train.png", 0, 0, 544, 100, "assets/sound/train_pass_no_horn.wav", false);
	Lamp->self_load("assets/image/green-lamp.png", 0, 0, 39, 93, "assets/sound/train_alarm.wav", false);
	Lamp->add_texture("assets/image/red-lamp.png");
	//redLampTexture = G_LoadImage("assets/image/red-lamp.png");
	Eagle->self_load("assets/image/eagle.png", 0, 0, 350, 250, "assets/sound/eagle_hit.wav", false);
	Coin->self_load("assets/image/coin.png", windowPos.w - 100, 20, 100, 100, "assets/sound/coin_tap.wav", false);
	//Gate.self_load("assets/image/gate.png",GATE,0,0,0,0)
	cout << "Load object complete" << endl;
}
void Game::loadPlayer()
{
	//sheepTexture = G_LoadImage("assets/image/player.png");
	//cowTexture = G_LoadImage("assets/image/player2.png");
	//pigTexture = G_LoadImage("assets/image/player3.png");
	Player.tile = { columns / 2, rows - 2 };
	Player.load_player("assets/image/player.png", map[Player.tile.x][Player.tile.y].position, "assets/sound/car_squish.wav");
	Player.add_texture("assets/image/player2.png");
	Player.add_texture("assets/image/player3.png");
	cout << "load player complete" << endl;
}
void Game::loadUI()
{
	ScoreText.self_load(font, "0", 255, 255, 255, 10, 10, 31, 42);
	TopScoreText.self_load(font, "TOP 0", 255, 255, 255, 10, 60, 81, 52);
	CoinText.self_load(font, "0", 253, 230, 75, windowPos.w - 70, 8, 20, 35);
	//GameOverText.self_load(font, "GAME OVER!", 0, 0, 0, (windowPos.w - 345) / 2, (windowPos.h - 43) / 2, 345, 43);
	Logo.self_load("assets/image/logo.png", (windowPos.w - 764) / 2, (windowPos.h - 144) / 3, 764, 144);
	ClickButton.self_load("assets/image/click.png", (windowPos.w - 100) / 2, Logo.getPos('y') + 190, 100, 172);
	TryAgainButton.self_load("assets/image/again.png", (windowPos.w - 177) / 2, (windowPos.h / 2) + 150, 177, 89);
	PauseButton.self_load("assets/image/pause.png", windowPos.w - 70, 70, 50, 50);
	PlayerChooseButton.self_load("assets/image/settings.png", 20, windowPos.h - 120, 125, 100);
	GameOver.self_load("assets/image/gameover.png", (windowPos.w - 764) / 2, (windowPos.h - 144) / 3 - 75, 764, 450);
	QuitButton.self_load("assets/image/quit.png", windowPos.w - 100, windowPos.h - 120, 100, 100);
	SoundButton.self_load("assets/image/onsound.png", 125, windowPos.h - 120, 100, 100);
	OffSoundButton.self_load("assets/image/offsound.png", 125, windowPos.h - 120, 100, 100);
	cout << "Load UI complete" << endl;
}
void Game::load()
{
	loadTiles();
	loadObjects();
	initTiles();
	loadPlayer();
	font = G_OpenFont("assets/font/editundo.ttf", 72);
	loadUI();
	clickSound = G_LoadSound("assets/sound/click.wav");
}
void Game::adjustCameraSpeed()
{
	if (state == OUT) {
		cameraSpeed = -2;
		return;
	}
	if (state != PLAY) {
		cameraSpeed = 0;
		return;
	}
	cameraSpeed = cameraBaseSpeed;
	switch (Player.tile.y) {
	case 4:
		cameraSpeed += 1;
		break;
	case 3:
		cameraSpeed += 3;
		break;
	case 2:
		cameraSpeed += 6;
		break;
	case 1:
		cameraSpeed = playerMoveSpeed;
		break;
	}
}
void Game::update()                // change to Update inside object
{
	adjustCameraSpeed();

	for (int y = 0; y < rows; y++)
		for (int x = 0; x < columns; x++)
			map[x][y].position.y += cameraSpeed;

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->position.y += cameraSpeed;
		if (dynamic_cast<CAR*>(objects[i]) != NULL)
		{
			if (objects[i]->dir == LEFT && objects[i]->position.x < -Car->position.w) {
				objects[i]->position.x = windowPos.w;
			}
			else if (objects[i]->dir == RIGHT && objects[i]->position.x > windowPos.w) {
				objects[i]->position.x = -Car->position.w;
			}
		}

		if (dynamic_cast<STICK*>(objects[i]) != NULL)
		{
			if (objects[i]->dir == LEFT && objects[i]->position.x < -Stick->position.w)
				objects[i]->position.x = windowPos.w;
			else if (objects[i]->dir == RIGHT && objects[i]->position.x > windowPos.w)
				objects[i]->position.x = -Stick->position.w;
		}

		if (dynamic_cast<TRAIN*>(objects[i]) != NULL)
		{
			if (objects[i]->isMoving)
			{
				if (objects[i]->dir == LEFT && objects[i]->position.x < -Train->position.w)
				{
					objects[i]->position.x = windowPos.w;
					objects[i]->isMoving = false;
					objects[i + 1]->change_skin(0); // We Are Sure there is a lamp just after a train!!
					objects[i]->timer = ((rand() % 5) + 2) * FPS;
				}
				else if (objects[i]->dir == RIGHT && objects[i]->position.x > windowPos.w)
				{
					objects[i]->position.x = -Train->position.w;
					objects[i]->isMoving = false;
					objects[i + 1]->change_skin(0);
					objects[i]->timer = ((rand() % 5) + 2) * FPS;
				}
			}
			else {
				if (objects[i]->timer == FPS)
				{  //1 Second Before it move we should change the lamp!!
					objects[i + 1]->change_skin(1);
					G_PlaySound(Lamp->sound, 0);
				}
				if (objects[i]->timer == 0)
				{
					objects[i]->isMoving = true;
					G_PlaySound(Train->sound, 0);
				}
				else
					objects[i]->timer--;
			}
		}

		if (objects[i]->isMoving)
		{
			if (dynamic_cast<TRAIN*>(objects[i]) != NULL)
				objects[i]->position.x += (objects[i]->dir == LEFT ? -trainMoveSpeed : trainMoveSpeed);
			else
				objects[i]->position.x += objects[i]->moveSpeed;
		}
	}

	if (map[0][rows - 1].position.y > windowPos.h) {
		for (int y = rows - 1; y > 0; y--)
			for (int x = 0; x < columns; x++)
				map[x][y] = map[x][y - 1];

		generateTiles(0);
		deleteObjects();

		for (size_t i = 0; i < objects.size(); i++)
			objects[i]->tile.y += 1;
		addObjects(0);
		Player.tile.y += 1;
	}
	Player.position.y += cameraSpeed;

	checkPlayerStatus();
}
char* Game::NewMaxScore()
{
	if (maxScore > topScore) topScore = maxScore;
	string all = "TOP " + to_string(topScore);
	strcpy_s(chars, 10, all.c_str());
	return chars;
}
void Game::Reload()
{
	destroyTiles();
	//deleteObjects();
	objects.clear();
	maxScore = score = coins = 0;
	updateScore();
	initTiles(); //ReGenerate The Envirement!
	Player.tile = { columns / 2 , rows - 2 };
	Player.position = map[Player.tile.x][Player.tile.y].position;
	state = START;
}
void Game::draw()
{
	drawTiles();
	if (state == GAME_OVER)
		G_Draw(Player.texture[Player.skin], &Player.position); // If player lose,Draw it Under Objects!!
	drawObjects();
	if (state != GAME_OVER)
		G_Draw(Player.texture[Player.skin], &Player.position); // But If It is alive , Draw It top of Objects!  
	ScoreText.self_draw();
	Coin->position = { windowPos.w - 50,0,50,50 };
	Coin->self_draw();
	CoinText.self_draw();
	QuitButton.self_draw();
}
void Game::SwitchPlayerTexture(int i)
{
	PlayerButton.settexture(Player.texture[i]);
}
void Game::SetPlayerTexture()
{
	//Player.texture = PlayerButton.Get_texture();
	for (size_t i = 0; i < Player.texture.size(); i++)
	{
		if (Player.texture[i] == PlayerButton.Get_texture())
		{
			Player.change_skin(i);
			break;
		}
	}
}
void Game::PlayerMove()
{
	if (Player.isMoving)
	{
		switch (Player.dir)
		{
		case UP:
			Player.position.y -= playerMoveSpeed;
			if (Player.position.y <= map[Player.tile.x][Player.tile.y - 1].position.y)
			{
				Player.isMoving = false;
				score++;
				Player.tile.y--;
				Player.position.y = map[Player.tile.x][Player.tile.y].position.y;
				//Player.position.x = map[Player.tile.x][Player.tile.y].position.x;
				if (map[Player.tile.x][Player.tile.y].type == ROAD) G_PlaySound(Car->sound, 0);
				else if (map[Player.tile.x][Player.tile.y].type == WATER) G_PlaySound(Stick->sound, 0);
			}
			break;
		case RIGHT:
			Player.position.x += playerMoveSpeed;
			if (Player.position.x >= map[Player.tile.x + 1][Player.tile.y].position.x)
			{
				Player.isMoving = false;
				Player.tile.x++;
				Player.position.x = map[Player.tile.x][Player.tile.y].position.x;
			}
			break;
		case LEFT:
			Player.position.x -= playerMoveSpeed;
			if (Player.position.x <= map[Player.tile.x - 1][Player.tile.y].position.x)
			{
				Player.isMoving = false;
				Player.tile.x--;
				Player.position.x = map[Player.tile.x][Player.tile.y].position.x;
			}
			break;
		case DOWN:
			Player.position.y += playerMoveSpeed;
			if (Player.position.y >= map[Player.tile.x][Player.tile.y + 1].position.y)
			{
				Player.isMoving = false;
				score--;
				Player.tile.y++;
				Player.position.y = map[Player.tile.x][Player.tile.y].position.y;
			}
			break;
		}
	}
}
void Game::CheckObstacle(int i)
{
	switch (i)
	{
	case 0:
		if (map[Player.tile.x][Player.tile.y - 1].type != TREE && map[Player.tile.x][Player.tile.y - 1].type != GATE)//normal tile
		{
			Player.isMoving = true;
			Player.dir = UP;
			//Player.position.y -= playerMoveSpeed;
		}
		if (map[Player.tile.x][Player.tile.y - 1].type == GATE)//gate tile
		{
			if (coins > score / 2)
			{
				Player.isMoving = true;
				Player.dir = UP;
			}
		}
		//tree tile
		break;
	case 1:
		if (Player.tile.x + 1 < columns && map[Player.tile.x + 1][Player.tile.y].type != TREE)
		{
			Player.isMoving = true;
			Player.dir = RIGHT;
			//Player.position.x += playerMoveSpeed;
		}
		break;
	case 2:
		if (Player.tile.x - 1 >= 0 && map[Player.tile.x - 1][Player.tile.y].type != TREE)
		{
			Player.isMoving = true;
			Player.dir = LEFT;
			//Player.position.x -= playerMoveSpeed;
		}
		break;
	case 3:
		if (Player.tile.y + 1 < rows && map[Player.tile.x][Player.tile.y + 1].type != TREE)
		{
			Player.isMoving = true;
			Player.dir = DOWN;
			//Player.position.y += playerMoveSpeed;
		}
		break;
	}
}
bool Game::Player_isMove()
{
	return Player.isMoving;
}
bool Game::IsState(enum GameState stat)
{
	if (state == stat) return true;
	return false;
}
void Game::SaveScoreToFile(string path)
{
	ofstream out_file(path);
	if (!out_file) cout << "Can not save." << endl;
	out_file << topScore;
	out_file.close();
}
void Game::RetrieveScore(string path)
{
	ifstream in_file(path);
	if (!in_file)
	{
		topScore = 0;
		cout << "High score was not initialized.";
	}
	in_file >> topScore;
	in_file.close();
}