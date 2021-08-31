#pragma once
#ifndef CLOTHESMENU_H
#define CLOTHESMENU_H

#include <iostream>

#include<irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

class ClothesMenu {
private:
	IAnimatedMeshSceneNode* playerNode;
	IGUIWindow* window;

	ITexture* originalTexture;

public:
	ClothesMenu() {

	}

	ClothesMenu(IrrlichtDevice* device) {
		IGUIEnvironment* guienv = device->getGUIEnvironment();

		this->window = guienv->addWindow(rect<s32>(100, 100, 300, 200), false, L"Clothes Menu");
		this->window->setVisible(false);

		guienv->addButton(rect<s32>(10, 30, 110, 55), this->window, 500, L"T Shirt");

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

	void toggleOpen() {
		if (!this->window) {
			return;
		}

		if (this->window->isVisible()) {
			close();
		}
		else {
			open();
		}
	}

	void setPlayer(IAnimatedMeshSceneNode* playerNode) {
		this->playerNode = playerNode;
		this->originalTexture = this->playerNode->getMaterial(0).getTexture(0);
	}

	void setRandomTShirtColor() {
		u32* p = (u32*)this->originalTexture->lock();

		for (u32 x = 0; x < this->originalTexture->getSize().Width; x++) {
			for (u32 y = 0; y < this->originalTexture->getSize().Height; y++) {
				p[y * this->originalTexture->getSize().Height + x] = SColor(255, 255, 0, 0).color;
			}
		}

		this->originalTexture->unlock();

		this->playerNode->setMaterialTexture(0, this->originalTexture);
	}
};

#endif