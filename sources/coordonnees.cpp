#include "includes.h"

//Constructeurs
Coordonnees::Coordonnees(int x,int y) : _x(x), _y(y)
{}

Coordonnees::Coordonnees(const Coordonnees & autre) : _x(autre._x), _y(autre._y)
{}

void Coordonnees::Set(int x, int y)
{
	this->_x = x;
	this->_y = y;
}

//Accesseurs
int Coordonnees::getX(void) const
{
	return _x;
}

int Coordonnees::getY(void) const
{
	return _y;
}

//Opérateurs
Coordonnees & Coordonnees::operator=(const Coordonnees & autre)
{
	_x = autre._x;
	_y = autre._y;

	return *this;
}

bool Coordonnees::operator==(const Coordonnees & autre) const
{
	if((_x == autre._x) && (_y == autre._y)) return true;

	return false;
}

Coordonnees Coordonnees::operator+(const Coordonnees & autre) const
{
	return Coordonnees(_x + autre._x, _y + autre._y);
}

Coordonnees Coordonnees::operator-(const Coordonnees & autre) const
{
	return Coordonnees(_x - autre._x, _y - autre._y);
}

bool Coordonnees::operator<(const Coordonnees & autre) const
{
	return (_x != autre._x) ? (_x < autre._x) : (_y < autre._y);
}