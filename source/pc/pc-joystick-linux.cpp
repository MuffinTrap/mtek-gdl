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

Joystick* joysticks[4];

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
void Joystick_Init()
{
    for (int i = 0; i < 4; i++)
    {
        joysticks[i] = Joystick_Create(i);
        Joystick* joystick = joysticks[i];
        const char* device;
        switch (i)
        {
        case 0:
            device = "/dev/input/js0";
            break;

        case 1:
            device = "/dev/input/js1";
            break;

        case 2:
            device = "/dev/input/js2";
            break;

        case 3:
            device = "/dev/input/js3";
            break;
        }
        joystick->linux_device_index = open(device, O_NONBLOCK);
        if (joystick->linux_device_index == -1)
        {
            Log_ErrorF("Could not open joystick %d\n", joystick->index);
            joystick->isConnected = false;
            continue;
        }
        else
        {
            joystick->isConnected = true;
            joystick->axisCount = get_axis_count(joystick->linux_device_index);
            joystick->buttonCount = get_button_count(joystick->linux_device_index);

            // Get joystick name
            char name[128];
            if (ioctl(joystick->linux_device_index, JSIOCGNAME(sizeof(name)), name) < 0)
            {
                strncpy(name, "Unknown", sizeof(name));
            }
            Log_InfoF("Joystick Init %d %s : %zu axii %zu buttons", joystick->index, name, joystick->axisCount, joystick->buttonCount);
            joystick->axes = (struct axis_state*)malloc(sizeof(struct axis_state) * joystick->axisCount);
        }
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


static void ReadAxis(Joystick* stick, sizetype axis)
{
    struct axis_state state = stick->axes[axis];

    switch(axis)
    {
        case 0: // Left thumbstick
            // Nunchaku
			stick->controller.m_nunchukJoystickDirectionX = Joystick_NormalizeAxis(state.x);
			stick->controller.m_nunchukJoystickDirectionY = Joystick_NormalizeAxis(state.y);
            break;
        case 1:
        {
            // Right stick left - right
			stick->controller.m_roll = Joystick_NormalizeAxis(state.y) * M_PI;

            float leftTrigger = (Joystick_NormalizeAxis(state.x) + 1.0f) /2.0f; // To [0, 1]
            // TODO Figure how to make this better
            float radians = leftTrigger * -M_PI;
            float newYaw = minF(stick->controller.m_yaw, radians);
            stick->controller.m_yaw = newYaw;
        }
        break;
        case 2:
        {
            // Right stick up-down
			stick->controller.m_pitch = Joystick_NormalizeAxis(state.x) * M_PI;

            float rightTrigger = (Joystick_NormalizeAxis(state.y) + 1.0f) /2.0f; // To [0, 1]
            float radians = rightTrigger * M_PI;
            float newYaw = maxF(stick->controller.m_yaw, radians);
            stick->controller.m_yaw = newYaw;
        }
        break;
        case 3:
            // Direction pad : always returns max values
            if (state.x == AXIS_MAX)
            {
                WiiController_SetButtonDown(&stick->controller, ButtonRight);
            }
            else if (state.x == -AXIS_MAX)
            {
                WiiController_SetButtonDown(&stick->controller, ButtonLeft);
            }
            else
            {
                WiiController_SetButtonUp(&stick->controller, ButtonLeft);
                WiiController_SetButtonUp(&stick->controller, ButtonRight);
            }
            if (state.y == -AXIS_MAX)
            {
                WiiController_SetButtonDown(&stick->controller, ButtonUp);
            }
            else if (state.y == AXIS_MAX)
            {
                WiiController_SetButtonDown(&stick->controller, ButtonDown);
            }
            else
            {
                WiiController_SetButtonUp(&stick->controller, ButtonDown);
                WiiController_SetButtonUp(&stick->controller, ButtonUp);
            }
            break;
    }
}

void Joystick_StartFrame()
{
    for (int i = 0; i < 4; i++)
    {
        Joystick* joystick = joysticks[i];
        if (joystick->isConnected == false)
        {
            continue;
        }

        WiiController_StartFrame(&joystick->controller);
    }
}

void Joystick_ReadInputs()
{
    struct js_event event;
    size_t axis;
    for (int i = 0; i < 4; i++)
	{
		Joystick* joystick = joysticks[i];
		if (joystick->isConnected == false)
		{
			continue;
		}

		// Read until queue is empty
		while (read(joystick->linux_device_index, &event, sizeof(js_event)) > 0)
		{
			switch (event.type)
			{
			case JS_EVENT_BUTTON:
				//Log_InfoF("Button %u %s\n", event.number, event.value ? "pressed" : "released");
				if (event.value)
				{
					WiiController_SetButtonDown(&joystick->controller, ButtonToWiiButton(event.number));
				}
				else
				{
					WiiController_SetButtonUp(&joystick->controller, ButtonToWiiButton(event.number));
				}
				break;
			case JS_EVENT_AXIS:
				axis = get_axis_state(joystick, &event);
				if (axis < joystick->axisCount)
				{
					if (axis == 3)
					{
						//Log_InfoF("Axis %zu at (%6d, %6d)\n", axis, joystick->axes[axis].x, joystick->axes[axis].y);
					}
					ReadAxis(joystick, axis);
				}
				break;
			default:
				/* Ignore init events. */
				break;
			}
		}
	}
}

void Joystick_Deinit()
{
    for (int i = 0; i < 4; i++)
    {
        Joystick* joystick = joysticks[i];
        if (joystick->isConnected)
        {
            close(joystick->linux_device_index);
            joystick->isConnected = false;
        }
    }
}
#endif

