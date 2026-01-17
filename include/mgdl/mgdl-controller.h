#pragma once

/**
 * @file mgdl-controller.h
 * @ingroup input
 * @brief Header for the controller class.
 */

#include "mgdl-types.h"
#include "mgdl-vector.h"

/**
 * @brief Enum values for all the buttons on the Wii Controller.
 */
enum WiiButtons
{
	ButtonNone = 	0x0000,
	Button2 = 		0x0001,
	Button1 = 		0x0002,
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
	ButtonC	=		(0x0002 << 16),

	ButtonAny = 	0xFFFF
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
	u32 m_pressedButtons;
	u32 m_releasedButtons;
	u32 m_heldButtons;
	float m_nunchukJoystickDirectionX;
	float m_nunchukJoystickDirectionY;
	float m_cursorX;
	float m_cursorY;
	float m_roll;
	float m_pitch;
	float m_yaw;

	// Which controller number this is: 0-3
	u8 m_channel;
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

bool WiiController_ButtonPress(WiiController* controller, u32 buttonEnum);
/**
 * @brief Tells if a button was released this frame.
 * @param buttonEnum The button to query.
 * @return True if the given button was released this frame.
 */
bool WiiController_ButtonRelease(WiiController* controller, u32 buttonEnum);

/**
 * @brief Tells if a button is down.
 * @param buttonEnum The button to query.
 * @return True if the given button was pressed down this frame or before.
 */
bool WiiController_ButtonHeld(WiiController* controller, u32 buttonEnum);

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
/**
 * @brief Returns the pitch reported by the gyroscope in radians.
 * @return Amount of pitch in radians [-Pi, Pi]. 0 means no roll.
 */
float WiiController_GetPitch(WiiController* controller);
/**
 * @brief Returns the yaw reported by the gyroscope in radians.
 * @return Amount of yaw in radians [-Pi, Pi]. 0 means no roll.
 */
float WiiController_GetYaw(WiiController* controller);

const char* WiiController_GetButtonSymbol(int buttonEnum);

void WiiController_SetButtonDown(WiiController* controller, u32 buttonEnum);
void WiiController_SetButtonUp(WiiController* controller, u32 buttonEnum);

/**
* @brief Adds the state of another controller to this controller
* @details OR the masks together. Replace direction if greater
* param dest The controller to modify
* param source The controller to read from
*/
void WiiController_AddStateFrom(WiiController* dest, WiiController* source);

/**
* @brief Replaces the state of this controller with state of another
* param dest The controller to modify
* param source The controller to read from
*/
void WiiController_ReplaceWith(WiiController* dest, WiiController* source);

#ifdef __cplusplus
}
#endif
