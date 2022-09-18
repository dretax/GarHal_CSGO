#pragma once
#ifndef _inputsystem_
#define _inputsystem_

#include <Windows.h>

enum INPUTS
{
    LBUTTON_DOWN,
    LBUTTON_CLICKED,
    KEY_INSERT_PRESSED,
    MAX_INPUT
};

class InputSystem
{
    int MouseX, MouseY;
public:
    bool InputStates[MAX_INPUT];

    InputSystem()
    {
        memset(InputStates, 0, sizeof(InputStates));
        MouseX = 0;
        MouseY = 0;
    }

    void UpdateMousePos(int diffx, int diffy)
    {
        POINT p;
        if (GetCursorPos(&p))
        {
            MouseX = p.x - (diffx / 2);
            MouseY = p.y - (diffy / 2);
        }
    }

    void GetMousePos(int& x, int& y)
    {
        x = MouseX;
        y = MouseY;
    }

    void PollInputState(bool ShouldSave)
    {
        if ((GetAsyncKeyState(VK_LBUTTON) >> 16) & 1)
        {
            if (ShouldSave)
                InputStates[LBUTTON_DOWN] = true;
        }
        else
        {
            if (ShouldSave)
                InputStates[LBUTTON_DOWN] = false;
        }

        if (GetAsyncKeyState(VK_LBUTTON) & 1)
        {
            if (ShouldSave)
                InputStates[LBUTTON_CLICKED] = true;
        }
        else
        {
            if (ShouldSave)
                InputStates[LBUTTON_CLICKED] = false;
        }
    }
};

#endif
