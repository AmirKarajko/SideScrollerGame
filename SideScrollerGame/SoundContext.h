#pragma once
#ifndef SOUNDCONTEXT_H
#define SOUNDCONTEXT_H

#include <iostream>

#pragma comment(lib, "irrKlang.lib")

#include <irrKlang.h>

using namespace irrklang;

class SoundContext {
private:
	ISoundEngine* engine;

public:
	SoundContext() {
		engine = createIrrKlangDevice();
	}

	void drop() {
		engine->drop();
	}

	void playPickupSound() {
		engine->play2D("assets/sounds/pickup.wav");
	}
};

#endif