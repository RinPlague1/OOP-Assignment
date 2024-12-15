#include "Trunk.h"



Trunk::Trunk() : Tree()
{

}
Trunk::Trunk(leftOrRight _trunkState, int _vectorIndex, int _x, int _y, int _width, int _height) : Tree(_vectorIndex, _x, _y, _width, _height), m_hasTrunk(_trunkState)
{

}
Trunk::~Trunk()
{

}


