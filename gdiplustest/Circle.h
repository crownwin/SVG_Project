#pragma once
#include "Ellipse.h"

class CCircle :public CEllipse
{
public:
	CCircle(float x, float y, float r, CColor s, CColor f, float sw, float so = 1, float fo = 1) : CEllipse(x, y, r, r, s, f, sw, so, fo) {};
	CCircle(xml_node<> *node) : CEllipse(node) 
	{
		for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
		{
			if (strcmp(attr->name(), "r") == 0) rx = ry = atof(attr->value());
		}
	}

	virtual void drawCImg(CImg<> &canvas) { CEllipse::drawCImg(canvas); };
	virtual void drawGDI(HDC hdc)
	{
		CEllipse::drawGDI(hdc);
	}
};
