#include "pch.h"
#include "Object.h"


Object::Object()
{

}

Object::Object(const Object& other)
{
	position = other.position;
	texture = other.texture;
	tile = other.tile;
	isMoving = other.isMoving;
	dir = other.dir;
	sound = other.sound;
	skin = other.skin;
	moveSpeed = other.moveSpeed;
	timer = other.timer;
}

Object::~Object()
{
}

void Object::self_draw()
{
	if (dir == LEFT)
	{
		G_Draw(texture[skin], &position);
	}
	else G_DrawEx(texture[skin], &position, SDL_FLIP_HORIZONTAL);
}
/*
void Object::self_load(const char *file, enum ObjectType otype,int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	texture.push_back(G_LoadImage(file));
	position = { pos1,pos2,posx,posy };
	type = otype;
	isMoving = Is_move;
	sound = G_LoadSound(sfile);
	if (otype == EAGLE) moveSpeed = 20;
	if (otype == COIN) dir = LEFT;
}
*/
void Object::self_update(int modelposw, int winposw)
{
	if (dir == LEFT && position.x < -modelposw)
	{
		position.x = winposw;
	}
	else if (dir == RIGHT && position.x > winposw)
	{
		position.x = -modelposw;
	}
	if (isMoving)
	{
		position.x += moveSpeed;
	}
}
void Object::add_texture(const char *file)
{
	texture.push_back(G_LoadImage(file));
}
/*
void Object::load_player(const char *file, G_Rect pos, const char *sfile)
{
	texture.push_back(G_LoadImage(file));
	position = pos;
	type = PLAYER;
	sound = G_LoadSound(sfile);
	//isMoving = true;
}
*/
void Object::change_skin(size_t n)
{
	if (n >= texture.size()) n = 0;
	skin = n;
}
void Object::random_skin()
{
	change_skin(rand() % texture.size());
}
void Object::Playsound()
{
	G_PlaySound(sound, 0);
}
void Player::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	texture.push_back(G_LoadImage(file));
	position = { pos1,pos2,posx,posy };
	isMoving = Is_move;
	sound = G_LoadSound(sfile);

}
void CAR::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	texture.push_back(G_LoadImage(file));
	position = { pos1,pos2,posx,posy };
	isMoving = Is_move;
	sound = G_LoadSound(sfile);
}
void ANIMAL::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	Object::self_load(file, pos1, pos2, posx, posy, sfile, Is_move);
}
void STICK::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	texture.push_back(G_LoadImage(file));
	position = { pos1,pos2,posx,posy };
	isMoving = Is_move;
	sound = G_LoadSound(sfile);
}
void LAMP::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	texture.push_back(G_LoadImage(file));
	position = { pos1,pos2,posx,posy };
	isMoving = Is_move;
	sound = G_LoadSound(sfile);
}
void TRAIN::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	texture.push_back(G_LoadImage(file));
	position = { pos1,pos2,posx,posy };
	isMoving = Is_move;
	sound = G_LoadSound(sfile);
}
void EAGLE::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	texture.push_back(G_LoadImage(file));
	position = { pos1,pos2,posx,posy };
	isMoving = Is_move;
	sound = G_LoadSound(sfile);
	moveSpeed = 20;
}
void COIN::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	texture.push_back(G_LoadImage(file));
	position = { pos1,pos2,posx,posy };
	isMoving = Is_move;
	sound = G_LoadSound(sfile);
	dir = LEFT;
}
void Player::load_player(const char *file, G_Rect pos, const char *sfile)
{
	texture.push_back(G_LoadImage(file));
	position = pos;
	sound = G_LoadSound(sfile);
}
Player::~Player()
{
	cout << "Player deleted" << endl;
	size_t s = texture.size();
	for (size_t i = 0; i < s; ++i) G_DestroyTexture(texture[i]);
	G_FreeSound(sound);
}
CAR::~CAR()
{
	instance--;
	if (instance == 0)
	{
		cout << "Car model deleted" << endl;
		size_t s = texture.size();
		for (size_t i = 0; i < s; ++i) G_DestroyTexture(texture[i]);
		G_FreeSound(sound);
	}
}
ANIMAL::~ANIMAL()
{
	instance--;
	if (instance == 0)
	{
		cout << "Animal model deleted" << endl;
		size_t s = texture.size();
		for (size_t i = 0; i < s; ++i) G_DestroyTexture(texture[i]);
		G_FreeSound(sound);
	}
}
TRAIN::~TRAIN()
{
	instance--;
	if (instance == 0)
	{
		cout << "Train model deleted" << endl;
		size_t s = texture.size();
		for (size_t i = 0; i < s; ++i) G_DestroyTexture(texture[i]);
		G_FreeSound(sound);
	}
}
STICK::~STICK()
{
	instance--;
	if (instance == 0)
	{
		cout << "Stick model deleted" << endl;
		size_t s = texture.size();
		for (size_t i = 0; i < s; ++i) G_DestroyTexture(texture[i]);
		G_FreeSound(sound);
	}
}
