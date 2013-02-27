class Missill : public Objet
{
private:
	int i_trajectoir;
	std::vector<Coordonnees> trajectoir;
public:
	Missill(Coordonnees posInitial, Coordonnees posFinal, int _Type);
	~Missill(void);
	int move(void);
};