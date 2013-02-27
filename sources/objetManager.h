#pragma once
class objetManager
{
private:
	objetManager(void);

	std::vector<Bateau*>	stockBateau;
	std::vector<Chateau*>	stockChateau;
	std::vector<BlockMur*>	stockBlockMur;
	std::vector<Missill*>	stockMissill;
	std::vector<Canon*>		stockCanon;

	static objetManager *instance;

public:
	~objetManager(void);
	static objetManager *getInstance(void);
	Objet* get(std::string type, int i);
	int getSize(std::string type);
	void add(std::string type, Objet* objet);
	void del(std::string type, int _numInstance = -1);
	void delAll(void);
	void set(std::string type, Objet* objet, int i);
};