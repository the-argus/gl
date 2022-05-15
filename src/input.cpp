#include "input.h"

Input::Input()
{
    Input::first_mouse = true;
    Input::invert_y_axis = false;

    Input::sensitivity = 0.1f;

    Input::mouse_last_x = 0.0f;
    Input::mouse_last_y = 0.0f;
}
