#include <mgdl/mgdl-ctoy.h>
#include <mgdl/mgdl-rocket.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-types.h>
#include <mgdl.h>

unsigned short add_to_rocket(const char* name)
{
    Rocket* singleton = _Rocket_GetSingleton();
    ROCKET_TRACK track = Rocket_AddTrack(name);
    return singleton->_trackCount - 1;
}

float get_from_rocket(unsigned short id)
{
    return Rocket_Float(Rocket_GetTrack(id));
}

void set_BPM(double val)
{
    Rocket_SetBeatsPerMinute(val);
}

void set_RPB(double val)
{
    Rocket_SetRowsPerBeat(val);
}

unsigned long ctoy_t(void)
{
    return (unsigned long)Platform_GetElapsedUpdates();
}

double ctoy_get_time(void)
{
    return (double)Platform_GetElapsedSeconds();
}

void ctoy_sleep(long sec, long nsec)
{
    Log_Error("ctoy sleep not implemented on Wii");
}

int ctoy_argc(void)
{
    return 0;
}

char** ctoy_argv(void)
{
    return nullptr;
}

// Window management

int ctoy_window_width(void)
{
    return mgdl_GetScreenWidth();
}

int ctoy_window_height(void)
{
    return mgdl_GetScreenHeight();
}

void ctoy_window_size(int width, int height)
{
    // TODO
}

void ctoy_window_title(const char* title)
{
    // TODO
}

void ctoy_window_fullscreen(int state)
{
    // TODO
}

// Fram buffer

int ctoy_frame_buffer_width(void)
{
    return ctoy_window_width();
}

int ctoy_frame_buffer_height(void)
{
    return ctoy_window_height();
}

void ctoy_render_image(struct m_image *src)
{

}

void ctoy_swap_buffer(struct m_image *src)
{

}

/* keyboard events */
int ctoy_key_press(int key); // return 1 on key press, or return 0
int ctoy_key_release(int key); // return 1 on key release, or return 0
int ctoy_key_pressed(int key); // return 1 if key is currently pressed, or return 0
int ctoy_get_chars(unsigned int dest[CTOY_CHAR_MAX]); // return number of characters typed and get a copy

/* mouse events */
float ctoy_mouse_x(void)
{
   return mgdl_GetController(0)->_cursorX;
}
float ctoy_mouse_y(void)
{
   return mgdl_GetController(0)->_cursorY;
}

WiiButtons MouseButtonToWiiButton(int button)
{
    switch(button)
    {
        case 0: return ButtonA; break; // Left
        case 1: return ButtonB; break; // middle?
        case 2: return ButtonB; break; // right
        default: return ButtonA; break;
    }
}

int ctoy_mouse_button_press(int button)
{
    if (WiiController_ButtonPress(mgdl_GetController(0), MouseButtonToWiiButton(button)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int ctoy_mouse_button_release(int button)
{
    if (WiiController_ButtonRelease(mgdl_GetController(0), MouseButtonToWiiButton(button)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int ctoy_mouse_button_pressed(int button)
{

    if (WiiController_ButtonHeld(mgdl_GetController(0), MouseButtonToWiiButton(button)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* scroll events */
float ctoy_scroll_x(void); // return scroll x position
float ctoy_scroll_y(void); // return scroll y position

/* joysticks events */
int ctoy_joystick_present(int joy)
{
    if (joy == 0) {return 1; }
    else {return 0;}
}
int ctoy_joystick_axis_count(int joy)
{
    return 1;
}
int ctoy_joystick_button_count(int joy)
{
    return 13;
}

WiiButtons JoystickButtonToWiiButton(int button)
{
    switch(button)
    {
        case 0: return Button1; break; // Left
        case 1: return Button2; break; // middle?
        case 2: return ButtonA; break; // right
        case 3: return ButtonB; break; // Left
        case 4: return ButtonMinus; break; // middle?
        case 5: return ButtonHome; break; // right
        case 6: return ButtonLeft; break; // Left
        case 7: return ButtonRight; break; // middle?
        case 8: return ButtonDown; break; // right
        case 9: return ButtonUp; break; // Left
        case 10: return ButtonPlus; break; // middle?
        case 12: return ButtonZ; break; // right
        case 13: return ButtonC; break;

        default: return ButtonA; break;
    }
}

int ctoy_joystick_button_press(int joy, int button)
{

    if (WiiController_ButtonPress(mgdl_GetController(joy), JoystickButtonToWiiButton(button)))
    {
        return 1;
    }
    else
    {
        return 0;
    }

}
int ctoy_joystick_button_release(int joy, int button)
{
    if (WiiController_ButtonRelease(mgdl_GetController(joy), JoystickButtonToWiiButton(button)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int ctoy_joystick_button_pressed(int joy, int button)
{
    if (WiiController_ButtonHeld(mgdl_GetController(joy), JoystickButtonToWiiButton(button)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
float ctoy_joystick_axis(int joy, int axis)
{
    vec2 axis = WiiController_GetNunchukJoystickDirection(mgdl_GetController(joy));

    if (axis == 0) { return V2f_X(axis);}
    if (axis == 1) { return V2f_Y(axis);}
    return 0.0f;
}
