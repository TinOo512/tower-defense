#pragma once
class Objet
{
protected :
	sf::Sprite Sprite;
	int Width;
	int Height;
	int Type;
	int numInstance;
	bool isMove;
	Coordonnees PosInitial;
	Coordonnees PosFinal;

public:
	Objet(void);
	~Objet(void);
	sf::Sprite getSprite(void);
	Coordonnees getSpritePos(int type = 0);
	int getType(void);
	void setNumInstance(int _numInstance);
	int getNumInstance(void);
};