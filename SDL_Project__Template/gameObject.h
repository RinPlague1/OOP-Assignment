#pragma once
#ifndef _GAMEOBJECT_CLASS_
#define _GAMEOBJECT_CLASS_

#include <SDL.h>

class gameObject
{
protected:
	int m_x;
	int m_y;
	int m_width;
	int m_height;

public:
	gameObject();
	gameObject(int _x, int _y, int _width, int _height);
	~gameObject();

	int getX();
	int getY();
	int getWidth();
	int getHeight();

	void setX(int _x);
	void setY(int _y);
	void setWidth(int _width);
	void setHeight(int _height);

	bool checkCollision(SDL_Rect _playerRect, SDL_Rect _treeRect);

};

#endif // !_GAMEOBJECT_CLASS_


