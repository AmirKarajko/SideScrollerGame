#pragma once
#ifndef CLOTHESMENU_H
#define CLOTHESMENU_H

#include<irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

class ClothesMenu {
private:
	IGUIWindow* window;

public:
	ClothesMenu() {

	}

	ClothesMenu(IrrlichtDevice* device) {
		this->window = device->getGUIEnvironment()->addWindow(rect<s32>(100, 100, 300, 200), false, L"Clothes Menu");
		this->window->setVisible(false);

		this->window->getCloseButton()->setVisible(false);
	}

	bool isOpen() {
		return this->window->isVisible();
	}

	void open() {
		if (this->window) {
			this->window->setVisible(true);
		}
	}

	void close() {
		if (this->window) {
			this->window->setVisible(false);
		}
	}
};

#endif