#pragma once 
#include "Figure.h"

class CText :public Figure
{
private:
	Point2D point;
	CColor fill;
	float font_size;
	char *data;
	vector<Transform*> m_transform;
public:
	CText() { m_transform.resize(0); };
	CText(float x, float y, CColor f, float fs, char *s)
	{
		point.set(x, y);
		fill = f;
		font_size = fs;
		data = new char[strlen(s) + 1];
		strcpy(data, s);
	}
	CText::CText(xml_node<> *node);

	virtual void drawCImg(CImg<>&canvas)
	{
		Point2D a(point);
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
		unsigned char color[3];
		color[0] = fill.red;
		color[1] = fill.green;
		color[2] = fill.blue;
		canvas.draw_text(point.getX(), point.getY() - font_size, data, color, 0, 1, font_size);
	}
	
	virtual void drawGDI(HDC hdc)
	{
		Graphics graphic(hdc);
		for (int i = m_transform.size() - 1; i >= 0; i--)
		{
			m_transform[i]->transform(graphic);
		}

		SolidBrush brush(Color(fill.red, fill.green, fill.blue));
		Font font(&FontFamily(L"Times New Roman"), font_size);
		wchar_t temp[50];
		mbstowcs(temp, data, 50);
		graphic.DrawString(temp, -1, &font, PointF(point.getX(), point.getY() - font_size), &brush);
	}

	void addTransform(Transform* tran) { m_transform.push_back(tran); };

	~CText()
	{
		for (int i = 0; i < m_transform.size(); i++)
		{
			if (m_transform[i] != NULL)
				delete m_transform[i];
		}
	}
};

CText::CText(xml_node<> *node) 
{
	float x = 0, y = 0;
	font_size = 10;
	m_transform.resize(0);
	for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
	{
		if (strcmp(attr->name(), "x") == 0) x = atof(attr->value());
		if (strcmp(attr->name(), "y") == 0) y = atof(attr->value());
		if (strcmp(attr->name(), "font-size") == 0) font_size = atof(attr->value());
		if (strcmp(attr->name(), "fill") == 0) fill = readCColor(attr->value());
		if (strcmp(attr->name(), "transform") == 0) {
			char*t = strstr(attr->value(), "rotate");
			if (t != NULL) m_transform.push_back(readRotate(t));
			t = strstr(attr->value(), "translate");
			if (t != NULL) m_transform.push_back(readTranslate(t));
		}
	}
	data = new char[strlen(node->value()) + 1];
	strcpy(data, node->value());
	point.set(x, y);
}