#pragma once

/**
 * @file mgdl-controller.h
 * @brief Header for the controller class.
 */

#include "mgdl-types.h"
#include "mgdl-vector.h"

namespace gdl
{
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

	/**
	 * @brief Class representing the Wii controller.
	 *
	 * This is the Wii controller and nunchuck on Wii and
	 * mouse and keyboard on PC platforms
	 */
	class WiiController
	{
	public:
		WiiController();
		void SetChannelNumber(int channel);
		void StartFrame();
		void ZeroAllInputs();
		// Button values are same as in <wiiuse/wpad.h>
		bool ButtonPress(int buttonEnum);
		bool ButtonRelease(int buttonEnum);
		bool ButtonHeld(int buttonEnum);

		vec2 GetCursorPosition();
		vec2 GetNunchukJoystickDirection(float deadzone);
		vec2 GetLeftStickDirection(float deadzone);

		void SetButtonDown(int buttonEnum);
		void SetButtonUp(int buttonEnum);

		int GetChannel();

		// In rad -PI...PI
		float GetRoll();
		// State
		u32 pressedButtons;
		u32 releasedButtons;
		u32 heldButtons;
		float nunchukJoystickDirectionX;
		float nunchukJoystickDirectionY;
		float nunchukJoystickDeadzone;
		float cursorX;
		float cursorY;
		float roll;

		// Which controller number this is: 0-3
		int channel;
	private:
	};
};
