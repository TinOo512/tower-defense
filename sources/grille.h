#pragma once
class grille
{
private:
	grille(void);
	static grille *instance;
	std::map<Coordonnees, Objet*> _Grille;
	int listeCases[80][40];
	bool is_checkClose;	
	Coordonnees SizeMap;

public:
	static grille *getInstance(void);
	~grille(void);
	Objet *get(Coordonnees _pos);
	Coordonnees getPos(Coordonnees _pos, int i);
	bool add(Coordonnees _pos, Objet *_obj, int overType=0);
	void del(Coordonnees _pos);
	void delAll(void);
	bool isFree(Coordonnees _pos, int i);
	void start_checkClose(void);
	void checkClose(int x=0, int y=0);
	std::vector<Terre*> grilleClose(void);
	Coordonnees getSizeMap(void);
	Coordonnees calcBezier(Coordonnees A, Coordonnees B, Coordonnees C, double t);
	int getListeCases(int x, int y);
};