#pragma once

#ifndef _SPRITE_CLASS_
#define _SPRITE_CLASS_

#include <SDL.h>

class Sprite
{
private:
	SDL_Texture* m_loadedTexture;

	int m_textureWidth;
	int m_textureHeight;

	SDL_Rect m_srcRect;
	SDL_Rect m_destinationRect;

public:
	Sprite();
	~Sprite();

	void displayTexture(SDL_Texture* _loadedTexture, int _x, int _y);


	SDL_Rect getSrcRect();
	SDL_Rect getDestRect();
	void setDestRect(int _x, int _y);

};

#endif // !_SPRITE_CLASS_