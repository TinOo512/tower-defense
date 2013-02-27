#include "includes.h"

grille::grille(void)
{
	SizeMap.Set(800,400);
}

grille::~grille(void)
{}

grille *grille::getInstance()
{
	if(grille::instance == NULL){
		grille::instance = new grille;
	}
	return grille::instance;
}

Coordonnees grille::getSizeMap(void)
{
	return SizeMap;
}

Objet *grille::get(Coordonnees _pos)
{
	if(!(this->_Grille.find(_pos) != this->_Grille.end())) std::cout << "grille get x:" << _pos.getX() << " y:" << _pos.getY() << " a leve une exception out_of_range" << std::endl;

	return this->_Grille[_pos];
}

Coordonnees grille::getPos(Coordonnees _pos, int i)
{
	int refX;
	int refY;

	switch(i)
	{
	case 1:
		refX = 5;
		refY = 5;
		break;
	case 2:
		refX = 20;
		refY = 20;
		break;
	case 3:
		refX = 30;
		refY = 30;
		break;
	case 4:
		refX = 100;
		refY = 50;
	}

	int tmpX = refX;
	int tmpY = refY;
	
	while( _pos.getX() > tmpX + refX ) tmpX = tmpX + (refX*2);
	while( _pos.getY() > tmpY + refY ) tmpY = tmpY + (refY*2);

	return Coordonnees(tmpX,tmpY);
}

bool  grille::add(Coordonnees _pos, Objet *_obj, int overType)
{
	if(!overType) _pos = this->getPos(_pos,_obj->getType());
	else		  _pos = this->getPos(_pos,overType);
	
	if(this->isFree(_pos,0))
	{
		this->_Grille[_pos] = _obj;
		if(_obj->getType() == 1) this->listeCases[(_pos.getX()-5)/10][(_pos.getY()-5)/10] = 1;
		return true;
	}
	
	return false;
}

void grille::del(Coordonnees _pos)
{
	std::map<Coordonnees, Objet*>::iterator it = this->_Grille.find(_pos);
	
	if(it != this->_Grille.end()) this->_Grille.erase(it);

	if(this->listeCases[(_pos.getX()-5)/10][(_pos.getY()-5)/10] == 1) this->listeCases[(_pos.getX()-5)/10][(_pos.getY()-5)/10] = 0;
}

void grille::delAll(void)
{
	objetManager *objetManager = objetManager::getInstance();

	//Effacement complet de la grille
	this->_Grille.clear();

	for (int i=0; i<80; i++){
		for (int j=0; j<40; j++){
			this->listeCases[i][j] = 0;

		}
	}
	//On remet les chateaux sur la grille
	for(int i=0 ; i<5 ; i++){
		//Grande zone
		this->add(((Chateau*)objetManager->get("chateau",i))->getSpritePos(), ((Chateau*)objetManager->get("chateau",i)), 4);
		Coordonnees posChateau = ((Chateau*)objetManager->get("chateau",i))->getSpritePos() + Coordonnees(0,30);
		posChateau = this->getPos(posChateau,3);
		this->add(posChateau, ((Chateau*)objetManager->get("chateau",i)));
	}
}

bool grille::isFree(Coordonnees _pos,int i=0)
{
	/**
	/* Grille 0 : toutes les grilles
	/* Grille 1 : Mur
	/* Grille 2 : Bateau
	/* Grille 3 : Chateau
	*/
	Coordonnees tpm;

	if(i==0)
	{
		for(int j=1;j<=3;j++)
		{
			tpm = this->getPos(_pos,j);

			if((this->_Grille.find(tpm) != this->_Grille.end())) return false;
		}
	}
	else
	{
		tpm = this->getPos(_pos,i);

		if((this->_Grille.find(tpm) != this->_Grille.end())) return false;
	}

	return true;
}

void grille::start_checkClose(void)
{
	this->is_checkClose = false;

	for(int y=0;y<(this->getSizeMap().getY()/10);y++)
	{
		for(int x=0;x<(this->getSizeMap().getX()/10);x++)
		{
			if(this->listeCases[x][y] != 1) this->listeCases[x][y] = 0;
		}
	}

	this->checkClose(0,0);
}

void grille::checkClose(int x, int y)
{
	if(x >= 0 && x < (this->getSizeMap().getX()/10) && y >= 0 && y < (this->getSizeMap().getY()/10) && this->listeCases[x][y] == 0)
	{
        if(!this->is_checkClose) this->is_checkClose = true;
		this->listeCases[x][y] = 2;

        checkClose(x+1, y);
        checkClose(x-1, y);
        checkClose(x, y+1);
        checkClose(x, y-1);
        checkClose(x+1, y+1);
        checkClose(x+1, y-1);
        checkClose(x-1, y+1);
        checkClose(x-1, y-1);
    }

	if(x < (this->getSizeMap().getX()/10) && !this->is_checkClose) checkClose(x+1,y);
}

std::vector<Terre*> grille::grilleClose(void)
{
	objetManager *objetManager = objetManager::getInstance();

	std::vector<Terre*> allTerre;
	int i = 0;

	for(int y=0;y<(this->getSizeMap().getY()/10);y++)
	{
		for(int x=0;x<(this->getSizeMap().getX()/10);x++)
		{
			if(this->listeCases[x][y] == 0) 
			{
				allTerre.resize(i+1);
				allTerre[i++] = new Terre(x*10+5,y*10+5);
			}
		}
	}

	return allTerre;
}

Coordonnees grille::calcBezier(Coordonnees A, Coordonnees B, Coordonnees C, double t)
{
    return Coordonnees(pow((1 - t), 2) * A.getX() + 2 * t * (1 -t) * B.getX() + pow(t, 2) * C.getX() , pow((1 - t), 2) * A.getY() + 2 * t * (1 -t) * B.getY() + pow(t, 2) * C.getY());
}

int grille::getListeCases(int x, int y){
	return this->listeCases[x][y];
}