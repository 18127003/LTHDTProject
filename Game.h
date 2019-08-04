#pragma once
#include "Object.h"
#include "TileTexture.h"
#include "UI.h"
#include <deque>
#include <fstream>

enum GameState { START, PLAY, QUIT, PAUSE, GAME_OVER, OUT, CHOOSE_PLAYER };

//Constants
const int TILE_LENGTH = 100;
const int playerMoveSpeed = 10;
const int maxTreesInARow = 2;
const int maxSticksInARow = 5;
const int minSticksInARow = 2;
const int maxCoinsInARow = 2;
const int FPS = 60;
const int delay = 1000 / FPS;
const int cameraBaseSpeed = 1;
const int trainMoveSpeed = 18;

class Game
{
private:
	deque<Object*> objects;
	TileTexture ** map = NULL;
	GameState state = START;
	Player Player;
	TileTexture TileModel[7];  //(grassTile, treeTile,lampTile, waterTile, roadTile, railTile, gateTile)
	Object* ObjModel[7];     //(Car,Stick,Train,Lamp,Eagle,Coin,Animal)

	int rows, columns;
	bool eagleIntersection = false;
	int maxScore = 0, score = 0, topScore = 0, coins = 0;
	int Gate = 0;
public:
	int getCoin() { return coins; }
	int getmaxScore() { return maxScore; }
	int cameraSpeed = cameraBaseSpeed;
	G_Rect windowPos;

	G_Rect realPlayerClip = { 30,0,40,100 };
	Game();
	~Game();
	void drawObjects();
	void drawTiles();
	void initTiles();
	void generateTiles(int row);
	void addStick(int row);
	void addTrain(int row);
	void addCoins(int row);
	void addObjects(int row);
	template<class O, class T >
	void addEnemy(T* obj, int row);
	void deleteObjects();
	void destroyTiles();
	void load();
	void loadTiles();
	void loadObjects();
	void loadPlayer();
	void CheckObstacle(int i);
	void updateScore();
	template<class T>
	void Playsound(T& obj);
	template<class T>
	void Playsound(T* obj);