#pragma once

#define SQUARE 0
#define CIRCLE 1

class Figure
{
private:
	int type;
	int x;
	int y;
	int radius;
	int velocityX;
	int velocityY;
	COLORREF color;

public:
	Figure(int radius, int x, int y);
	~Figure();
};

