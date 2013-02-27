#pragma once
class imageManager
{
private:
	imageManager(void);
	static imageManager *instance;
	std::map<std::string, sf::Image*> Images;
public:
	~imageManager(void);
	static imageManager *getInstance(void);
	sf::Image *CreateImage(const std::string &ImageLink);
	bool SearchImage(const std::string &ImageLink, sf::Image *&ImgTemp);
	sf::Image *GetImage(const std::string &ImageLink);
	void DeleteImage(sf::Image &Img);
	void DeleteImage(const std::string &ImageLink);
};