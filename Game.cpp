#include "pch.h"
#include "Game.h"
#include "ConsoleCtrl.h"


Game::Game()
{
	RetrieveScore("topScore.txt");
	ObjModel[0] = new CAR;
	ObjModel[1] = new STICK;
	ObjModel[2] = new TRAIN;
	ObjModel[3] = new LAMP;
	ObjModel[4] = new EAGLE;
	ObjModel[5] = new COIN;
	ObjModel[6] = new ANIMAL;
}


Game::~Game()
{
	for (int i = objects.size() - 1; i >= 0; i--)
	{
		delete objects[i];
	}
	for (int i = 0; i < rows; i++)
		delete[] map[i];
	delete[] map;
	for (int i = 0; i < 7; ++i) delete ObjModel[i];
	for (int i = 0; i < 7; ++i) FreeTileTexture(TileModel[i]);
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
	if (maxScore == 50)
	{
		TileModel[0].change_skin(1);
		TileModel[1].change_skin(1);
		TileModel[2].change_skin(1);
		TileModel[5].change_skin(1);
	}
	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			map[i][j].self_draw();
		}
	}
}
void Game::draw() /////Console draw
{
	drawTiles();
	if (state == GAME_OVER)
	{
		G_Draw(Player.texture[Player.texture.size() - 1], &Player.position); // If player lose,Draw it Under Objects!!
	}
	drawObjects();
	if (state != GAME_OVER)
		G_Draw(Player.texture[Player.skin], &Player.position); // But If It is alive , Draw It top of Objects!  
	ObjModel[5]->position = { windowPos.w - 50,0,50,50 };
	ObjModel[5]->self_draw();
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
				map[x][y] = (x == 0 || x == columns - 1) ? TileModel[1] : TileModel[0];
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
	int rowChance = rand() % 100 + 1;
	int Trees = 0;
	TileModel[4].change_skin(rand() % 2);
	for (int x = 0; x < columns; x++)
	{
		if (Gate < Player.Level() && score > 0 && map[columns / 2][row - 1].type != GATE)
		{
			if (x == columns / 2) map[x][row] = TileModel[6];
			else map[x][row] = TileModel[1];
			Gate++;
		}
		else
		{
			if (rowChance <= 50 /*rowType == GRASS*/)
			{
				int chance = rand() % 100 + 1;
				if (x == 0 || x == columns - 1)
					map[x][row] = TileModel[1];
				else {
					if (chance >= 80 && Trees < maxTreesInARow)
					{
						if (rand() % 3 == 1)
						{
							map[x][row] = TileModel[2];
						}
						else map[x][row] = TileModel[1];
						Trees++;
					}
					else
					{
						map[x][row] = TileModel[0];
					}
				}
			}
			else if (rowChance <= 70 /*rowType == ROAD*/)
			{
				map[x][row] = TileModel[4];
			}
			else if (rowChance <= 75 /*rowType == RAIL*/)
			{
				map[x][row] = TileModel[5];
			}
			else if (rowChance <= 100 /*rowType == WATER*/)
			{
				map[x][row] = TileModel[3];
			}
		}
		map[x][row].position.x = x * 100;
		map[x][row].position.y = map[x][row + 1].position.y - TILE_LENGTH;
	}
}
void Game::addStick(int row)
{
	ObjModel[1]->position.y = map[0][row].position.y;
	ObjModel[1]->tile.y = row;
	if (map[0][row + 1].type == WATER) { // Direction of two rivers should be diffrent.
		for (int i = objects.size() - 1; i >= 0; i--) {
			if (objects[i]->tile.y == row + 1 && dynamic_cast<STICK*>(objects[i]) != NULL)
			{
				ObjModel[1]->dir = (objects[i]->dir == LEFT ? RIGHT : LEFT);
				ObjModel[1]->isMoving = (objects[i]->isMoving ? rand() % 2 : true); //We Cant Have Two Stopped Rivers
				break;
			}
		}
	}
	else
	{
		ObjModel[1]->isMoving = rand() % 2;
		ObjModel[1]->dir = (Direction)((rand() % 2) + 2);
	}

	if (ObjModel[1]->isMoving)
	{
		ObjModel[1]->moveSpeed = (rand() % 3 + 1) * (ObjModel[1]->dir == RIGHT ? 1 : -1);
		int num = (rand() % 4) + 3;
		int space = (windowPos.w - ((num - 1) * ObjModel[1]->position.w)) / num;
		int randPos = rand() % 200;
		for (int i = 0; i < num; i++)
		{
			Object *StickDup = new STICK(ObjModel[1]);
			objects.push_back(StickDup);///////////////////////////////////////////////////////////
			if (ObjModel[1]->dir == LEFT)
				objects.back()->position.x = randPos + (i*(space + ObjModel[1]->position.w));
			else
				objects.back()->position.x = windowPos.w - randPos - (i*(space + ObjModel[1]->position.w));
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
						ObjModel[1]->position.x = map[i][row].position.x;
						Object *StickDup = new STICK(ObjModel[1]);
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
	ObjModel[2]->dir = dir;
	ObjModel[2]->position.y = map[0][row].position.y;
	ObjModel[2]->tile.y = row;
	ObjModel[2]->timer = ((rand() % 5) + 2) * FPS;
	ObjModel[2]->random_skin();
	Object* TrainDup = new TRAIN(ObjModel[2]);
	objects.push_back(TrainDup);////////////////////////////////////////
	if (dir == LEFT)
		objects.back()->position.x = windowPos.w + ObjModel[2]->position.w;
	else
		objects.back()->position.x = -ObjModel[2]->position.w;

	ObjModel[3]->tile.y = row;
	ObjModel[3]->position.y = map[0][row].position.y + TILE_LENGTH - ObjModel[3]->position.h;
	ObjModel[3]->position.x = map[0][row].position.x + 20;
	Object *LampDup = new LAMP(ObjModel[3]);
	objects.push_back(LampDup);////////////////////////////////////////////////
}
void Game::addCoins(int row)
{
	int num = 0;
	ObjModel[5]->position.y = map[0][row].position.y;
	ObjModel[5]->position.w = ObjModel[5]->position.h = 100;
	ObjModel[5]->tile.y = row;
	for (int i = 0; i < columns; i++)
	{
		if (map[i][row].type != TREE) {
			int chance = rand() % 101;
			if (chance >= 90 && num < maxCoinsInARow) {
				num++;
				ObjModel[5]->position.x = map[i][row].position.x;
				Object *CoinDup = new COIN(ObjModel[5]);
				objects.push_back(CoinDup);//////////////////////////////////////////
			}
		}
	}
}
void Game::addObjects(int row)
{
	if (map[0][row].type != WATER) addCoins(row);
	switch (map[0][row].type)
	{
	case ROAD:
		if (map[0][row].skin == 0) addEnemy<CAR>(ObjModel[0], row);
		else addEnemy<ANIMAL>(ObjModel[6], row);
		break;
	case WATER:
		addStick(row);
		break;
	case RAIL:
		addTrain(row);
		break;
	}
}
template<class O, class T>
void Game::addEnemy(T*obj, int row)
{
	Direction dir = (Direction)((rand() % 2) + 2);
	obj->dir = dir;
	obj->position.y = map[0][row].position.y;
	obj->tile.y = row;
	obj->moveSpeed = (rand() % 5 + 2) * (dir == RIGHT ? 1 : -1);
	int num = (rand() % 3) + 1;
	int space = (windowPos.w - ((num - 1) * obj->position.w)) / num;
	int randPos = rand() % windowPos.w;
	for (int i = 0; i < num; i++)
	{
		Object* Prefab = new O(obj);
		objects.push_back(Prefab);////////////////////////////////////////////////////////
		if (dir == LEFT)
			objects.back()->position.x = windowPos.w + (i*(space + obj->position.w)) - randPos;
		else
			objects.back()->position.x = -obj->position.w - (i*(space + obj->position.w)) + randPos;
		objects.back()->random_skin();
	}
}
void Game::deleteObjects()
{
	while (!objects.empty() && objects.front()->tile.y == rows - 1)
	{
		delete objects[0];
		objects.pop_front();
	}
}
void Game::destroyTiles() {
	for (int i = 0; i < rows; i++)
		delete[] map[i];

	delete[] map;
}
template<class T>
void Game::FreeTileTexture(T obj)
{
	size_t s = obj.texture.size();
	for (size_t i = 0; i < s; ++i) G_DestroyTexture(obj.texture[i]);
	vector<G_Texture*>().swap(obj.texture);
}
void Game::loadTiles()
{
	TileModel[0].self_load("assets/image/floor.png", GRASS);
	TileModel[0].add_texture("assets/image/floor1.png");
	TileModel[2].self_load("assets/image/streetlamp.png", TREE);
	TileModel[2].add_texture("assets/image/crystal.png");
	TileModel[1].self_load("assets/image/tree.png", TREE);
	TileModel[1].add_texture("assets/image/lava.png");
	TileModel[3].self_load("assets/image/sea.png", WATER);
	TileModel[4].self_load("assets/image/street.png", ROAD);
	TileModel[4].add_texture("assets/image/grass.png");
	TileModel[5].self_load("assets/image/rail.png", RAIL);
	TileModel[5].add_texture("assets/image/rail1.png");
	TileModel[6].self_load("assets/image/gate.png", GATE);
	cout << "Load tile complete" << endl;
}
void Game::loadObjects()
{
	ObjModel[0]->self_load("assets/image/car.png", 0, 0, 169, 100, "assets/sound/car-horn.wav", true);
	ObjModel[0]->add_texture("assets/image/car2.png");
	ObjModel[0]->add_texture("assets/image/car3.png");
	ObjModel[6]->self_load("assets/image/turtle.png", 0, 0, 169, 100, "assets/sound/animal_horn.wav", true);
	ObjModel[6]->add_texture("assets/image/sheep.png");
	ObjModel[1]->self_load("assets/image/stick1.png", 0, 0, 100, 100, "assets/sound/water.wav", false);
	ObjModel[2]->self_load("assets/image/train.png", 0, 0, 544, 100, "assets/sound/train_pass_no_horn.wav", false);
	ObjModel[2]->add_texture("assets/image/train2.png");
	ObjModel[3]->self_load("assets/image/green-lamp.png", 0, 0, 39, 93, "assets/sound/train_alarm.wav", false);
	ObjModel[3]->add_texture("assets/image/red-lamp.png");
	ObjModel[4]->self_load("assets/image/eagle.png", 0, 0, 350, 250, "assets/sound/eagle_hit.wav", false);
	ObjModel[5]->self_load("assets/image/coin.png", windowPos.w - 100, 20, 100, 100, "assets/sound/coin_tap.wav", false);
	cout << "Load object complete" << endl;
}
void Game::loadPlayer()
{
	Player.tile = { columns / 2, rows - 2 };
	Player.load_player("assets/image/player.png", map[Player.tile.x][Player.tile.y].position, "assets/sound/car_squish.wav");
	Player.add_texture("assets/image/player2.png");
	Player.add_texture("assets/image/player3.png");
	Player.add_texture("assets/image/player_die.png");
	cout << "load player complete" << endl;
}
void Game::load()
{
	loadTiles();
	loadObjects();
	initTiles();
	loadPlayer();

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
			if (coins > score / 3)
			{
				Player.isMoving = true;
				Player.dir = UP;
				//Player.LevelUp();
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
void Game::updateScore()// repair
{
	if (score > 0 && score % 50 == 0) Player.LevelUp();
	if (score > maxScore) maxScore = score;
}
template<class T>
void Game::Playsound(T& obj)
{
	if (ConsoleCtrl::onSound == true) obj.Playsound();
}
template<class T>
void Game::Playsound(T* obj)
{
	if (ConsoleCtrl::onSound == true) obj->Playsound();
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
void Game::checkPlayerStatus()
{
	if (state != PLAY) return;

	if (Player.position.y + 5 >= windowPos.h)
	{
		state = OUT;
		ObjModel[4]->position.x = Player.position.x - (ObjModel[4]->position.w / 2);
		ObjModel[4]->position.y = -ObjModel[4]->position.h;
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
			if (dynamic_cast<CAR*>(objects[i]) != NULL || dynamic_cast<TRAIN*>(objects[i]) != NULL || dynamic_cast<ANIMAL*>(objects[i]) != NULL)
			{
				state = GAME_OVER;
				Playsound(Player);
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
				Playsound(ObjModel[5]);
				updateScore();
				delete objects[i];
				objects.erase(objects.begin() + i);
				i--;
				continue;
			}
		}
	}
	if (!Player.isMoving && !onStick && map[Player.tile.x][Player.tile.y].type == WATER)
		state = GAME_OVER;
}
void Game::update()
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
			objects[i]->self_update(ObjModel[0]->position.w, windowPos.w);
		}
		if (dynamic_cast<ANIMAL*>(objects[i]) != NULL)
		{
			objects[i]->self_update(ObjModel[6]->position.w, windowPos.w);
		}
		if (dynamic_cast<STICK*>(objects[i]) != NULL)
		{
			objects[i]->self_update(ObjModel[1]->position.w, windowPos.w);
		}

		if (dynamic_cast<TRAIN*>(objects[i]) != NULL)
		{
			if (objects[i]->isMoving)
			{
				if (objects[i]->dir == LEFT && objects[i]->position.x < -ObjModel[2]->position.w)
				{
					objects[i]->position.x = windowPos.w;
					objects[i]->isMoving = false;
					objects[i + 1]->change_skin(0); // We Are Sure there is a lamp just after a train!!
					objects[i]->timer = ((rand() % 5) + 2) * FPS;
				}
				else if (objects[i]->dir == RIGHT && objects[i]->position.x > windowPos.w)
				{
					objects[i]->position.x = -ObjModel[2]->position.w;
					objects[i]->isMoving = false;
					objects[i + 1]->change_skin(0);
					objects[i]->timer = ((rand() % 5) + 2) * FPS;
				}
			}
			else {
				if (objects[i]->timer == FPS)
				{  //1 Second Before it move we should change the lamp!!
					objects[i + 1]->change_skin(1);
					Playsound(ObjModel[3]);
				}
				if (objects[i]->timer == 0)
				{
					objects[i]->isMoving = true;
					Playsound(ObjModel[3]);
				}
				else
					objects[i]->timer--;
			}
			if (objects[i]->isMoving)
			{
				objects[i]->position.x += (objects[i]->dir == LEFT ? -trainMoveSpeed : trainMoveSpeed);
			}
		}
	}

	if (map[0][rows - 1].position.y > windowPos.h)
	{
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
				if (map[Player.tile.x][Player.tile.y].type == ROAD)
				{
					if (map[Player.tile.x][Player.tile.y].skin == 0) Playsound(ObjModel[0]);
					else Playsound(ObjModel[6]);
				}
				else if (map[Player.tile.x][Player.tile.y].type == WATER) Playsound(ObjModel[1]);
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
G_Texture* Game::GetPlayerTexture(int i)
{
	return Player.texture[i];
}
void Game::SetPlayerTexture(G_Texture* txture)
{
	for (size_t i = 0; i < Player.texture.size(); i++)
	{
		if (Player.texture[i] == txture)
		{
			Player.change_skin(i);
			break;
		}
	}
}
int Game::NewTopScore()
{
	if (maxScore > topScore) topScore = maxScore;
	return topScore;
}
void Game::Reload()
{
	destroyTiles();
	size_t s = objects.size();
	for (size_t i = 0; i < s; ++i)
	{
		delete objects[i];
	}
	objects.clear();
	maxScore = score = coins = 0;
	updateScore();
	for (int i = 0; i < 7; i++) TileModel[i].change_skin(0);
	initTiles(); //ReGenerate The Envirement!
	Player.tile = { columns / 2 , rows - 2 };
	Player.position = map[Player.tile.x][Player.tile.y].position;
	Player.ResetLv();
	state = START;
}
void Game::eagle()
{
	ObjModel[4]->position.y += ObjModel[4]->moveSpeed;
	G_Draw(ObjModel[4]->texture[ObjModel[4]->skin], &ObjModel[4]->position);
	if (!eagleIntersection && SDL_HasIntersection(&Player.position, &ObjModel[4]->position)) {
		Player.position.x = windowPos.w; //Move it to a hidden Area
		Playsound(ObjModel[4]);
		eagleIntersection = true;
	}

	if (ObjModel[4]->position.y > windowPos.h)
		state = GAME_OVER;
}