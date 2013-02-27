#include "includes.h"

Canon::Canon(Coordonnees pos){
	this->Type = 4;
	this->ColorRed = false;

	imageManager *imageMur	= imageManager::getInstance();
	grille *Grille			= grille::getInstance();

	Grille->add(pos, this, 1);

	this->Sprite.SetImage(*imageMur->GetImage("images/canon.psd"));

	this->Sprite.SetCenter(5,5);
	this->Sprite.SetPosition(pos.getX(),pos.getY());
}

Canon::~Canon(void)
{
}

void Canon::DelCanonGrille(void){
	grille *Grille = grille::getInstance();

	Grille->del(Coordonnees(this->Sprite.GetPosition().x, this->Sprite.GetPosition().y));
}

bool Canon::getColor(void)
{
	return this->ColorRed;
}

void Canon::setColor(bool color)
{
	if(color)	this->Sprite.SetColor(sf::Color(255, 0, 0, 255));
	else		this->Sprite.SetColor(sf::Color(255, 255, 255, 255));

	this->ColorRed = color;
}

void Canon::SetPosition(int x, int y)
{
	grille *Grille = grille::getInstance();

	this->Sprite.SetPosition(x,y);

	if( ( Grille->getListeCases((x-5)/10, (y-5)/10) != 0 || !Grille->isFree(Coordonnees(x, y), 1) || !Grille->isFree(Coordonnees(x, y), 3) ) && !this->getColor() )
	{
		// position canon interdit, on color en rouge
		this->setColor(true);
	}
	else if(Grille->getListeCases((x-5)/10, (y-5)/10) == 0 && Grille->isFree(Coordonnees(x, y), 1) && Grille->isFree(Coordonnees(x, y), 3) && this->getColor())
	{
		// position canon ok
		this->setColor(false);
	}
}

void Canon::tir(Coordonnees posInitial, Coordonnees posFinal){
	objetManager *objetManager = objetManager::getInstance();

	objetManager->add("missill", new Missill(posInitial, posFinal, this->Type));
}

void Canon::rotate(Coordonnees posSouris){
	const float Pi = 3.141592654f;

	float ab = std::abs(this->Sprite.GetPosition().x - posSouris.getX());
	float cb = std::abs(this->Sprite.GetPosition().y - posSouris.getY());
	float ac = std::sqrt(ab*ab + cb*cb);

	float angle = std::acos( ( ab*ab + ac*ac - cb*cb ) / ( 2*ab*ac ) );
	angle = angle / Pi * 180;

	if(posSouris.getX() < this->Sprite.GetPosition().x){
		if(posSouris.getY() < this->Sprite.GetPosition().y){
			//Haut Gauche
			this->Sprite.SetRotation(180-angle);
		}else{
			//Bas Gauche
			this->Sprite.SetRotation(180+angle);
		}
	}else{
		if(posSouris.getY() < this->Sprite.GetPosition().y){
			//Haut Droite
			this->Sprite.SetRotation(angle);
		}else{
			//Bas Droite
			this->Sprite.SetRotation(-angle);
		}
	}
}