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
	int getType();
	int getX();
	int getY();
	void setPosition(int x, int y);
	void setSquare();
	void setCircle();
	void setRandomColor();
	void setRandomVelocity();
	void setRandomRadius();
	void paint(HDC hdc);
	void update();
	bool collides(Figure figure);
	bool collidesVerticalLine(int x);
	bool collidesHorizontalLine(int y);
	void invertVelocityX();
	void invertVelocityY();
	void increaseVelocity();
	void decreaseVelocity();
	void flipFigure();
};

