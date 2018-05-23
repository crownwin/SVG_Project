#pragma once
#include "Figure.h"
#include "Line.h"

class CPolyline : public Figure
{
protected:
	vector<Point2D> m_p;
	CColor stroke;
	CColor fill;
	float stroke_width;
	float stroke_opacity;
	float fill_opacity;
	vector<Transform*> m_transform;
public:
	CPolyline(vector<Point2D> p, CColor s, CColor f, float sw, float so = 1, float fo = 1)
	{
		m_p = p;
		stroke = s;
		fill = f;
		stroke_width = sw;
		stroke_opacity = so;
		fill_opacity = fo;
		m_transform.resize(0);
	}

	CPolyline::CPolyline(xml_node<> *node);


	virtual void drawCImg(CImg<> &canvas);
	virtual void drawGDI(HDC hdc);

	void addTransform(Transform* tran)
	{
		m_transform.push_back(tran);
	}

	~CPolyline()
	{
		for (int i = 0; i < m_transform.size(); i++)
		{
			if (m_transform[i] != NULL)
				delete m_transform[i];
		}
	}
};

void CPolyline::drawCImg(CImg<> &canvas) 
{
	CImg<> Points(m_p.size(), 2);
	std::vector<Point2D> a;
	a.resize(0);
	for (int i = 0; i<m_p.size(); i++)
	{
		Point2D temp(m_p[i]);
		for (int j = 0; j<m_transform.size(); j++)
		{
			temp = m_transform[j]->transform(temp);
		}
		a.push_back(temp);
	}
	for (int i = 0; i<a.size(); i++)
	{
		Points(i, 0) = a[i].getX();
		Points(i, 1) = a[i].getY();
	}
	unsigned char color[3];
	color[0] = fill.red;
	color[1] = fill.green;
	color[2] = fill.blue;
	canvas.draw_polygon(Points, color, fill_opacity);

	Line l;
	for (int i = 0; i<a.size() - 1; i++)
	{
		l.setLine(a[i].getX(), a[i].getY(), a[i + 1].getX(), a[i + 1].getY(), stroke, stroke_width, stroke_opacity);
		l.drawCImg(canvas);
	}
}

void CPolyline::drawGDI(HDC hdc)
{
	Graphics graphic(hdc);
	
	for (int i = m_transform.size() - 1; i >= 0; i--)
	{
		m_transform[i]->transform(graphic);
	}
	
	SolidBrush brush(Color(fill_opacity * 255, fill.red, fill.green, fill.blue));
	Point *point1 = new Point[m_p.size() + 1];
	
	for (int i = 0; i<m_p.size(); i++)
	{
		point1[i].X = m_p[i].getX();
		point1[i].Y = m_p[i].getY();
	}
	
	graphic.FillPolygon(&brush, point1, m_p.size());
	Pen pen(Color(stroke_opacity * 255, stroke.red, stroke.green, stroke.blue), stroke_width);
	
	for (int i = 0; i<m_p.size() - 1; i++)
	{
		graphic.DrawLine(&pen, Point(m_p[i].getX(), m_p[i].getY()), Point(m_p[i + 1].getX(), m_p[i + 1].getY()));
	}
}

CPolyline::CPolyline(xml_node<> *node) 
{
	float x = 0, y = 0;
	stroke_width = 1;
	stroke_opacity = 1;
	fill_opacity = 1;
	stroke.red = stroke.green = stroke.blue = 0;
	fill.red = fill.green = fill.blue = 0;
	m_transform.resize(0);
	m_p.resize(0);
	for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
	{
		if (strcmp(attr->name(), "stroke") == 0) stroke = readCColor(attr->value());
		if (strcmp(attr->name(), "fill") == 0) fill = readCColor(attr->value());
		if (strcmp(attr->name(), "stroke-opacity") == 0) stroke_opacity = atof(attr->value());
		if (strcmp(attr->name(), "stroke-width") == 0) stroke_width = atof(attr->value());
		if (strcmp(attr->name(), "fill-opacity") == 0) fill_opacity = atof(attr->value());
		if (strcmp(attr->name(), "points") == 0) m_p = readPoints(attr->value());
		if (strcmp(attr->name(), "transform") == 0) 
		{
			char*t = strstr(attr->value(), "rotate");
			if (t != NULL) m_transform.push_back(readRotate(t));
			t = strstr(attr->value(), "translate");
			if (t != NULL) m_transform.push_back(readTranslate(t));
		}
	}
}