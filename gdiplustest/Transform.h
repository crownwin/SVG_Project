#pragma once
#include "stdafx.h"
#include "gdiplustest.h"
#include "Point2D.h"

#define PI	3.14159265

class Transform
{
public:
	virtual Point2D transform(Point2D a) = 0;
	virtual void transform(Graphics &graphic) = 0;
};

class Translate :public Transform
{
private:
	float dx;
	float dy;
public:
	Translate(float a, float b) { dx = a; dy = b; };
	
	virtual Point2D transform(Point2D a)
	{
		Point2D kq;
		kq.set(a.getX() + dx, a.getY() + dy);
		return kq;
	}

	void transform(Graphics &graphic) 
	{
		graphic.TranslateTransform(dx, dy);
	};
};

class Rotate : public Transform
{
private:
	float angle;
public:
	Rotate(float a) { angle = a; };
	virtual Point2D transform(Point2D p)
	{
		Point2D kq;
		float m = p.getX();
		float n = p.getY();
		float r = sqrt(m*m + n*n);
		float a = atan(m / n) - angle * PI / 180;
		kq.set(r*sin(a), r*cos(a));
		return kq;
	}
	float getAngle() { return angle; };

	void transform(Graphics &graphic) { graphic.RotateTransform(angle); };
};