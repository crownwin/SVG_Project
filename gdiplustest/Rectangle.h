#pragma once

#include "Figure.h"
#include "HamXuLy.h"

class CRectangle :public Figure 
{
private:
	Point2D Pos;
	float Width;
	float Height;
	CColor Fill;
	CColor Stroke;
	float Stroke_Width;
	float Stroke_Opacity;
	float Fill_Opacity;
	std::vector<Transform*> m_transform;
public:
	CRectangle(float x, float y, float width, float height, CColor stroke, CColor fill, float stroke_width, float stroke_opacity = 1, float fill_opacity = 1);
	CRectangle(xml_node<> *node);
	
	virtual void drawCImg(CImg<> &canvas);
	virtual void drawGDI(HDC hdc);

	void addTransform(Transform *tran);
	Point2D GetPos() { return Pos; };
	~CRectangle() {
		for (int i = 0; i<m_transform.size(); i++)
			if (m_transform[i] != NULL) delete m_transform[i];
	}
};

CRectangle::CRectangle(xml_node<> *node) {
	float x = 0, y = 0;
	Width = Height = 1;
	CColor stroke;
	Stroke_Width = 1;
	Stroke_Opacity = 1;
	Fill_Opacity = 1;
	m_transform.resize(0);
	for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
	{
		if (strcmp(attr->name(), "x") == 0) x = atof(attr->value());
		if (strcmp(attr->name(), "y") == 0) y = atof(attr->value());
		if (strcmp(attr->name(), "width") == 0) Width = atof(attr->value());
		if (strcmp(attr->name(), "height") == 0) Height = atof(attr->value());
		if (strcmp(attr->name(), "stroke") == 0) Stroke = readCColor(attr->value());
		if (strcmp(attr->name(), "fill") == 0) Fill = readCColor(attr->value());
		if (strcmp(attr->name(), "stroke-opacity") == 0) Stroke_Opacity = atof(attr->value());
		if (strcmp(attr->name(), "stroke-width") == 0) Stroke_Width = atof(attr->value());
		if (strcmp(attr->name(), "fill-opacity") == 0) Fill_Opacity = atof(attr->value());
		if (strcmp(attr->name(), "transform") == 0) {
			char*t = strstr(attr->value(), "rotate");
			if (t != NULL) m_transform.push_back(readRotate(t));
			t = strstr(attr->value(), "translate");
			if (t != NULL) m_transform.push_back(readTranslate(t));
		}
	}
	Pos.set(x, y);
}

CRectangle::CRectangle(float x, float y, float width, float height, CColor stroke, CColor fill, float stroke_width, float stroke_opacity, float fill_opacity) {
	Pos.set(x, y);
	Width = width;
	Height = height;
	Stroke = stroke;
	Fill = fill;
	Stroke_Width = stroke_width;
	Stroke_Opacity = stroke_opacity;
	Fill_Opacity = fill_opacity;
	m_transform.resize(0);
}

void CRectangle::addTransform(Transform *tran) {
	m_transform.push_back(tran);
}

void CRectangle::drawCImg(CImg<> &canvas) {
	Point2D a(Pos);
	Point2D b(Pos.getX() + Width, Pos.getY());
	Point2D c(Pos.getX() + Width, Pos.getY() + Height);
	Point2D d(Pos.getX(), Pos.getY() + Height);
	for (int i = 0; i<m_transform.size(); i++)
	{
		a = m_transform[i]->transform(a);
		b = m_transform[i]->transform(b);
		c = m_transform[i]->transform(c);
		d = m_transform[i]->transform(d);
	}
	std::vector<Point2D> points;
	points.resize(0);
	points.push_back(a);
	points.push_back(b);
	points.push_back(c);
	points.push_back(d);
	CPolygon* p = new CPolygon(points, Stroke, Fill, Stroke_Width, Stroke_Opacity, Fill_Opacity);
	p->drawCImg(canvas);
}


void CRectangle::drawGDI(HDC hdc) {
	Graphics graphic(hdc);
	for (int i = m_transform.size() - 1; i >= 0; i--)
	{
		m_transform[i]->transform(graphic);
	}
	SolidBrush brush(Color(Fill_Opacity * 255, Fill.red, Fill.green, Fill.blue));
	graphic.FillRectangle(&brush, Pos.getX(), Pos.getY(), Width, Height);
	Pen pen(Color(Stroke_Opacity * 255, Stroke.red, Stroke.green, Stroke.blue), Stroke_Width);
	graphic.DrawRectangle(&pen, Pos.getX(), Pos.getY(), Width, Height);
}