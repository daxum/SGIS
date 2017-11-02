#include <stdexcept>
#include <random>
#include <chrono>
#include "ExtraMath.hpp"

namespace {
	std::mt19937 engine(ExMath::getTimeMillis());
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	std::uniform_int_distribution<int> intDistribution;
}

float ExMath::clamp(float& value, float minimum, float maximum) {
	if (minimum >= maximum) {
		throw std::logic_error("Maximum not greater than minimum!");
	}

	if (value > maximum) {
		value = maximum;
	}

	if (value < minimum) {
		value = minimum;
	}
}

float ExMath::abs(float value) {
	if (value < 0) {
		return -value;
	}

	return value;
}

float ExMath::interpolate(float start, float finish, float percent) {
	return (finish - start) * percent + start;
}

AxisAlignedBB ExMath::interpolateBox(const AxisAlignedBB& start, const AxisAlignedBB& finish, float percent) {
	return AxisAlignedBB(interpolate(start.minX, finish.minX, percent),
						 interpolate(start.minY, finish.minY, percent),
						 interpolate(start.maxX, finish.maxX, percent),
						 interpolate(start.maxY, finish.maxY, percent));
}

float ExMath::randomFloat(float min, float max) {
	return interpolate(min, max, distribution(engine));
}

bool ExMath::randomBool() {
	return (bool)(intDistribution(engine) & 1);
}

int ExMath::randomInt(int min, int max) {
	return intDistribution(engine) % (max - min + 1) + min;
}

double ExMath::getTimeMillis() {
	std::chrono::duration<double, std::ratio<1, 1000>> time = std::chrono::steady_clock::now().time_since_epoch();
	return time.count();
}

float ExMath::min(float val1, float val2) {
	if (val1 < val2) {
		return val1;
	}

	return val2;
}

float ExMath::minMagnitude(float val1, float val2) {
	if (ExMath::min(ExMath::abs(val1), ExMath::abs(val2)) == ExMath::abs(val1)) {
		return val1;
	}

	return val2;
}
