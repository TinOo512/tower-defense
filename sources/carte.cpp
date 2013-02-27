#include "includes.h"

carte::carte(void)
{}

carte::~carte(void)
{}

carte *carte::getInstance(void)
{
	if(carte::instance == NULL){
			carte::instance = new carte;
		}
		return carte::instance;
}

void carte::load(int numImage)
{
	imageManager *imageMur  = imageManager::getInstance();
	grille		 *Grille	= grille::getInstance();

	this->Sprite.SetImage(*imageMur->GetImage("images/carte_"+to_string(numImage)+".psd"));
	
	this->Sprite.Resize(Grille->getSizeMap().getX(), Grille->getSizeMap().getY()+5);
	this->Sprite.SetCenter(0,0);
	this->Sprite.SetPosition(0, 0);
}


sf::Sprite carte::getSprite(void)
{
	return this->Sprite;
}

int carte::getElement(Coordonnees c)
{
	if(c.getX() < 0 || c.getX() >= 800 || c.getY() < 0 || c.getY() >= 400) return -1;

	sf::Color couleur = this->Sprite.GetPixel(c.getX(),c.getY());
	
	if(couleur.r==0)
	{
		if(couleur.g > couleur.b)
		{
			//Terre (return 1)
			return 1;
		}
		else if (couleur.b < 200)
		{
			//Eau (return 2)
			return 2;
		}
	}
	//Limite terre/eau
	return -1;
}