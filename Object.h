#pragma once
#include "SDLexe.h"
#include <vector>
using namespace std;
enum Direction { UP, DOWN, LEFT, RIGHT };
class Object
{
protected:
	G_Rect position;
	vector<G_Texture*> texture;
	SDL_Point tile;
	bool isMoving = false;
	Direction dir;
	G_Sound* sound = NULL;
	size_t skin = 0;
	union {
		int moveSpeed;//others obj
		int timer; //train
	};
public:
	void self_draw();
	virtual void self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move);
	void self_update(int modelposw, int winposw);
	bool Is_move() { return isMoving; }
	void add_texture(const char *file);
	void random_skin();
	void change_skin(size_t n);
	void Playsound();
	Object();
	Object(const Object& other);
	virtual ~Object();
	friend class Game;
};

class Player : public Object
{
private:
	int level = 0;
public:
	void load_player(const char *file, G_Rect pos, const char *sfile);
	void LevelUp() { level++; }
	int Level() { return level; }
	void ResetLv() { level = 1; }
	~Player();
};
class CAR :public Object
{
private:
	static int instance;
public:
	CAR() { instance++; }
	CAR(const Object* other) : Object(*other) { instance++; }
	~CAR();
};
class ANIMAL :public Object
{
private:
	static int instance;
public:
	ANIMAL() { instance++; };
	ANIMAL(const Object* other) : Object(*other) { instance++; }
	~ANIMAL();
};
class TRAIN :public Object
{
private:
	static int instance;
public:
	TRAIN() { instance++; };
	TRAIN(const Object* other) : Object(*other) { instance++; }
	~TRAIN();
};
class STICK :public Object
{
private:
	static int instance;
public:
	STICK() { instance++; };
	STICK(const Object* other) : Object(*other) { instance++; }
	~STICK();
};
class LAMP :public Object
{
private:
	static int instance;
public:
	LAMP() { instance++; };
	LAMP(const Object* other) : Object(*other) { instance++; }
	~LAMP();
};
class EAGLE :public Object
{
private:

public:
	void self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move);
	~EAGLE();
};
class COIN :public Object
{
private:
	static int instance;
public:
	COIN() { instance++; };
	COIN(const Object* other) : Object(*other) { instance++; }
	void self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move);
	~COIN();
};
