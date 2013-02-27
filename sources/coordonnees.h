#pragma once
class Coordonnees
{
protected :
	int _x;
	int _y;
public :
	Coordonnees(int x = 0,int y = 0);
	Coordonnees(const Coordonnees &);
	void Set(int x, int y);
	int getX(void) const;
	int getY(void) const;
	Coordonnees & operator=(const Coordonnees &);
	bool operator==(const Coordonnees &) const;
	Coordonnees operator+(const Coordonnees &) const;
	Coordonnees operator-(const Coordonnees &) const;
	bool operator<(const Coordonnees &) const;
};