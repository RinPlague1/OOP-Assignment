#include "Sprite.h"
Sprite::Sprite()
{

}

Sprite::~Sprite()
{

}

void Sprite::displayTexture(SDL_Texture* _loadedTexture, int _x, int _y)
{
	m_loadedTexture = _loadedTexture;

	SDL_QueryTexture(m_loadedTexture, 0, 0, &m_textureWidth, &m_textureHeight);
	
	m_srcRect = {0, 0, m_textureWidth, m_textureHeight};

	m_destinationRect = { _x, _y, m_textureWidth, m_textureHeight };
}



SDL_Rect Sprite::getSrcRect()
{
	return m_srcRect;
}
SDL_Rect Sprite::getDestRect()
{
	return m_destinationRect;
}

void Sprite::setDestRect(int _x, int _y)
{
	m_destinationRect = { _x, _y, m_textureWidth, m_textureHeight };
}