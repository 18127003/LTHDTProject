#pragma once
#include "Object.h"
#include "TileTexture.h"
#include "UI.h"
#include <deque>
#include <fstream>

enum GameState { START, PLAY, EXITE, PAUSE, GAME_OVER, OUT, CHOOSE_PLAYER };

//Constants
const int TILE_LENGTH = 100;
const int playerMoveSpeed = 10;
const int maxTreesInARow = 2; //Choose it carefully! if it be great enough , Player stuck in trees!!!!
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
	TileTexture grassTile, treeTile, lampTile, waterTile, roadTile, railTile, gateTile;
	Object *Car, *Stick, *Train, *Lamp, *Eagle, *Coin, *Gate;
	//G_Texture* car2Texture;
	//G_Texture* car3Texture;
	//G_Texture* redLampTexture;
	//G_Texture* cowTexture;
	//G_Texture* sheepTexture;
	//G_Texture* pigTexture;
	bool onSound = true;
	int rows, columns;
	bool eagleIntersection = false;
	int maxScore = 0, score = 0, topScore = 0, coins = 0;
	char chars[10] = ""; //Used in updateScore();
public:

	int cameraSpeed = cameraBaseSpeed;

	G_Rect windowPos;

	UI TryAgainButton, ClickButton, SoundButton, OffSoundButton, PauseButton, PlayerChooseButton, PlayerButton, ScoreText, Logo, CoinText, TopScoreText, GameOver, QuitButton;
	G_Font* font;
	G_Sound* clickSound;
	G_Rect realPlayerClip = { 30,0,40,100 };
	Game();
	~Game();
	void drawObjects();
	void drawTiles();
	void initTiles();
	void generateTiles(int row);
	void addObjects(int row);
	void addCar(int row);
	void addStick(int row);
	void addTrain(int row);
	void addCoins(int row);
	void deleteObjects();
	void checkPlayerStatus();
	void updateScore();
	void eagle();
	void destroyTiles();
	void load();
	void loadTiles();
	void loadObjects();
	void loadPlayer();
	void loadUI();
	void adjustCameraSpeed();
	void update();
	void SetState(enum GameState stat) { state = stat; }
	char* NewMaxScore();
	void Reload();
	void draw();
	void SwitchPlayerTexture(int i);
	void SetPlayerTexture();
	void PlayerMove();
	void CheckObstacle(int i);
	bool Player_isMove();
	bool IsState(enum GameState stat);
	bool OnSound() { return onSound; }
	void SetSound(bool stat) { onSound = stat; }
	void SaveScoreToFile(string path);
	void RetrieveScore(string path);
	void FreeObject();
	void FreeTileTexture();
	void FreeUI();
};

