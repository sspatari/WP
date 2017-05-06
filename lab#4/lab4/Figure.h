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
	Figure(int x, int y, int radius);
	~Figure();
	void setPosition(int x, int y);
	void setSquare();
	void setCircle();
	void setRandomColor();
	void setRandomVelocity();
	void setRandomRadius();
	int getType();
	int getX();
	int getY();
	void paint(HDC hdc);
	void updatePosition();
	bool collides(Figure figure);
	bool collidesVerticalBorder(int x);
	bool collidesHorizontalBorder(int y);
	void invertVelocityX();
	void invertVelocityY();
	void increaseVelocity();
	void decreaseVelocity();
	void flipFigure();
};

