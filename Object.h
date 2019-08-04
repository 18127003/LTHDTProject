#pragma once
#include "Genio.h"
#include <vector>
using namespace std;
//enum ObjectType { PLAYER, CAR, TRAIN, STICK, LAMP, EAGLE, COIN};
enum Direction { UP, DOWN, LEFT, RIGHT };
class Object
{
protected:
	G_Rect position;
	vector<G_Texture*> texture;
	//ObjectType type;
	SDL_Point tile;
	bool isMoving = false;
	Direction dir;
	G_Sound* sound = NULL;
	size_t skin = 0;
	union { //All of Objects Need MoveSpeed Except Train. The Speed of Train Should Be Constant. Instead of Speed , it has a Timer to show when it should come!!!
		int moveSpeed;
		int timer;
	};
public:
	void self_draw(); //G_draw
	virtual void self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move) = 0;
	virtual void self_update(int modelposw, int winposw);
	bool Is_move() { return isMoving; }
	//void load_player(const char *file, G_Rect pos, const char *sfile);
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
	void self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move);
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
	void self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move);
	~CAR();
};
class ANIMAL :public Object
{
private:
	static int instance;
public:
	ANIMAL() { instance++; };
	ANIMAL(const Object* other) : Object(*other) { instance++; }
	void self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move);
	~ANIMAL();
};
class TRAIN :public Object
{
private:
	static int instance;
public:
	TRAIN() { instance++; };
	TRAIN(const Object* other) : Object(*other) { instance++; }
	void self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move);
	~TRAIN();
};
class STICK :public Object
{
private:
	static int instance;
public:
	STICK() { instance++; };
	STICK(const Object* other) : Object(*other) { instance++; }
	void self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move);
	~STICK();
};
class LAMP :public Object
{
private:
	static int instance;
public:
	LAMP() { instance++; };
	LAMP(const Object* other) : Object(*other) { instance++; }
	void self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move);
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
