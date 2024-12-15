#include "gameObject.h"

gameObject::gameObject()
{

}

gameObject::gameObject(int _x, int _y, int _width, int _height) : m_x(_x), m_y(_y), m_width(_width), m_height(_height)
{

}

gameObject::~gameObject()
{

}



int gameObject::getX()
{
	return m_x;
}

int gameObject::getY()
{
	return m_y;
}

int gameObject::getWidth()
{
	return m_width;
}

int gameObject::getHeight()
{
	return m_height;
}

void gameObject::setX(int _x)
{
	m_x = _x;
}
void gameObject::setY(int _y)
{
	m_y = _y;
}
void gameObject::setWidth(int _width)
{
	m_width = _width;
}
void gameObject::setHeight(int _height)
{
	m_height = _height;
}

bool gameObject::checkCollision(SDL_Rect _playerRect, SDL_Rect _treeRect)
{
	//https://lazyfoo.net/tutorials/SDL/27_collision_detection/index.php

	int playerLeft, playerRight, playerTop, playerBottom;
	int treeLeft, treeRight, treeTop, treeBottom;

	playerLeft = _playerRect.x;
	playerRight = _playerRect.x + _playerRect.w;
	playerTop = _playerRect.y;
	playerBottom = _playerRect.y + _playerRect.h;

	treeLeft = _treeRect.x;
	treeRight = _treeRect.x + _treeRect.w;
	treeTop = _treeRect.y;
	treeBottom = _treeRect.y + _treeRect.h;

	if (playerBottom <= treeTop)
	{
		return false;
	}

	if (playerTop >= treeBottom)
	{
		return false;
	}

	if (playerRight <= treeLeft)
	{
		return false;
	}

	if (playerLeft >= treeRight)
	{
		return false;
	}

	return true;
}