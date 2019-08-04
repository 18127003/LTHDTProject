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