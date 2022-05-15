#pragma once
// #include <stdbool.h>


struct Input
{
    float sensitivity;
    float mouse_last_x;
    float mouse_last_y;
    
    // whether or not the mouse has moved yet
    bool first_mouse;

    // invert y axis of control
    bool invert_y_axis;

    Input();
};
