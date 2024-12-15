#include "Tree.h"

Tree::Tree() : gameObject()
{

}
Tree::Tree(SDL_Rect _destRect, leftOrRight _trunkState, int _vectorIndex, int _x, int _y, int _width, int _height) : gameObject(_x, _y, _width, _height), m_destRect(_destRect), m_vectorPosition(_vectorIndex), m_hasTrunk(_trunkState)
{

}
Tree::~Tree()
{

}

int Tree::getVectorPos()
{
	return m_vectorPosition;
}
void Tree::setVectorPos(int _pos)
{
	m_vectorPosition = _pos;
}

void Tree::formatRect()
{
	m_destRect = {m_x, m_y, m_width, m_height};
}


SDL_Rect Tree::getDestRect()
{
	return m_destRect;
}
void Tree::setDestRect(SDL_Rect _newDestRect)
{	
	m_destRect = _newDestRect;
}

leftOrRight Tree::getTrunkStatus()
{
	return m_hasTrunk;
}
void Tree::setTrunkStatus(leftOrRight _newStatus)
{
	m_hasTrunk = _newStatus;
}
