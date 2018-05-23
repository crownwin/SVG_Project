#pragma once
#include "stdafx.h"
#include "gdiplustest.h"
#include "Transform.h"

void ChuanHoa(char* source)
{
	int t = 0;
	while (t<strlen(source))
	{
		if (source[t] == ' ') {
			strcpy(source + t, source + t + 1);
			t--;
		}
		t++;
	}
}

void ChuanHoa1(char* source)
{
	int t = 0;
	while (source[0] == ' ') strcpy(source, source + 1);;
	while (t<strlen(source) - 1)
	{
		if (source[t] == ' '&&source[t + 1] == ' ') {
			strcpy(source + t, source + t + 1);
			t--;
		}
		t++;
	}
	if (source[strlen(source) - 1] == ' ') source[strlen(source) - 1] = 0;
}

CColor readCColor(char* source)
{
	char* t;
	CColor b;
	t = strtok(source, " (,)");
	t = strtok(NULL, " (,)");
	b.red = atoi(t);
	t = strtok(NULL, " (,)");
	b.green = atoi(t);
	t = strtok(NULL, " (,)");
	b.blue = atoi(t);
	return b;
}

Transform* readTranslate(char* source)
{
	char* t;
	float x = 0, y = 0;
	t = strtok(source, " (,)");
	t = strtok(NULL, " (,)");
	x = atof(t);
	t = strtok(NULL, " (,)");
	y = atof(t);
	Transform* kq = new Translate(x, y);
	return kq;
}

Transform* readRotate(char* source)
{
	char* t;
	float x = 0;
	t = strtok(source, " (,)");
	t = strtok(NULL, " (,)");
	x = atof(t);
	Transform* kq = new Rotate(x);
	return kq;
}

vector<Point2D> readPoints(char* source)
{
	vector<Point2D> kq;
	kq.resize(0);
	float x = 0, y = 0;
	Point2D temp;
	char* t;
	t = strtok(source, " ,");
	x = atof(t);
	t = strtok(NULL, " ,");
	y = atof(t);
	temp.set(x, y);
	kq.push_back(temp);
	t = strtok(NULL, " ,");
	while (t != NULL)
	{
		x = atof(t);
		t = strtok(NULL, " ,");
		y = atof(t);
		temp.set(x, y);
		kq.push_back(temp);
		t = strtok(NULL, " ,");
	}
	return kq;
}