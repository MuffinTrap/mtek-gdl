#pragma once

/**
 * @file mgdl-controller.h
 * @ingroup input
 * @brief Header for the controller class.
 */

#include "mgdl-types.h"
#include "mgdl-vector.h"

#define MGDL_MAX_CONTROLLERS 4

/**
 * @brief Enum values for all the buttons on the Wii Controller.
 */
enum WiiButtons : u32
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
	ButtonZ	=		(0x0001u << 16),
	ButtonC	=		(0x0002u << 16),

	// Classic controller
	ButtonClassicUp =		(0x0001u<<16),
	ButtonClassicLeft =		(0x0002u<<16),
	ButtonClassicRightShoulder   =		(0x0004u<<16),
	ButtonClassicX =		(0x0008u<<16),
	ButtonClassicA =		(0x0010u<<16),
	ButtonClassicY =		(0x0020u<<16),
	ButtonClassicB =		(0x0040u<<16),
	ButtonClassicLeftShoulder =		(0x0080u<<16),
	ButtonClassicRightTrigger =		(0x0200u<<16),
	ButtonClassicPlus =		(0x0400u<<16),
	ButtonClassicHome =		(0x0800u<<16),
	ButtonClassicMinus =		(0x1000u<<16),
	ButtonClassicLeftTrigger=(0x2000u<<16),
	ButtonClassicDown =		(0x4000u<<16),
	ButtonClassicRight =		(0x8000u<<16),


	ButtonAny = 	0xFFFFFFFu
};
typedef enum WiiButtons WiiButtons;

enum ControllerType
{
	Controller_Wiimote, /**< Wii remote without attachments */
	Controller_Nunchuk /**< Wii remote with Nunchuk */,
	Controller_ClassicController, /**< Wii remote with Pro Controller */
	Controller_Xbox360Pad /**< Wii remote with Nunchuk and Pro Controller */
};
typedef enum ControllerType ControllerType;

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
	float m_rightStickDirectionX;
	float m_rightStickDirectionY;
	float m_cursorX;
	float m_cursorY;
	float m_roll;
	float m_pitch;
	float m_yaw;

	// Which controller number this is: 0-3
	u8 m_channel;
	bool m_isConnected;
	bool m_rightStickMovesPointer;
	float m_pointerSpeed; /**< Pixels per second */
	ControllerType m_type;
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
Vector2 WiiController_GetCursorPosition(WiiController* controller);

/**
 * @brief Sets the position of the cursor.
 * @note Only usable on PC platform with gamepads because it will be overriden on
 * next update on Wii
 * @param controller The controller
 * @param x X position in pixels
 * @param y Y position in pixels
 */
void WiiController_SetCursorPosition(WiiController* controller, float x, float y);

/**
 * @brief Returns the direction of the nunchuck joystick.
 * @note The returned vector2 is not normalized. +Y is towards player or down
 * @return Direction of the nunchuck joystick.
 */
Vector2 WiiController_GetNunchukJoystickDirection(WiiController* controller);

/**
 * @brief Returns the direction of the left thumbstick.
 * @note On Xbox360 pad this is the same as Nunchuk direction
 * The returned vector2 is not normalized. +Y is towards player or down
 * @return Direction of the left thumbstick
 */
Vector2 WiiController_GetLeftStickDirection(WiiController* controller);
/**
 * @brief Returns the direction of the right thumbstick.
 * @note The returned vector2 is not normalized. +Y is towards player or down
 * @return Direction of the right thumbstick
 */
Vector2 WiiController_GetRightStickDirection(WiiController* controller);


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

bool WiiController_HasNunchuk(WiiController* controller);
bool WiiController_HasClassicController(WiiController* controller);


// Private functions


const char* WiiController_GetButtonSymbol(WiiController* controller, u32 buttonEnum);

void WiiController_SetButtonDown(WiiController* controller, u32 buttonEnum);
void WiiController_SetButtonUp(WiiController* controller, u32 buttonEnum);

/**
* @brief Adds the state of another controller to this controller
* @details OR the masks together. Replace direction if greater
* @param dest The controller to modify
* @param source The controller to read from
*/
void WiiController_AddStateFrom(WiiController* dest, WiiController* source);

/**
* @brief Replaces the state of this controller with state of another
* @param dest The controller to modify
* @param source The controller to read from
*/
void WiiController_ReplaceWith(WiiController* dest, WiiController* source);

#ifdef __cplusplus
}
#endif
