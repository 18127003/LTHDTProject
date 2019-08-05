#include "pch.h"
#include "Object.h"

int CAR::instance = 0;
int STICK::instance = 0;
int TRAIN::instance = 0;
int LAMP::instance = 0;
int COIN::instance = 0;
int ANIMAL::instance = 0;
Object::Object()
{
	texture.reserve(5);
}

Object::Object(const Object& other)
{
	*this = other;
}

Object::~Object()
{

	vector <G_Texture*>().swap(texture);
	sound = NULL;

}

void Object::self_draw()
{
	if (dir == LEFT)
	{
		G_Draw(texture[skin], &position);
	}
	else G_DrawEx(texture[skin], &position, SDL_FLIP_HORIZONTAL);
}

void Object::add_texture(const char *file)
{
	texture.push_back(G_LoadImage(file));
}

void Object::change_skin(size_t n)
{
	if (n >= texture.size()) n = 0;
	skin = n;
}
void Object::random_skin()
{
	change_skin(rand() % texture.size());
}
void Object::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	texture.push_back(G_LoadImage(file));
	position = { pos1,pos2,posx,posy };
	isMoving = Is_move;
	sound = G_LoadSound(sfile);
}
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
void CAR::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	Object::self_load(file, pos1, pos2, posx, posy, sfile, Is_move);
}


void ANIMAL::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	Object::self_load(file, pos1, pos2, posx, posy, sfile, Is_move);
}


void Player::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	Object::self_load(file, pos1, pos2, posx, posy, sfile, Is_move);
}


void TRAIN::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	Object::self_load(file, pos1, pos2, posx, posy, sfile, Is_move);
}

void LAMP::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	Object::self_load(file, pos1, pos2, posx, posy, sfile, Is_move);
}
void STICK::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	Object::self_load(file, pos1, pos2, posx, posy, sfile, Is_move);
}

void EAGLE::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	Object::self_load(file, pos1, pos2, posx, posy, sfile, Is_move);
	moveSpeed = 20;
}
void COIN::self_load(const char *file, int pos1, int pos2, int posx, int posy, const char* sfile, bool Is_move)
{
	Object::self_load(file, pos1, pos2, posx, posy, sfile, Is_move);
	dir = LEFT;
}
void Player::load_player(const char *file, G_Rect pos, const char *sfile)
{
	texture.push_back(G_LoadImage(file));
	position = pos;
	sound = G_LoadSound(sfile);
}
void Object::Playsound()
{
	G_PlaySound(sound, 0);
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
LAMP::~LAMP()
{
	instance--;
	if (instance == 0)
	{
		cout << "Lamp model deleted" << endl;
		size_t s = texture.size();
		for (size_t i = 0; i < s; ++i) G_DestroyTexture(texture[i]);
		G_FreeSound(sound);
	}
}
COIN::~COIN()
{
	instance--;
	if (instance == 0)
	{
		cout << "Coin model deleted" << endl;
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
EAGLE::~EAGLE()
{
	cout << "Eagle model deleted" << endl;
	size_t s = texture.size();
	for (size_t i = 0; i < s; ++i) G_DestroyTexture(texture[i]);
	G_FreeSound(sound);
}