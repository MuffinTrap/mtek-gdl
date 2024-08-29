#pragma once

#include "mgdl-types.h"

namespace gdl
{
	struct ControllerVec2
	{
		float xAxis;
		float yAxis;
	};

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

		gdl::ControllerVec2 GetCursorPosition();
		gdl::ControllerVec2 GetNunchukJoystickDirection(float deadzone);
		gdl::ControllerVec2 GetLeftStickDirection(float deadzone);

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
	};
};
