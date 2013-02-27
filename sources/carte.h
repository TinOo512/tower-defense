#pragma once
class carte
{
private:
	carte(void);
	static carte *instance;
	sf::Sprite Sprite;
	sf::Image Image;

public:
	~carte(void);
	static carte *getInstance(void);
	void load(int numImage);
	int getElement(Coordonnees c);
	sf::Sprite getSprite(void);
};