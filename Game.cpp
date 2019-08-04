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