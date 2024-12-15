#pragma once

#ifndef _TRUNK_CLASS_
#define _TRUNK_CLASS_

#include <string>

#include "Tree.h"





class Trunk :
    public Tree
{
private:


public:
    Trunk();
    Trunk(leftOrRight trunkState, int _vectorIndex, int _x, int _y, int _width, int _height);
    ~Trunk();


};

#endif // !_TRUNK_CLASS_

