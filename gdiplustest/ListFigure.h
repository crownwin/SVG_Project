#pragma once
#include "stdafx.h"
#include "gdiplustest.h"
#include "Figure.h"

class ListFigure
{
private:
	vector<Figure*> m_list;
public:
	ListFigure() { m_list.resize(0); };
	void addFigure(Figure* a) { m_list.push_back(a); };

	void drawGDI(HDC hdc)
	{
		for (int i = 0; i < m_list.size(); i++)
		{
			m_list[i]->drawGDI(hdc);
		}
	}
	void drawCImg(CImg<> &canvas)
	{
		for (int i = 0; i < m_list.size(); i++)
		{
			CImg<> tmp(canvas);
			tmp.fill(255);
			m_list[i]->drawCImg(tmp);
			canvas += tmp;
		}
	}

	~ListFigure()
	{
		for (int i = 0; i < m_list.size(); i++)
		{
			if (m_list[i] != NULL)
				delete m_list[i];
		}
	}
};