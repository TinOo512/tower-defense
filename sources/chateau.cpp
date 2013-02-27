#include "includes.h"

Chateau::Chateau(void)
{
	carte		 *Carte		  = carte::getInstance();
	grille		 *Grille	  = grille::getInstance();
	imageManager *imageChateau = imageManager::getInstance();
	objetManager *objetManager = objetManager::getInstance();

	this->Type = 3;
	this->Width = 60;
	this->Height = 60;
		
	this->Sprite.SetImage(*imageChateau->GetImage("images/chateau.psd"));
	this->Sprite.Resize(this->Width, this->Height);
	this->Sprite.SetCenter(this->Width/2, this->Height/2);

	Coordonnees posInit(100,50);

	// On récupère la position du dernier chateau placé
	int lastPos = objetManager->getSize("chateau");

	if(lastPos > 0)
	{
		sf::Sprite lastSprite = ((Chateau*)objetManager->get("chateau",lastPos-1))->getSprite();

		if(lastPos%2)	posInit.Set(lastSprite.GetPosition().x + 200, lastSprite.GetPosition().y);
		else			posInit.Set(lastSprite.GetPosition().x + 400, lastSprite.GetPosition().y);
	}

	do
	{
		if(posInit.getX() > 700)
		{
			// on passe à la ligne suivante en revenant a gauche
			posInit.Set(100, posInit.getY()+100);
		}

		if(posInit.getY() > 350)
		{
			// on revient au point de depart en haut à gauche
			posInit.Set(100, 50);
		}


		if(!Grille->isFree(posInit, 4) || Carte->getElement(posInit) != 1)
		{
			posInit = posInit + Coordonnees(200,0);
		}

	} while(Carte->getElement(posInit) != 1 || !Grille->isFree(posInit, 4));

	// Sauvegarde du chateau dans sa zone;
	Grille->add(posInit,this, 4);

	posInit = posInit + Coordonnees(0,30);
	
	posInit = Grille->getPos(posInit,3);
	
	Grille->add(posInit,this);
	
	Sprite.SetPosition(posInit.getX(),posInit.getY());
}

Chateau::~Chateau(void)
{}