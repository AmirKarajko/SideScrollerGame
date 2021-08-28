#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <irrlicht.h>

using namespace irr;
using namespace core;

class Player {
public:
	vector3df position;
	float speed = 20.0f;
	float jumpSpeed = 50.0f;

	const int IDLE = 0, WALK = 1, JUMP = 2; // Jump Animation Missing
	int state = IDLE;

	Player() {
		position = vector3df(0, 0, 0);
	}

	Player(vector3df position) {
		this->position = position;
	}
};

#endif // !PLAYER_H
