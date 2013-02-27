#include "includes.h"

Bateau::Bateau(void)
{
	carte		 *Carte		  = carte::getInstance();
	grille		 *Grille	  = grille::getInstance();
	imageManager *imageBateau = imageManager::getInstance();
	objetManager *objetManager = objetManager::getInstance();

	this->vie = 3;
	this->isMove = false;
	this->Type = 2;
	this->Width = 40;
	this->Height = 40;
	
	this->Sprite.SetImage(*imageBateau->GetImage("images/bateau.psd"));
	Sprite.SetCenter(20,20);

	// Angle Haut, Gauche
	int tmpDirection = 1;
	Coordonnees pos(20,20);
	this->direction[0] = 0; //direction[0] = 0 -> bas ; 1 -> haut
	this->direction[1] = 1; //direction[1] = 0 -> gauche ; 1 -> droite
	Sprite.SetSubRect(sf::IntRect(this->Width, 0, (this->Width*2), this->Height));

	while(Carte->getElement(pos) != 2){
		if(pos.getX() == Grille->getSizeMap().getX()-20){
			//Angle Bas, Gauche
			tmpDirection = 2;
			pos.Set(20, Grille->getSizeMap().getY()-20);
			this->direction[0] = 1;
			this->direction[1] = 1;
			Sprite.SetSubRect(sf::IntRect(this->Width, 0, (this->Width*2), this->Height));
		}
		else if(pos.getY() == Grille->getSizeMap().getY()-20){
			//Angle Bas, Droite
			tmpDirection = 3;
			pos.Set(Grille->getSizeMap().getX()-20, Grille->getSizeMap().getY()-20);
			this->direction[0] = 1;
			this->direction[1] = 0;
			Sprite.SetSubRect(sf::IntRect(0, 0, this->Width, this->Height));
		}
		else{
			//Angle Haut, Droite
			tmpDirection = 4;
			pos.Set(Grille->getSizeMap().getX()-20, 20);
			this->direction[0] = 0;
			this->direction[1] = 0;
			Sprite.SetSubRect(sf::IntRect(0, 0, this->Width, this->Height));
		}
	}

	int x = 0;
	int y = 0;
	Coordonnees posInit;

	switch(tmpDirection)
	{
	case 1:
		x = y = 40;
		break;
	case 2:
		x = 40;
		y = -40;
		break;
	case 3:
		x = y -40;
		break;
	case 4:
		x = -40;
		y = 40;
		break;
	}

	// On récupère la position du dernier chateau placé
	int lastPos = objetManager->getSize("bateau");

	if(lastPos > 0)
	{
		sf::Sprite lastSprite = ((Chateau*)objetManager->get("bateau",lastPos-1))->getSprite();

		posInit.Set(lastSprite.GetPosition().x + x, lastSprite.GetPosition().y);
	}
	else
	{
		switch(tmpDirection)
		{
		case 1:
			posInit.Set(20, 20);
			break;
		case 2:
			posInit.Set(20, 380);
			break;
		case 3:
			posInit.Set(780, 380);
			break;
		case 4:
			posInit.Set(780, 20);
			break;
		}
	}

	do
	{
		if(posInit.getX() > 780 || posInit.getX() < 20)
		{
			// on passe à la ligne
			switch(tmpDirection)
			{
			case 1:
				posInit.Set(20, posInit.getY()+20);
				break;
			case 2:
				posInit.Set(20, posInit.getY()-20);
				break;
			case 3:
				posInit.Set(780, posInit.getY()-20);
				break;
			case 4:
				posInit.Set(780, posInit.getY()+20);
				break;
			}			
		}

		if(posInit.getY() > 380 || posInit.getY() < 20)
		{
			// on revient au point de depart en haut à gauche
			//posInit.Set(100, 50);
			switch(tmpDirection)
			{
			case 1:
				posInit.Set(20, 20);
				break;
			case 2:
				posInit.Set(20, 380);
				break;
			case 3:
				posInit.Set(780, 380);
				break;
			case 4:
				posInit.Set(780, 20);
				break;
			}
		}

		if(!Grille->isFree(posInit, 2) || Carte->getElement(posInit) != 2)
		{
			//posInit = posInit + Coordonnees(200,0);

			switch(tmpDirection)
			{
			case 1:
				posInit = posInit + Coordonnees(20,0);
				break;
			case 2:
				posInit = posInit + Coordonnees(20,0);
				break;
			case 3:
				posInit = posInit + Coordonnees(-20,0);
				break;
			case 4:
				posInit = posInit + Coordonnees(-20,0);
				break;
			}
		}

	} while(Carte->getElement(posInit) != 2 || !Grille->isFree(posInit, 2));

	Sprite.SetPosition(posInit.getX(),posInit.getY());
}

