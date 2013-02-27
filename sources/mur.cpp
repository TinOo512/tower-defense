#include "includes.h"

Mur::Mur(int x, int y)
{
	this->Type = 1;
	this->ColorRed = false;
	this->isVisible = true;

	imageManager *imageMur = imageManager::getInstance();

	this->Sprite.SetImage(*imageMur->GetImage("images/mur.psd"));

	this->Sprite.SetCenter(5,5);
	this->Sprite.SetPosition(x,y);
}

Mur::~Mur(void)
{}

bool Mur::getColor(void)
{
	return this->ColorRed;
}

void Mur::setColor(bool color)
{
	if(color)	this->Sprite.SetColor(sf::Color(255, 0, 0, 255));
	else		this->Sprite.SetColor(sf::Color(255, 255, 255, 255));

	this->ColorRed = color;
}

void Mur::SetPosition(int x, int y)
{
	this->Sprite.SetPosition(x,y);
}

void Mur::destruct(void){
	this->isVisible = false;
}

bool Mur::getIsVisible(void)
{
	return this->isVisible;
}

BlockMur::BlockMur(int nbMur)
{
	// Initialise les variable du block de mur
	this->prevX		= 5;
	this->prevY		= 5;
	this->_nbMur	= nbMur;

	int x; int y;

	// Creation du block de mur
	for(int i=0;i<nbMur;i++)
	{
		if(i==0)
		{
			x = this->prevX;
			y = this->prevY;
		}
		else
		{
			if(sf::Randomizer::Random(1, 2) == 1)	x=x+10;
			else									y=y+10;
		}

		// Sauvegarde l'instance du mur
		this->_Mur.resize(i+1);
		this->_Mur[i] = new Mur(x,y);

	}

}

BlockMur::~BlockMur(void)
{}

std::vector<Mur*> BlockMur::getMur(void)
{
	return this->_Mur;
}

void BlockMur::SetPosition(int x, int y)
{
	int tmpX = x; 
	int tmpY = y;
	Coordonnees pos;

	grille *Grille = grille::getInstance();
	carte *Carte	= carte::getInstance();

	// deplace les mur en fonction de la position precedente du mur 1
	for(int i=0;i<this->_nbMur;i++)
	{
		if(i > 0)
		{
			tmpX = this->_Mur[i]->getSprite().GetPosition().x - this->prevX + x;
			tmpY = this->_Mur[i]->getSprite().GetPosition().y - this->prevY + y;
		}

		pos.Set(tmpX,tmpY);
		
		this->_Mur[i]->SetPosition(tmpX,tmpY);
				
		if( ( Carte->getElement(pos) != 1 || !Grille->isFree(pos, 0) ) && !this->_Mur[i]->getColor() )
		{
			// position mur interdit, on color en rouge
			this->_Mur[i]->setColor(true);
		}
		else if(Carte->getElement(pos) == 1 && Grille->isFree(pos, 0) && this->_Mur[i]->getColor())
		{
			// position mur ok
			this->_Mur[i]->setColor(false);
		}

	}

	// Sauvegarde la nouvelle position du mur 1
	this->prevX = x;
	this->prevY = y;
}

void BlockMur::Rotation(int direction)
{
	int i = 1;
	int x = 0; 
	int y = 0;
	Coordonnees tmpTAB[6];

	// sauvegarde la position actuelle des mur
	for(i=0;i<this->_nbMur;i++)
	{
		tmpTAB[i] = Coordonnees(this->_Mur[i]->getSprite().GetPosition().x,this->_Mur[i]->getSprite().GetPosition().y);
	}

	for(i=1;i<this->_nbMur;i++)
	{
		x = y = 0;

		if(tmpTAB[i].getX() != tmpTAB[i-1].getX())
		{
			// mur +1 sur la meme ligne horizontale;
			if( (tmpTAB[i].getX() - tmpTAB[i-1].getX()) == 10)
			{
				// mur +1 a droite
				if(direction > 0)
				{
					// mur +1 passe en haut
					y=-10;
				}
				else
				{
					// mur +1 passe en bas
					y=10;
				}
			}
			else if( (tmpTAB[i].getX() - tmpTAB[i-1].getX()) == -10)
			{
				// mur +1 a gauche
				if(direction > 0)
				{
					// mur +1 passe en haut
					y=10;
				}
				else
				{						
					// mur +1 passe en bas
					y=-10;
				}
			}
		}
		else
		{
			// mur +1 sur la meme ligne verticale;
			if( (tmpTAB[i].getY() - tmpTAB[i-1].getY()) == 10)
			{
				// mur +1 en bas
				if(direction > 0)
				{
					// mur +1 passe a droite
					x=10;
				}
				else
				{
					// mur +1 passe a gauche
					x=-10;					
				}
			}
			else if( (tmpTAB[i].getY() - tmpTAB[i-1].getY()) == -10)
			{
				// mur +1 en haut
				if(direction > 0)
				{
					// mur +1 passe a droite
					x=-10;										
				}
				else
				{
					// mur +1 passe a gauche
					x=10;
				}
			}
		}

		this->_Mur[i]->SetPosition(this->_Mur[i-1]->getSprite().GetPosition().x + x,this->_Mur[i-1]->getSprite().GetPosition().y + y);
	}

}

bool BlockMur::ToGrille(void)
{
	grille *Grille	= grille::getInstance();
	carte *Carte	= carte::getInstance();

	Coordonnees pos;
	int nbCheckOK = 0;
	int i=0;

	// on check si tous les mur sont dans une zone libre et sur la terre
	for(i=0;i<this->_nbMur;i++)
	{
		pos.Set(this->_Mur[i]->getSprite().GetPosition().x,this->_Mur[i]->getSprite().GetPosition().y);

		if(Carte->getElement(pos) == 1 && Grille->isFree(pos, 0))
		{
			nbCheckOK++;
		}
	}

	if(nbCheckOK == this->_nbMur)
	{
		nbCheckOK = 0;

		for(i=0;i<this->_nbMur;i++)
		{
			pos.Set(this->_Mur[i]->getSprite().GetPosition().x,this->_Mur[i]->getSprite().GetPosition().y);

			if(Grille->add(pos,this->_Mur[i]))
			{
				nbCheckOK++;
			}
		}

		if(nbCheckOK == this->_nbMur) return true;
	}

	return false;
}

Terre::Terre(int x, int y)
{
	this->Type = 5;

	imageManager *imageMur = imageManager::getInstance();

	this->Sprite.SetImage(*imageMur->GetImage("images/terre.psd"));

	this->Sprite.SetCenter(5,5);
	this->Sprite.SetPosition(x,y);
}

Terre::~Terre(void)
{}