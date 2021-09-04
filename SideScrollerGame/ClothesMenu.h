#pragma once
#ifndef CLOTHESMENU_H
#define CLOTHESMENU_H

#include <iostream>

#include<irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class ClothesMenu {
private:
	IVideoDriver* driver;

	IAnimatedMeshSceneNode* playerNode;
	IGUIWindow* window;

	ITexture* prevTexture;

	IFileSystem* fileSystem;

	IReadFile* imageFile;
	IImage* textureImage;
	dimension2d<u32> dimension;
	IImage* tempImage;

	SColor tShirtColor = SColor(255, 100, 0, 0);
	SColor pantsColor = SColor(255, 0, 0, 100);


	IImage* TextureImage(ITexture* texture) {
		driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

		IImage* image = driver->createImageFromData(
			texture->getColorFormat(),
			texture->getSize(),
			texture->lock(E_TEXTURE_LOCK_MODE::ETLM_READ_WRITE),
			true
		);

		texture->unlock();

		image->drop();

		driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);

		return image;
	}

	ITexture* ImageTexture(IImage* image) {
		prevTexture = driver->addTexture("texture", image);
		prevTexture->grab();
		return prevTexture;
	}

	void initTextureImage() {
		if (prevTexture) {
			driver->removeTexture(prevTexture);
			prevTexture->drop();
		}

		if (!textureImage) {
			imageFile = fileSystem->createAndOpenFile("assets/player/player.bmp");

			ITexture* texture = driver->getTexture(imageFile);

			imageFile->drop();

			textureImage = TextureImage(texture);

			dimension = textureImage->getDimension();

			tempImage = driver->createImage(ECF_A8R8G8B8, dimension);
		}
	}

	SColor getRandomColor() {
		u32 r = rand() % 256;
		u32 g = rand() % 256;
		u32 b = rand() % 256;
		return SColor(255, r, g, b);
	}

	void setRandomColor(SColor* color) {
		SColor r = getRandomColor();
		color->set(r.getAlpha(), r.getRed(), r.getGreen(), r.getBlue());
	}

	void setColorsOnTexture() {
		for (u32 x = 0; x < dimension.Width; x++) {
			for (u32 y = 0; y < dimension.Height; y++) {
				SColor pixel = this->textureImage->getPixel(x, y);
				if (x >= 0 && x < 32 && y >= 0 && y < 32) {
					tempImage->setPixel(x, y, pantsColor);
				}
				else if (x >= 32 && x < 64 && y >= 0 && y < 32) {
					tempImage->setPixel(x, y, tShirtColor);
				}
				else {
					tempImage->setPixel(x, y, pixel);
				}
			}
		}

		ITexture* newTexture = ImageTexture(tempImage);
		playerNode->setMaterialTexture(0, newTexture);
	}

public:
	ClothesMenu() {

	}

	ClothesMenu(IrrlichtDevice* device, IVideoDriver* driver) {
		this->driver = driver;

		this->fileSystem = device->getFileSystem();

		IGUIEnvironment* guienv = device->getGUIEnvironment();

		this->window = guienv->addWindow(rect<s32>(100, 100, 220, 200), false, L"Clothes Menu");
		this->window->setVisible(false);

		guienv->addButton(rect<s32>(10, 30, 110, 55), this->window, 500, L"T-Shirt");
		guienv->addButton(rect<s32>(10, 65, 110, 90), this->window, 501, L"Pants");

		this->window->getCloseButton()->setVisible(false);


		this->imageFile = this->fileSystem->createAndOpenFile("player.bmp");
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
	}

	void setRandomTShirtColor() {
		initTextureImage();
		setRandomColor(&tShirtColor);
		setColorsOnTexture();
	}

	void setRandomPantsColor() {
		initTextureImage();
		setRandomColor(&pantsColor);
		setColorsOnTexture();
	}
};

#endif