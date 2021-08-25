#pragma once
#ifndef PLAYER_H

#include <irrlicht.h>

using namespace irr;
using namespace core;

class Player {
public:
	vector3df position;

	Player() {
		position = vector3df(0, 0, 0);
	}

	Player(vector3df position) {
		this->position = position;
	}
};

#endif // !PLAYER_H
