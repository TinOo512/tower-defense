#include "includes.h"

objetManager::objetManager(void)
{
}

objetManager::~objetManager(void)
{
}

objetManager *objetManager::getInstance(){
	if(objetManager::instance == NULL){
		objetManager::instance = new objetManager;
	}
	return objetManager::instance;
}

Objet* objetManager::get(std::string type, int i)
{
	try
    {
		if(type == "bateau")		return this->stockBateau.at(i);
		else if(type == "chateau")	return this->stockChateau.at(i);
		else if(type == "blockmur")	return this->stockBlockMur.at(i);
		else if(type == "missill")	return this->stockMissill.at(i);
		else if(type == "canon")	return this->stockCanon.at(i);
	}
	catch( const std::out_of_range & )
    {
        std::cout << "objetManager get " << type << " a leve une exception std::out_of_range" << std::endl;
    }
	return NULL;
}

int objetManager::getSize(std::string type)
{
	if(type == "bateau")		return this->stockBateau.size();
	else if(type == "chateau")	return this->stockChateau.size();
	else if(type == "blockmur")	return this->stockBlockMur.size();
	else if(type == "missill")	return this->stockMissill.size();
	else if(type == "canon")	return this->stockCanon.size();
}

void objetManager::add(std::string type, Objet* objet)
{
	int i = 0;

	if(type == "bateau")
	{
		i = this->stockBateau.size();
		this->stockBateau.resize(i+1);
		this->stockBateau[i] = (Bateau*)objet;
	}
	else if(type == "chateau")
	{
		i = this->stockChateau.size();
		this->stockChateau.resize(i+1);
		this->stockChateau[i] = (Chateau*)objet;
	}
	else if(type == "blockmur")	
	{
		i = this->stockBlockMur.size();
		this->stockBlockMur.resize(i+1);
		this->stockBlockMur[i] = (BlockMur*)objet;
	}
	else if(type == "missill")	
	{
		i = this->stockMissill.size();
		this->stockMissill.resize(i+1);
		this->stockMissill[i] = (Missill*)objet;
	}
	else if(type == "canon")	
	{
		i = this->stockCanon.size();
		this->stockCanon.resize(i+1);
		this->stockCanon[i] = (Canon*)objet;
	}

	objet->setNumInstance(i);
}

void objetManager::del(std::string type, int _numInstance)
{
	if(type == "bateau")
	{
		if(_numInstance == -1){
			for(int i = 0 ; i < this->stockBateau.size() ; i++){
				this->stockBateau[i]->DelBateauGrille();
			}
			this->stockBateau.clear();
		}else{
			std::vector<Bateau*>	stockBateauTmp;

			int j=0;

			for(int i = 0 ; i < this->stockBateau.size() ; i++){
				if(i != _numInstance){
					stockBateauTmp.resize(j+1);
					stockBateauTmp[j] = this->stockBateau[i];
					this->stockBateau[i]->setNumInstance(j);
					j++;
				}else{
					this->stockBateau[i]->DelBateauGrille();
				}
			}
		
			this->stockBateau = stockBateauTmp;
		}
	}
	else if(type == "chateau")
	{
		this->stockChateau.clear();
	}
	else if(type == "blockmur")	
	{
		if(_numInstance == -1){
			this->stockBlockMur.clear();
		}else{
			std::vector<BlockMur*>	stockBlockMurTmp;

			int j=0;

			for(int i = 0 ; i < this->stockBlockMur.size() ; i++){
				if(i != _numInstance){
					stockBlockMurTmp.resize(j+1);
					stockBlockMurTmp[j] = this->stockBlockMur[i];
					this->stockBlockMur[i]->setNumInstance(j);
					j++;
				}
			}
		
			this->stockBlockMur = stockBlockMurTmp;
		}
	}
	else if(type == "missill")	
	{
		if(_numInstance == -1){
			this->stockMissill.clear();
		}else{
			std::vector<Missill*>	stockMissillTmp;

			int j=0;

			for(int i = 0 ; i < this->stockMissill.size() ; i++){
				if(i != _numInstance){
					stockMissillTmp.resize(j+1);
					stockMissillTmp[j] = this->stockMissill[i];
					this->stockMissill[i]->setNumInstance(j);
					j++;
				}
			}
		
			this->stockMissill = stockMissillTmp;
		}
	}
	else if(type == "canon")	
	{
		if(_numInstance == -1){
			for(int i = 0 ; i < this->stockCanon.size() ; i++){
				this->stockCanon[i]->DelCanonGrille();
			}
			this->stockCanon.clear();
		}else{
			std::vector<Canon*>	stockCanonTmp;

			int j=0;

			for(int i = 0 ; i < this->stockCanon.size() ; i++){
				if(i != _numInstance){
					stockCanonTmp.resize(j+1);
					stockCanonTmp[j] = this->stockCanon[i];
					this->stockCanon[i]->setNumInstance(j);
					j++;
				}
				else{
					this->stockCanon[i]->DelCanonGrille();
				}
			}
		
			this->stockCanon = stockCanonTmp;
		}
	}
}

void objetManager::delAll(void){
	this->stockBateau.clear();
	this->stockBlockMur.clear();
	this->stockMissill.clear();
	this->stockCanon.clear();
}

void objetManager::set(std::string type, Objet* objet, int i)
{
	try
    {
		if(type == "bateau")		this->stockBateau.at(i)		= (Bateau*)objet;
		else if(type == "chateau")	this->stockChateau.at(i)	= (Chateau*)objet;
		else if(type == "blockmur")	this->stockBlockMur.at(i)	= (BlockMur*)objet;
		else if(type == "missill")	this->stockMissill.at(i)	= (Missill*)objet;
		else if(type == "canon")	this->stockCanon.at(i)		= (Canon*)objet;
    }
    catch( const std::out_of_range & )
    {
        std::cout << "objetManager set " << type << " a leve une exception std::out_of_range" << std::endl;
    }	
}