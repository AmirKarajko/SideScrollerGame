#pragma once
#ifndef PICKUP_H
#define PICKUP_H

#include <irrlicht.h>

using namespace irr;
using namespace core;

class Pickup {
public:
	vector3df position;
	bool pickedUp = false;

	Pickup() {
		this->position = vector3df(0, 0, 0);
	}

	Pickup(vector3df position) {
		this->position = position;
	}
};

#endif // !1
