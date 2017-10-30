#pragma once

struct AxisAlignedBB {
	float minX;
	float minY;
	float maxX;
	float maxY;

	AxisAlignedBB(float minX, float minY, float maxX, float maxY) :
		minX(minX),
		minY(minY),
		maxX(maxX),
		maxY(maxY) {}

	inline bool intersects(const AxisAlignedBB& other) const {
		return minX < other.maxX && maxX > other.minX && minY < other.maxY && maxY > other.minY;
	}

	inline bool contains(const AxisAlignedBB& other) const {
		return maxX >= other.maxX && minX <= other.minX && minY <= other.minY && maxY >= other.maxY;
	}

	inline void translate(float x, float y) {
		minX += x;
		maxX += x;
		minY += y;
		maxY += y;
	}

	inline void getCenter(float& centerX, float& centerY) const {
		centerX = (minX + maxX) / 2.0f;
		centerY = (minY + maxY) / 2.0f;
	}

	inline float getArea() const {
		return (maxX - minX) * (maxY - minY);
	}

	inline void scale(float x, float y) {
		float centerX = (minX + maxX) / 2.0f;
		float centerY = (minY + maxY) / 2.0f;

		float xdiff = (maxX - centerX) * x - (maxX - centerX);
		float ydiff = (maxY - centerY) * y - (maxY - centerY);

		minX -= xdiff;
		minY -= ydiff;
		maxX += xdiff;
		maxY += ydiff;
	}

	inline float xLength() const {
		return maxX - minX;
	}

	inline float yLength() const {
		return maxY - minY;
	}
};
