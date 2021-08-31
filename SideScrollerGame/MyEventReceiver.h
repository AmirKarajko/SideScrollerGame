#pragma once
#ifndef MYEVENTRECEIVER_H
#define MYEVENTRECEIVER_H

#include <irrlicht.h>

#include "ClothesMenu.h"

using namespace irr;

class MyEventReceiver : public IEventReceiver {
public:
	virtual bool OnEvent(const SEvent& event) {
		if (event.EventType == EET_KEY_INPUT_EVENT) {
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
		}

		if (event.EventType == EET_KEY_INPUT_EVENT
			&& event.KeyInput.Key == KEY_F10
			&& !event.KeyInput.PressedDown) {
			clothesMenu->toggleOpen();
		}

		if (event.EventType == EET_GUI_EVENT) {
			if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
				s32 id = event.GUIEvent.Caller->getID();
				if (id == 500) {
					clothesMenu->setRandomTShirtColor();
				}
			}
		}

		return false;
	}

	virtual bool isKeyDown(EKEY_CODE keyCode) const {
		return KeyIsDown[keyCode];
	}

	MyEventReceiver(ClothesMenu* clothesMenu) {
		this->clothesMenu = clothesMenu;

		for (u32 i = 0; i < KEY_KEY_CODES_COUNT; i++) {
			KeyIsDown[i] = false;
		}
	}

private:
	ClothesMenu* clothesMenu;
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

#endif // !MYEVENTRECEIVER_H
