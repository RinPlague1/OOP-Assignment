#pragma once
#ifndef _SOURCE_HEADER_
#define _SOURCE_HEADER_

#include <SDL.h>
#include <SDL_TTF.h>
#include <SDL_MIXER.h>
//https://fonts.google.com/download/next-steps?query=pixel

#include <iostream>
#include <vector>

#include "Tree.h"
#include "Player.h"
#include "Button.h"

enum gameState
{
	menu = 0,
	game = 1,
	character_select = 2,
	quit = 3,
	game_over = 4,
	instructions = 5
};

void initialiseButtons(SDL_Renderer* _rend, std::vector<SDL_Texture*> &_buttonTextures, std::vector<SDL_Rect> &_spriteStateRects, std::vector<Button> &_menuButtons);
void initialiseText(TTF_Font* _font, SDL_Color _colour, int _logsChopped, std::vector<SDL_Texture*>& _fontTexture, SDL_Renderer* _rend, std::vector <SDL_Rect> &_textDestRect);
void initialise(SDL_Renderer* _rend, SDL_Texture* &menuBackgroundTexture, SDL_Texture* &backgroundTexture, SDL_Texture* &logTexture, SDL_Texture* &RightlogTexture, SDL_Texture* &LeftlogTexture, std::vector<std::vector<SDL_Rect>> &_rectVector, SDL_Texture* &playerSpriteSheet, std::vector<std::vector<SDL_Rect>> &_playerRectLocations);
void initialiseTreePieces(std::vector<Tree> &_treePieces, std::vector<SDL_Rect> &_destRectVector, int _logsChopped);

void rerenderMenuButtons(SDL_Renderer* _rend, std::vector<SDL_Texture*>& _buttonTextures, std::vector<SDL_Rect>& _spriteStateRects, std::vector<Button>& _menuButtons);
void drawBackButton(SDL_Renderer* _rend, std::vector<SDL_Texture*>& _buttonTextures, std::vector<SDL_Rect>& _spriteStateRects, std::vector<Button>& _menuButtons);

void initialiseHelp(SDL_Renderer* _rend, std::vector<SDL_Texture*> &_textTexture, std::vector<SDL_Rect> &_textRects, TTF_Font* _font, SDL_Color _colour);
void showHelp(SDL_Renderer* _rend, TTF_Font* _font, SDL_Color _colour, SDL_Texture*& _fontTexture, std::vector<SDL_Texture*>& _textTexture, std::vector<SDL_Rect>& _textRects);

void changeCharacter(SDL_Renderer* _rend, player _playerCharacter, std::vector<std::vector<SDL_Rect>> _playerRectLocations, SDL_Texture*& _PlayerSpriteSheet);

void calculateLogSetupTexture(SDL_Renderer* _rend, SDL_Texture* logTexture, SDL_Texture* RightlogTexture, SDL_Texture* LeftlogTexture, std::vector<std::vector<SDL_Rect>>& _imgRectVector, Tree _inputTree, std::vector<SDL_Rect> _treePieceDestRect, int _vectorPos, Tree _prevPos);
void generateNewTreePiece(std::vector<Tree>& _treePieces, std::vector<SDL_Rect>& _destRectVector, int _logsChopped);
void rerenderNewTreeImage(SDL_Renderer* _rend, SDL_Texture* logTexture, SDL_Texture* RightlogTexture, SDL_Texture* LeftlogTexture, std::vector<std::vector<SDL_Rect>>& _imgRectVector, Tree _inputTree, std::vector<SDL_Rect> _treePieceDestRect, int _vectorPos, int _prevVectorPos, std::vector<Tree>& _treePieces);
void updatePlayer(player _playerCharacter, std::vector<std::vector<SDL_Rect>> _playerRectLocations, SDL_Renderer* _rend, SDL_Texture* _PlayerSpriteSheet, bool &_gameOver);
void updateScore(SDL_Renderer* _rend, TTF_Font* _font, SDL_Color _colour, int _logsChopped, int _highscore,SDL_Texture*& _fontTexture, SDL_Rect& _textDestRect, std::vector <SDL_Texture*> digitTextureVector, gameState _currentGameState);



#endif // !_SOURCE_HEADER_
