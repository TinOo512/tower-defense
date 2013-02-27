class Canon : public Objet
{
private:
	bool ColorRed;
public:
	Canon(Coordonnees pos);
	~Canon(void);
	void DelCanonGrille(void);
	bool getColor(void);
	void setColor(bool color);
	void SetPosition(int x, int y);
	void tir(Coordonnees posInitial, Coordonnees posFinal);
	void rotate(Coordonnees posSouris);
};