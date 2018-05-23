#pragma once
#include "Figure.h"

class Line :public Figure 
{
private:
	Point2D p1;
	Point2D p2;
	CColor stroke;
	float stroke_width;
	float stroke_opacity;
	std::vector<Transform*> m_transform;
public:
	Line() { m_transform.resize(0); };
	Line(xml_node<> *node);
	Line(float x1, float y1, float x2, float y2, CColor s, float sw, float so);

	virtual void drawCImg(CImg<> &canvas);
	virtual void drawGDI(HDC hdc);

	void setLine(float x1, float y1, float x2, float y2, CColor s, float sw, float so);
	void addTransform(Transform *tran);
	Point2D getPoint1() { return p1; };
	Point2D getPoint2() { return p2; };
	~Line() {
		for (int i = 0; i<m_transform.size(); i++)
			if (m_transform[i] != NULL) delete m_transform[i];
	}
};

Line::Line(xml_node<> *node) 
{
	float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	stroke_width = 1, stroke_opacity = 1;
	m_transform.resize(0);
	for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
	{
		if (strcmp(attr->name(), "x1") == 0) x1 = atof(attr->value());
		if (strcmp(attr->name(), "x2") == 0) x2 = atof(attr->value());
		if (strcmp(attr->name(), "y1") == 0) y1 = atof(attr->value());
		if (strcmp(attr->name(), "y2") == 0) y2 = atof(attr->value());
		if (strcmp(attr->name(), "stroke") == 0) stroke = readCColor(attr->value());
		if (strcmp(attr->name(), "stroke-opacity") == 0) stroke_opacity = atof(attr->value());
		if (strcmp(attr->name(), "stroke-width") == 0) stroke_width = atof(attr->value());
		if (strcmp(attr->name(), "transform") == 0) {
			char*t = strstr(attr->value(), "rotate");
			if (t != NULL) m_transform.push_back(readRotate(t));
			t = strstr(attr->value(), "translate");
			if (t != NULL) m_transform.push_back(readTranslate(t));
		}
	}
	p1.set(x1, y1);
	p2.set(x2, y2);
}

void Line::setLine(float x1, float y1, float x2, float y2, CColor s, float width, float opacity)
{
	p1.set(x1, y1);
	p2.set(x2, y2);
	stroke = s;
	stroke_width = width;
	stroke_opacity = opacity;
}
Line::Line(float x1, float y1, float x2, float y2, CColor s, float width, float opacity) {
	p1.set(x1, y1);
	p2.set(x2, y2);
	stroke = s;
	stroke_width = width;
	stroke_opacity = opacity;
	m_transform.resize(0);
}

void Line::addTransform(Transform *tran) {
	m_transform.push_back(tran);
}

void Line::drawCImg(CImg<> &canvas) {
	Point2D a(p1);
	Point2D b(p2);
	for (int i = 0; i<m_transform.size(); i++)
	{
		a = m_transform[i]->transform(a);
		b = m_transform[i]->transform(b);
	}
	unsigned char color[3];
	color[0] = stroke.red;
	color[1] = stroke.green;
	color[2] = stroke.blue;
	for (int i = -stroke_width / 2; i<stroke_width / 2; i++)
		for (int j = -stroke_width / 2; j<stroke_width / 2; j++)
		{
			canvas.draw_line(a.getX() + i, a.getY() + j, b.getX() + i, b.getY() + j, color, stroke_opacity);
		}
}



void Line::drawGDI(HDC hdc)
{
	Graphics graphic(hdc);
	for (int i = m_transform.size() - 1; i >= 0; i--)
	{
		m_transform[i]->transform(graphic);
	}

	Pen pen(Color(stroke_opacity * 255, stroke.red, stroke.green, stroke.blue), stroke_width);
	Point pos1(p1.getX(), p1.getY());
	Point pos2(p2.getX(), p2.getY());
	graphic.DrawLine(&pen, pos1, pos2);

}
