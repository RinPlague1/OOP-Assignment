#include "player.h"

player::player()
{

}
player::player(int _x, int _y, int _width, int _height, playerStateValue _playerState, characterChosenValue _characterChosen) : gameObject(_x, _y, _width, _height), m_playerState(_playerState), m_characterChosen(_characterChosen)
{

}
player::~player()
{

}

playerStateValue player::getplayerState()
{
	return m_playerState;
}

void player::setplayerState(playerStateValue _newState)
{
	m_playerState = _newState;
}


characterChosenValue player::getCharacterChosen()
{
	return m_characterChosen;
}

void player::setCharacterChosen(characterChosenValue _newCharacter)
{
	m_characterChosen = _newCharacter;
}

void player::setCharacterChosenNum(int _newCharacter)
{
	//https://stackoverflow.com/questions/14897311/pass-an-enum-to-a-function-taking-a-reference-to-int
	m_characterChosen = static_cast<characterChosenValue>(_newCharacter);	//casts the index to the enum
}