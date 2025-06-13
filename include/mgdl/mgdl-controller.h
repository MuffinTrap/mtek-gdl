#pragma once

/**
 * @file mgdl-controller.h
 * @brief Header for the controller class.
 */

#include "mgdl-types.h"
#include "mgdl-vector.h"

/**
 * @brief Enum values for all the buttons on the Wii Controller.
 */
enum WiiButtons
{
	Button1 = 		0x0001,
	Button2 = 		0x0002,
	ButtonB = 		0x0004,
	ButtonA = 		0x0008,
	ButtonMinus = 	0x0010,
	ButtonHome = 	0x0080,
	ButtonLeft = 	0x0100,
	ButtonRight = 	0x0200,
	ButtonDown = 	0x0400,
	ButtonUp = 		0x0800,
	ButtonPlus = 	0x1000,

	// Nunchuck
	ButtonZ	=		(0x0001 << 16),
	ButtonC	=		(0x0002 << 16)
};
typedef enum WiiButtons WiiButtons;

/**
 * @brief Struct representing the Wii controller.
 *
 * This is the Wii controller and nunchuck on Wii and
 * mouse and keyboard on PC platforms
 */
struct WiiController
{
	u32 _pressedButtons;
	u32 _releasedButtons;
	u32 _heldButtons;
	float _nunchukJoystickDirectionX;
	float _nunchukJoystickDirectionY;
	float _cursorX;
	float _cursorY;
	float _roll;

	// Which controller number this is: 0-3
	u8 _channel;
};
typedef struct WiiController WiiController;

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Initialize a controller and set a channel for it.
 * @param channel Channel of the controller. Valid values [0,3]
 */
void WiiController_Init(WiiController* controller, u8 channel);

/**
 * @brief Zero all inputs on a controller.
 * @details This releases all buttons and sets the joystick and roll to initial position.
 */
void WiiController_ZeroAllInputs(WiiController* controller);

/**
 * @brief Prepares controller for input.
 * @details Releases all buttons and clears the list of released buttons. Call this before any input and code that reads controller state.
 */
void WiiController_StartFrame(WiiController* controller);

// Button values are same as in <wiiuse/wpad.h>
/**
 * @brief Tells if a button was pressed down this frame.
 * @param buttonEnum The button to query.
 * @return True if the given button was pressed down this frame.
 */

bool WiiController_ButtonPress(WiiController* controller, int buttonEnum);
/**
 * @brief Tells if a button was released this frame.
 * @param buttonEnum The button to query.
 * @return True if the given button was released this frame.
 */
bool WiiController_ButtonRelease(WiiController* controller, int buttonEnum);

/**
 * @brief Tells if a button is down.
 * @param buttonEnum The button to query.
 * @return True if the given button was pressed down this frame or before.
 */
bool WiiController_ButtonHeld(WiiController* controller, int buttonEnum);

/**
 * @brief Returns the position of the cursor.
 * @return Position of the cursor in pixels.
 */
vec2 WiiController_GetCursorPosition(WiiController* controller);

/**
 * @brief Returns the direction of the nunchuck joystick.
 * @note The returned vector2 is not normalized.
 * @return Direction of the nunchuck joystick.
 */
vec2 WiiController_GetNunchukJoystickDirection(WiiController* controller);

/**
 * @brief Returns the roll reported by the gyroscope in radians.
 * @return Amount of roll in radians [-Pi, Pi]. 0 means no roll.
 */
float WiiController_GetRoll(WiiController* controller);

void _WiiController_SetButtonDown(WiiController* controller, int buttonEnum);
void _WiiController_SetButtonUp(WiiController* controller, int buttonEnum);

#ifdef __cplusplus
}
#endif
