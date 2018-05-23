#pragma once
#include "Transform.h"
#include "Color.h"
#include "HamXuLy.h"

class Figure
{
public:
	virtual void drawGDI(HDC hdc) = 0;
	virtual void drawCImg(CImg<> &canvas) = 0;
};