Bateau::~Bateau(void)
{
}

void Bateau::DelBateauGrille(void){
	grille *Grille = grille::getInstance();

	for(int i=0; i<this->grilleOccupe.size(); i++) Grille->del(this->grilleOccupe[i]);
}

void Bateau::setColor(bool color)
{
	if(color)	this->Sprite.SetColor(sf::Color(255, 0, 0, 255));
	else		this->Sprite.SetColor(sf::Color(255, 255, 255, 255));
}

void Bateau::resetClockTimer(void)
{
	this->ClockTimer.Reset();
}

int Bateau::getVie(void){
	return this->vie;
}

void Bateau::setVie(int _vie){
	this->vie = _vie;
}

void Bateau::move(void){
	carte		 *Carte		  = carte::getInstance();
	grille		 *Grille	  = grille::getInstance();

	//Changement de couleur pendant 0.5sec.
	if (this->ClockTimer.GetElapsedTime() > 0.5) this->setColor(false);

	Coordonnees combi[4];

	if(!this->isMove)
	{
		this->i_trajectoir = 0;
		
		combi[0] = this->getSpritePos(2);

		int i = 0;

		if(this->direction[0] == 0) //direction[0] = 0 -> bas ; 1 -> haut
		{
			if(this->direction[1] == 0) //direction[1] = 0 -> gauche ; 1 -> droite
			{
				// BAS GAUCHE
				this->Sprite.SetSubRect(sf::IntRect(0, 0, this->Width, this->Height));
				
				if(Grille->isFree(combi[0]+Coordonnees(-40,0),2) && Carte->getElement(combi[0]+Coordonnees(-40,0)) == 2)	combi[1] = combi[0]+Coordonnees(-40,0);
				if(Grille->isFree(combi[0]+Coordonnees(0,40),2) && Carte->getElement(combi[0]+Coordonnees(0,40)) == 2)		combi[2] = combi[0]+Coordonnees(0,40);
				if(Grille->isFree(combi[0]+Coordonnees(-40,40),2) && Carte->getElement(combi[0]+Coordonnees(-40,40)) == 2)	combi[3] = combi[0]+Coordonnees(-40,40);			
				
			}
			else
			{
				// BAS DROITE
				this->Sprite.SetSubRect(sf::IntRect(this->Width, 0, (this->Width*2), this->Height));
				
				if(Grille->isFree(combi[0]+Coordonnees(40,0),2) && Carte->getElement(combi[0]+Coordonnees(40,0)) == 2)		combi[1] = combi[0]+Coordonnees(40,0);
				if(Grille->isFree(combi[0]+Coordonnees(0,40),2) && Carte->getElement(combi[0]+Coordonnees(0,40)) == 2)		combi[2] = combi[0]+Coordonnees(0,40);
				if(Grille->isFree(combi[0]+Coordonnees(40,40),2) && Carte->getElement(combi[0]+Coordonnees(40,40)) == 2)	combi[3] = combi[0]+Coordonnees(40,40);			
			}
		}
		else
		{
			if(this->direction[1] == 0)
			{
				// HAUT GAUCHE
				this->Sprite.SetSubRect(sf::IntRect(0, 0, this->Width, this->Height));
				
				if(Grille->isFree(combi[0]+Coordonnees(-40,0),2) && Carte->getElement(combi[0]+Coordonnees(-40,0)) == 2)	 combi[1] = combi[0]+Coordonnees(-40,0);
				if(Grille->isFree(combi[0]+Coordonnees(0,-40),2) && Carte->getElement(combi[0]+Coordonnees(0,-40)) == 2)	 combi[2] = combi[0]+Coordonnees(0,-40);
				if(Grille->isFree(combi[0]+Coordonnees(-40,-40),2) && Carte->getElement(combi[0]+Coordonnees(-40,-40)) == 2) combi[3] = combi[0]+Coordonnees(-40,-40);			
				
			}
			else
			{
				// HAUT DROITE
				this->Sprite.SetSubRect(sf::IntRect(this->Width, 0, (this->Width*2), this->Height));
				
				if(Grille->isFree(combi[0]+Coordonnees(40,0),2) && Carte->getElement(combi[0]+Coordonnees(40,0)) == 2)		combi[1] = combi[0]+Coordonnees(40,0);
				if(Grille->isFree(combi[0]+Coordonnees(0,-40),2) && Carte->getElement(combi[0]+Coordonnees(0,-40)) == 2)	combi[2] = combi[0]+Coordonnees(0,-40);
				if(Grille->isFree(combi[0]+Coordonnees(40,-40),2) && Carte->getElement(combi[0]+Coordonnees(40,-40)) == 2)	combi[3] = combi[0]+Coordonnees(40,-40);			
				
			}
		}

		if(combi[3].getX() > 0 && sf::Randomizer::Random(0, 1000) > 500)
		{
			if(combi[3].getX() > 780 || combi[3].getX() < 20 || combi[3].getY() > 380 || combi[3].getY() < 20)
			{
				this->isMove = false;
			}
			else
			{
				this->trajectoir.resize(40);

				for(int t=1; t<=40; t++) 
				{
					int x = 0;
					int y = 0;
					
					if(combi[3].getX() > combi[0].getX())	x = combi[0].getX() + t; 
					else									x = combi[0].getX() - t;
				
					if(combi[3].getY() > combi[0].getY())	y = combi[0].getY() + t; 
					else									y = combi[0].getY() - t;

					this->trajectoir[t-1] = Coordonnees(x,y);
				}

				// Ajoute a la grilleOccupe
				this->grilleOccupe.resize(3);
				this->grilleOccupe[0] = combi[0];
				this->grilleOccupe[1] = combi[1];
				this->grilleOccupe[2] = combi[3];

				this->isMove = true;
			}

		}
		else if(combi[1].getX() > 0 || combi[2].getX() > 0)
		{
			this->trajectoir.resize(40);

			if(combi[1].getX() > 0)
			{
				if(combi[1].getX() > 780 || combi[1].getX() < 20 || combi[1].getY() > 380 || combi[1].getY() < 20)
				{
					this->isMove = false;
				}
				else
				{				
					if(combi[1] < combi[0]) for(i=0;i<40;i++) this->trajectoir[i] = combi[0] - Coordonnees(i+1,0);
					else					for(i=0;i<40;i++) this->trajectoir[i] = combi[0] + Coordonnees(i+1,0);

					// Ajoute a la grilleOccupe
					this->grilleOccupe.resize(2);
					this->grilleOccupe[0] = combi[0];
					this->grilleOccupe[1] = combi[1];

					this->isMove = true;
				}
			}
			else
			{
				if(combi[2].getX() > 780 || combi[2].getX() < 20 || combi[2].getY() > 380 || combi[2].getY() < 20)
				{
					this->isMove = false;
				}
				else
				{
					if(combi[2] < combi[0]) for(i=0;i<40;i++) this->trajectoir[i] = combi[0] - Coordonnees(0,i+1);
					else					for(i=0;i<40;i++) this->trajectoir[i] = combi[0] + Coordonnees(0,i+1);

					// Ajoute a la grilleOccupe
					this->grilleOccupe.resize(2);
					this->grilleOccupe[0] = combi[0];
					this->grilleOccupe[1] = combi[2];

					this->isMove = true;
				}
			}

			
			if(sf::Randomizer::Random(0, 1000) > 500)
			{
				sf::Randomizer::SetSeed(sf::Randomizer::Random(0, 1000)+10);
				if(sf::Randomizer::Random(0, 1000) > 250) this->direction[0] = !this->direction[0];

				sf::Randomizer::SetSeed(sf::Randomizer::Random(0, 1000)+20);
				if(sf::Randomizer::Random(0, 1000) > 500) this->direction[1] = !this->direction[1];
			}
			

		}

		// Ajoute à la vrai grille
		if(this->isMove)
		{
			for(i=0; i<this->grilleOccupe.size(); i++) Grille->add(this->grilleOccupe[i], this);
		}
	}
	
	if(this->isMove)
	{		
		this->Sprite.SetPosition(this->trajectoir[this->i_trajectoir].getX(), this->trajectoir[this->i_trajectoir].getY());
		this->i_trajectoir++;

		if(this->i_trajectoir >= this->trajectoir.size()) 
		{
			// Supprime de la grille
			for(int i=0; i<this->grilleOccupe.size(); i++) Grille->del(this->grilleOccupe[i]);

			this->isMove = false;
		}
	}
	else
	{

		if(sf::Randomizer::Random(0, 1000) > 250) this->direction[0] = !this->direction[0];

		sf::Randomizer::SetSeed(sf::Randomizer::Random(0, 1000)+30);

		if(sf::Randomizer::Random(0, 1000) > 500) this->direction[1] = !this->direction[1];
	}
}

void Bateau::tir(void){
	objetManager *objetManager = objetManager::getInstance();

	int rdm = sf::Randomizer::Random(0, 4);

	objetManager->add("missill", new Missill(Coordonnees(this->Sprite.GetPosition().x, this->Sprite.GetPosition().y), ((Chateau*)objetManager->get("chateau",rdm))->getSpritePos(), this->Type));
}