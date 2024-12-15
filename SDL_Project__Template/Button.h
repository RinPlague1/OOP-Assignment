#pragma once

#ifndef _BUTTON_CLASS_
#define _BUTTON_CLASS_

#include "gameObject.h"

// https://lazyfoo.net/tutorials/SDL/17_mouse_events/index.php

enum buttonStates
{
    button_idle = 0,
    button_mouse_over = 1,
    button_mouse_down = 2,
    button_mouse_up = 3,
    button_mouse_total = 4
};


class Button :
    public gameObject
{
private:
    SDL_Point mousePosition;
    buttonStates currentState;
public:
    Button();
    Button(int _x, int _y, int _width, int _height, buttonStates _currentState);
    ~Button();

    void handleMouseEvent(SDL_Event* mouseEvent);

    buttonStates getCurrentState();
    void setCurrentState(buttonStates _newState);

};

#endif