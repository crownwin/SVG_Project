#pragma once
#include "Polyline.h"

class CPolygon :public CPolyline
{
public:
	CPolygon(vector<Point2D> p, CColor s, CColor f, float sw, float so = 1, float fo = 1) :CPolyline (p, s, f, sw, so, fo) {};
	CPolygon(xml_node<> *node) :CPolyline(node) {};

	virtual void drawCImg(CImg<>&canvas)
	{
		Point2D a(m_p[0]);
		Point2D b(m_p[m_p.size() - 1]);
		for (int i = 0; i<m_transform.size(); i++)
		{
			a = m_transform[i]->transform(a);
			b = m_transform[i]->transform(b);
		}
		Line l(a.getX(), a.getY(), b.getX(), b.getY(), stroke, stroke_width, stroke_opacity);
		CPolyline::drawCImg(canvas);
		l.drawCImg(canvas);
	}
	virtual void drawGDI(HDC hdc)
	{
		Graphics graphic(hdc);

		for (int i = m_transform.size() - 1; i >= 0; i--)
		{
			m_transform[i]->transform(graphic);
		}

		SolidBrush brush(Color(fill_opacity * 255, fill.red, fill.green, fill.blue));
		Point *point1 = new Point[m_p.size()];

		for (int i = 0; i<m_p.size(); i++)
		{
			point1[i].X = m_p[i].getX();
			point1[i].Y = m_p[i].getY();
		}

		graphic.FillPolygon(&brush, point1, m_p.size());
		Pen pen(Color(stroke_opacity * 255, stroke.red, stroke.green, stroke.blue), stroke_width);

		graphic.DrawPolygon(&pen, point1, m_p.size());
	}
};