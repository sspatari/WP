#include "stdafx.h"
#include "Figure.h"

Figure::Figure(int x, int y, int radius)
{
	this->x = x;
	this->y = y;
	this->radius = radius;
}

Figure::~Figure()
{
}

void Figure::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Figure::setSquare()
{
	type = SQUARE;
}

void Figure::setCircle()
{
	type = CIRCLE;
}

void Figure::setRandomColor()
{
	color = RGB(rand() % 256, rand() % 256, rand() % 256);
}

void Figure::setRandomVelocity()
{
	velocityX = (rand() % 20 + 1) * ((rand() % 2) ? -1 : 1);
	velocityY = (rand() % 20 + 1) * ((rand() % 2) ? -1 : 1);
}

void Figure::setRandomRadius()
{
	radius = 10 + rand() % 10;
}

int Figure::getType()
{
	return this->type;
}

int Figure::getX()
{
	return x;
}

int Figure::getY()
{
	return y;
}

void Figure::paint(HDC hdc)
{
	SelectObject(hdc, CreateSolidBrush(color));
	// used for debuging
	//wchar_t str[256]; 
	//wsprintf(str, L"color: %d \n", color);
	//OutputDebugString(str);
	// delete if not necesarry
	if (type == CIRCLE) {
		Ellipse(hdc,
			x - radius,
			y - radius,
			x + radius,
			y + radius
		);
	}
	else if (type == SQUARE)
	{
		Rectangle(hdc,
			x - radius,
			y - radius,
			x + radius,
			y + radius
		);
	}
}

void Figure::updatePosition()
{
	x += velocityX;
	y += velocityY;
}

bool Figure::collides(Figure figure)
{
	if (sqrt(pow(x - figure.x, 2) + pow(y - figure.y, 2)) <= radius + figure.radius)
		return true;
	return false;
}

bool Figure::collidesVerticalBorder(int x)
{
	if (x == 0)
	{
		if (this->x <= 0)
			return true;
	}
	else
	{
		if (this->x >= x)
			return true;
	}
	return false;
}

bool Figure::collidesHorizontalBorder(int y)
{
	if (y == 0)
	{
		if (this->y <= 0)
			return true;
	}
	else
	{
		if (this->y >= y)
			return true;
	}
	return false;
}

void Figure::invertVelocityX()
{
	velocityX *= -1;
}

void Figure::invertVelocityY()
{
	velocityY *= -1;
}

void Figure::increaseVelocity()
{	//velocity will not increase more then specified amount
	if (abs(velocityX) < 60)
		velocityX += (velocityX > 0) ? 1 : -1;
	if (abs(velocityY) < 60)
		velocityY += (velocityY > 0) ? 1 : -1;
}

void Figure::decreaseVelocity()
{	//velocity will never decrese to zero on any of axes, min value will be 1
	if (abs(velocityX) > 1)
		velocityX -= (velocityX > 0) ? 1 : -1;
	if (abs(velocityY) > 1)
		velocityY -= (velocityY > 0) ? 1 : -1;
}

void Figure::flipFigure()
{
	if (type == CIRCLE)
		type = SQUARE;
	else if(type == SQUARE)
		type = CIRCLE;
	//add here if more figure will be available
}
