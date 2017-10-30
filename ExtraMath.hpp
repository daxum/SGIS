#pragma once

#include "AxisAlignedBB.hpp"

namespace ExMath {
	float clamp(float& value, float minimum, float maximum);

	float abs(float value);

	float interpolate(float start, float finish, float percent);

	AxisAlignedBB interpolateBox(const AxisAlignedBB& start, const AxisAlignedBB& finish, float percent);

	float randomFloat(float min, float max);

	bool randomBool();

	int randomInt(int min, int max);

	double getTimeMillis();

	float min(float val1, float val2);

	float minMagnitude(float val1, float val2);
}
