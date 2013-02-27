class Bateau : public Objet
{
private :
	bool direction[2];
	int lastPcourbe;
	int i_trajectoir;
	int vie;
	sf::Clock ClockTimer;
	std::vector<Coordonnees> grilleOccupe;
	std::vector<Coordonnees> trajectoir;

public:
	Bateau(void);
	~Bateau(void);
	void DelBateauGrille(void);
	void setColor(bool color);
	void resetClockTimer(void);
	int getVie(void);
	void setVie(int _vie);
	void move(void);
	void tir(void);
};