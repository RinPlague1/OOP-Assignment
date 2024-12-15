#pragma once

#ifndef _TREE_CLASS_
#define _TREE_CLASS_

#include <SDL.h>

#include "gameObject.h"

enum leftOrRight
{

    none = 0,
    left = 1,       // just made this make it easier to see if its got a trunk or not
    right = 2

};


class Tree :
    public gameObject
{
private:
    int m_vectorPosition;
    leftOrRight m_hasTrunk; // left isTrue right is false
    SDL_Rect m_destRect;

public:
    Tree();
    Tree(SDL_Rect _destRect, leftOrRight _trunkState, int _vectorIndex, int _x, int _y, int _width, int _height);
    ~Tree();

    int getVectorPos();
    void setVectorPos(int _pos);

    void formatRect();

    SDL_Rect getDestRect();
    void setDestRect(SDL_Rect _newDestRect);

    leftOrRight getTrunkStatus();
    void setTrunkStatus(leftOrRight _newStatus);

};

#endif // !_TREE_CLASS_