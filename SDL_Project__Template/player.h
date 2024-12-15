#pragma once

#ifndef _PLAYER_CLASS_
#define _PLAYER_CLASS_

#include "gameObject.h"

enum playerStateValue
{
    p_idle = 0,
    p_left = 1,
    p_right = 2,
    p_dead = 3
};

enum characterChosenValue
{
    p_lumberTim = 0,
    p_theHunter = 1
};

class player :
    public gameObject
{
private:
    playerStateValue m_playerState;
    characterChosenValue m_characterChosen;

public:
    player();
    player(int _x, int _y, int _width, int _height, playerStateValue _playerState, characterChosenValue _characterChosen);
    ~player();

    playerStateValue getplayerState();
    void setplayerState(playerStateValue _newState);

    characterChosenValue getCharacterChosen();
    void setCharacterChosen(characterChosenValue _newCharacter);
    void setCharacterChosenNum(int _newCharacter);

};

#endif // !_PLAYER_CLASS_

