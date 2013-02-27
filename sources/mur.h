class Mur : public Objet
{
private:
	bool ColorRed;
	bool isVisible;
public:
	Mur(int x, int y);
	~Mur(void);
	bool getColor(void);
	void setColor(bool color);
	void SetPosition(int x, int y);
	void destruct(void);
	bool getIsVisible(void);
};

class BlockMur : public Objet
{
private:	
	std::vector<Mur*> _Mur;
	int _nbMur;
	int prevX;
	int prevY;

public:
	BlockMur(int nbMur);
	~BlockMur(void);
	std::vector<Mur*>  getMur(void);
	void SetPosition(int x, int y);
	void Rotation(int direction);
	bool ToGrille(void);
};

class Terre : public Objet
{
public:
	Terre(int x, int y);
	~Terre(void);
};