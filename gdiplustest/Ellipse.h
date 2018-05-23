#pragma once
#include "Figure.h"

class CEllipse : public Figure
{
protected:
	Point2D p;
	float rx;
	float ry;
	CColor fill;
	CColor stroke;
	float stroke_width;
	float stroke_opacity;
	float fill_opacity;
	vector<Transform*> m_transform;
public:
	CEllipse(float x, float y, float r1, float r2, CColor s, CColor f, float sw, float so = 1, float fo = 1)
	{
		p.set(x, y);
		rx = r1;
		ry = r2;
		fill = f;
		stroke = s;
		stroke_width = sw;
		stroke_opacity = so;
		fill_opacity = fo;
		m_transform.resize(0);
	}

	CEllipse::CEllipse(xml_node<> *node);

	virtual void drawCImg(CImg<> &canvas);
	virtual void drawGDI(HDC hdc);

	virtual void addTransform(Transform *tran)
	{
		m_transform.push_back(tran);
	}

	~CEllipse()
	{
		for (int i = 0; i < m_transform.size(); i++)
		{
			if (m_transform[i] != NULL)
				delete m_transform[i];
		}
	}
};

void CEllipse::drawCImg(CImg<> &canvas)
{
	Point2D a(p);
	int angle = 0;
	for (int i = 0; i<m_transform.size(); i++)
	{
		a = m_transform[i]->transform(a);
		if (dynamic_cast<Rotate*>(m_transform[i]))
		{
			Rotate *temp = (Rotate*)m_transform[i];
			angle = temp->getAngle();
		}

	}
	unsigned char color1[3], color2[3];
	color2[0] = fill.red;
	color2[1] = fill.green;
	color2[2] = fill.blue;
	color1[0] = stroke.red;
	color1[1] = stroke.green;
	color1[2] = stroke.blue;
	for (int i = 0; i<stroke_width; i++)
		canvas.draw_ellipse(a.getX(), a.getY(), rx + i, ry + i, angle, color1, stroke_opacity, 1);
	canvas.draw_ellipse(a.getX(), a.getY(), rx, ry, angle, color2, fill_opacity);
}

void CEllipse::drawGDI(HDC hdc) 
{
	Graphics graphic(hdc);
	for (int i = m_transform.size() - 1; i >= 0; i--)
	{
		m_transform[i]->transform(graphic);
	}
	SolidBrush brush(Color(fill_opacity * 255, fill.red, fill.green, fill.blue));
	graphic.FillEllipse(&brush, Rect(p.getX() - rx, p.getY() - ry, 2 * rx, 2 * ry));
	
	Pen pen(Color(stroke_opacity * 255, stroke.red, stroke.green, stroke.blue), stroke_width);
	graphic.DrawEllipse(&pen, Rect(p.getX() - rx, p.getY() - ry, 2 * rx, 2 * ry));
}

CEllipse::CEllipse(xml_node<> *node) 
{
	float x = 0, y = 0;
	stroke_width = 1;
	stroke_opacity = 1;
	fill_opacity = 1;
	m_transform.resize(0);
	for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
	{
		if (strcmp(attr->name(), "cx") == 0) x = atof(attr->value());
		if (strcmp(attr->name(), "cy") == 0) y = atof(attr->value());
		if (strcmp(attr->name(), "rx") == 0) rx = atof(attr->value());
		if (strcmp(attr->name(), "ry") == 0) ry = atof(attr->value());
		if (strcmp(attr->name(), "stroke") == 0) stroke = readCColor(attr->value());
		if (strcmp(attr->name(), "fill") == 0) fill = readCColor(attr->value());
		if (strcmp(attr->name(), "stroke-opacity") == 0) stroke_opacity = atof(attr->value());
		if (strcmp(attr->name(), "stroke-width") == 0) stroke_width = atof(attr->value());
		if (strcmp(attr->name(), "fill-opacity") == 0) fill_opacity = atof(attr->value());
		if (strcmp(attr->name(), "transform") == 0) 
		{
			char*t = strstr(attr->value(), "rotate");
			if (t != NULL) m_transform.push_back(readRotate(t));
			t = strstr(attr->value(), "translate");
			if (t != NULL) m_transform.push_back(readTranslate(t));
		}
	}
	p.set(x, y);
}