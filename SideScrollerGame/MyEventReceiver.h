#pragma once
#ifndef MYEVENTRECEIVER_H

#include <irrlicht.h>

using namespace irr;

class MyEventReceiver : public IEventReceiver {
public:
	virtual bool OnEvent(const SEvent& event) {
		if (event.EventType == EET_KEY_INPUT_EVENT) {
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
		}
		return false;
	}

	virtual bool isKeyDown(EKEY_CODE keyCode) const {
		return KeyIsDown[keyCode];
	}

	MyEventReceiver() {
		for (u32 i = 0; i < KEY_KEY_CODES_COUNT; i++) {
			KeyIsDown[i] = false;
		}
	}

private:
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

#endif // !MYEVENTRECEIVER_H
