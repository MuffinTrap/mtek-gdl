#ifdef MGDL_PLATFORM_LINUX

#include <mgdl/pc/mgdl-joystick.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-util.h>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <linux/joystick.h>

static const short AXIS_MAX = 32767;

/**
 * Returns the number of axes on the controller or 0 if an error occurs.
 */
size_t get_axis_count(int fd)
{
    __u8 axes;

    if (ioctl(fd, JSIOCGAXES, &axes) == -1)
        return 0;

    return axes;
}
/**
 * Returns the number of buttons on the controller or 0 if an error occurs.
 */
size_t get_button_count(int fd)
{
    __u8 buttons;
    if (ioctl(fd, JSIOCGBUTTONS, &buttons) == -1)
        return 0;

    return buttons;
}
void Joystick_Init(Joystick* stick)
{
    const char *device;
    device = "/dev/input/js0";
    stick->linux_device_index = open(device, O_NONBLOCK);
    if (stick->linux_device_index == -1)
    {
        Log_Error("Could not open joystick");
        stick->index = -1;
        return;
    }
    else
    {
        stick->axisCount = get_axis_count(stick->linux_device_index);
        stick->buttonCount = get_button_count(stick->linux_device_index);

        // Get joystick name
        char name[128];
        if (ioctl(stick->linux_device_index, JSIOCGNAME(sizeof(name)), name) < 0)
        {
            strncpy(name, "Unknown", sizeof(name));
        }
        Log_InfoF("Joystick Init %s : %zu axii %zu buttons", name, stick->axisCount, stick->buttonCount);
        stick->axes = (struct axis_state*)malloc(sizeof(struct axis_state) * stick->axisCount);
    }
}


/**
 * Keeps track of the current axis state.
 *
 * NOTE: This function assumes that axes are numbered starting from 0, and that
 * the X axis is an even number, and the Y axis is an odd number. However, this
 * is usually a safe assumption.
 *
 * Returns the axis that the event indicated.
 */
size_t get_axis_state(Joystick* stick, struct js_event *event)
{
    size_t axis = event->number / 2;

    if (axis < stick->axisCount)
    {
        if (event->number % 2 == 0)
            stick->axes[axis].x = event->value;
        else
            stick->axes[axis].y = event->value;
    }

    return axis;
}

static WiiButtons ButtonToWiiButton(int number)
{
    switch(number)
    {
        case 0: return ButtonA; break; // A
        case 1: return ButtonB; break; // B
        case 2: return Button1; break; // X
        case 3: return Button2; break; // Y
        case 4: return ButtonC; break; // LS
        case 5: return ButtonZ; break; // RS

        case 6: return ButtonMinus; break; // Select
        case 7: return ButtonPlus; break;  // Start
        case 8: return ButtonHome; break;  // XBOX
    }
    return ButtonNone;
}

static float NormalizeAxis(short value)
{
    return (float)value / 32767.0f;
}

static void ReadAxis(Joystick* stick, sizetype axis)
{
    struct axis_state state = stick->axes[axis];

    switch(axis)
    {
        case 0: // Left thumbstick
            // Nunchaku
			stick->controller.m_nunchukJoystickDirectionX = NormalizeAxis(state.x);
			stick->controller.m_nunchukJoystickDirectionY = NormalizeAxis(state.y);
            break;
        case 1:
        {
            // Right stick left - right
			stick->controller.m_roll = NormalizeAxis(state.y) * M_PI;

            float leftTrigger = (NormalizeAxis(state.x) + 1.0f) /2.0f; // To [0, 1]
            // TODO Figure how to make this better
            float radians = leftTrigger * -M_PI;
            float newYaw = minF(stick->controller.m_yaw, radians);
            stick->controller.m_yaw = newYaw;
        }
        break;
        case 2:
        {
            // Right stick up-down
			stick->controller.m_pitch = NormalizeAxis(state.x) * M_PI;

            float rightTrigger = (NormalizeAxis(state.y) + 1.0f) /2.0f; // To [0, 1]
            float radians = rightTrigger * M_PI;
            float newYaw = maxF(stick->controller.m_yaw, radians);
            stick->controller.m_yaw = newYaw;
        }
        break;
        case 3:
            // Direction pad : always returns max values
            if (state.x == AXIS_MAX)
            {
                _WiiController_SetButtonDown(&stick->controller, ButtonRight);
            }
            else if (state.x == -AXIS_MAX)
            {
                _WiiController_SetButtonDown(&stick->controller, ButtonLeft);
            }
            else
            {
                _WiiController_SetButtonUp(&stick->controller, ButtonLeft);
                _WiiController_SetButtonUp(&stick->controller, ButtonRight);
            }
            if (state.y == -AXIS_MAX)
            {
                _WiiController_SetButtonDown(&stick->controller, ButtonUp);
            }
            else if (state.y == AXIS_MAX)
            {
                _WiiController_SetButtonDown(&stick->controller, ButtonDown);
            }
            else
            {
                _WiiController_SetButtonUp(&stick->controller, ButtonDown);
                _WiiController_SetButtonUp(&stick->controller, ButtonUp);
            }
            break;
    }
}

void Joystick_ReadInputs(Joystick* stick)
{
    struct js_event event;
    size_t axis;

    // Read until queue is empty
    while ( read(stick->linux_device_index, &event, sizeof(js_event)) > 0)
    {
        switch (event.type)
        {
            case JS_EVENT_BUTTON:
                Log_InfoF("Button %u %s\n", event.number, event.value ? "pressed" : "released");
                if (event.value)
                {
                    _WiiController_SetButtonDown(&stick->controller, ButtonToWiiButton(event.number));
                }
                else
                {
                    _WiiController_SetButtonUp(&stick->controller, ButtonToWiiButton(event.number));
                }
                break;
            case JS_EVENT_AXIS:
                axis = get_axis_state(stick, &event);
                if (axis < stick->axisCount)
                {
                    if (axis == 3)
                    {
                        Log_InfoF("Axis %zu at (%6d, %6d)\n", axis, stick->axes[axis].x, stick->axes[axis].y);
                    }
                    ReadAxis(stick, axis);
                }
                break;
            default:
                /* Ignore init events. */
                break;
        }
    }
}

void Joystick_Disconnect(Joystick* joystick)
{
    close(joystick->linux_device_index);
    joystick->index = -1;
}
#endif

