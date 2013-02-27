#include "includes.h"

Objet::Objet(void)
{}

Objet::~Objet(void)
{}

sf::Sprite Objet::getSprite(void)
{
	return this->Sprite;
}

Coordonnees Objet::getSpritePos(int type){
	grille *Grille = grille::getInstance();

	if(type == 0) return Coordonnees(this->Sprite.GetPosition().x, this->Sprite.GetPosition().y);
	else return Grille->getPos(Coordonnees(this->Sprite.GetPosition().x, this->Sprite.GetPosition().y), type);
}

int Objet::getType(void)
{
	return this->Type;
}

void Objet::setNumInstance(int _numInstance)
{
	this->numInstance = _numInstance;
}

int Objet::getNumInstance(void)
{
	return this->numInstance;
}