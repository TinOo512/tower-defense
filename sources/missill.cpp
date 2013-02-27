#include "includes.h"

Missill::Missill(Coordonnees posInitial, Coordonnees posFinal, int _Type){
	this->Type = _Type;

	imageManager *imageTir	= imageManager::getInstance();
	grille		 *Grille	= grille::getInstance();
	
	this->Sprite.SetImage(*imageTir->GetImage("images/missill.psd"));

	this->PosInitial = posInitial;
	this->PosFinal = posFinal;

	Coordonnees posIntermediaire;
	if(this->PosInitial.getY() < this->PosFinal.getY()){
		posIntermediaire.Set(this->PosFinal.getX(), this->PosInitial.getY());
	}else{
		posIntermediaire.Set(this->PosInitial.getX(), this->PosFinal.getY());
	}

	Sprite.SetCenter(5,5);
	Sprite.SetPosition(this->PosInitial.getX(), this->PosInitial.getY());

	this->i_trajectoir = 0;

	int i = 0;
	for(double t = 0.0;t <= 1.0; t += 0.02) {
		this->trajectoir.resize(i+1);
		this->trajectoir[i++] = Grille->calcBezier(this->PosInitial, posIntermediaire, this->PosFinal, t);
	}
}

Missill::~Missill(void)
{}

int Missill::move(void){
	grille		 *Grille		= grille::getInstance();
	objetManager *objetManager	= objetManager::getInstance();

	this->Sprite.SetPosition(this->trajectoir[this->i_trajectoir].getX(), this->trajectoir[this->i_trajectoir].getY());
	this->i_trajectoir++;

	if(this->i_trajectoir >= this->trajectoir.size()){
		//suppression de l'instance missill
		return -1;
	}

	//Coordonnee du Missill
	Coordonnees missill(this->Sprite.GetPosition().x, this->Sprite.GetPosition().y);

	if(this->Type == 2 && !Grille->isFree(missill, 1)){
		Coordonnees missillGrilleMur = Grille->getPos(missill, 1);
		//Passage de isVisible à false (masquage du mur)
		((Mur*)Grille->get(missillGrilleMur))->destruct();
		//suppression du mur de la grille
		Grille->del(missillGrilleMur);

		//suppression de l'instance missill + incrementation du score de 100
		return 1;
	}
	else if(this->Type == 4 && !Grille->isFree(missill, 2)){
		Coordonnees missillGrilleBateau = Grille->getPos(missill, 2);

		int _vie = ((Bateau*)Grille->get(missillGrilleBateau))->getVie();
		((Bateau*)Grille->get(missillGrilleBateau))->setVie(_vie-1);

		if(_vie-1 == 0){
			//suppression de l'instance bateau
			objetManager->del("bateau", ((Bateau*)Grille->get(missillGrilleBateau))->getNumInstance());
		}else{
			((Bateau*)Grille->get(missillGrilleBateau))->setColor(true);
			((Bateau*)Grille->get(missillGrilleBateau))->resetClockTimer();
		}

		//suppression de l'instance missill + incrementation du score de 200
		return 2;
	}

	return 0;
}