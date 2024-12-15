#include "Button.h"

Button::Button()
{

}
Button::Button(int _x, int _y, int _width, int _height, buttonStates _currentState) : gameObject(_x, _y, _width, _height), currentState(_currentState)
{
}
Button::~Button()
{

}

void Button::handleMouseEvent(SDL_Event* mouseEvent)
{
	if (mouseEvent->type == SDL_MOUSEMOTION || mouseEvent->type == SDL_MOUSEBUTTONDOWN || mouseEvent->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		bool inside = true;

        if (x < m_x)
        {
            inside = false;
        }
        //Mouse is right of the button
        else if (x > m_x + m_width)
        {
            inside = false;
        }
        //Mouse above the button
        else if (y < m_y)
        {
            inside = false;
        }
        //Mouse below the button
        else if (y > m_y + m_height)
        {
            inside = false;
        }

        if (inside == false)
        {
            currentState = button_idle;
        }
        else
        {
            switch (mouseEvent->type)
            {
            case SDL_MOUSEMOTION:
                currentState = button_mouse_over;
                break;

            case SDL_MOUSEBUTTONDOWN:
                currentState = button_mouse_down;
                break;

            case SDL_MOUSEBUTTONUP:
                currentState = button_mouse_up;
                break;

            }
        }
	}
}

buttonStates Button::getCurrentState()
{
    return currentState;
}

void Button::setCurrentState(buttonStates _newState)
{
    currentState = _newState;
}