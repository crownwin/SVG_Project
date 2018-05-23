#pragma once

class Point2D
{
private:
	float x;
	float y;
public:
	Point2D() { x = y = 0; };
	Point2D(float a, float b) { x = a; y = b; };
	Point2D(const Point2D &a) { x = a.x; y = a.y; };

	void set(float a, float b) { x = a; y = b; };
	float getX() { return x; };
	float getY() { return y; };
};