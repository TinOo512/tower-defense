#include "includes.h"

imageManager::imageManager(void)
{
}


imageManager::~imageManager(void)
{
	//On détruit toutes les images restantes
	std::map<std::string, sf::Image*>::iterator it;
	for(it = Images.begin(); it != Images.end(); it++)
	{
		delete it->second;
	}
}

imageManager *imageManager::getInstance(){
	if(imageManager::instance == NULL){
		imageManager::instance = new imageManager;
	}
	return imageManager::instance;
}

bool imageManager::SearchImage(const std::string &ImageLink, sf::Image* &ImgTemp)
{
  std::map<std::string, sf::Image*>::iterator it;
 
  it = Images.find(ImageLink);
 
  if(it == Images.end())
  {
    return false;
  }
  else
  {
    ImgTemp = it->second;
  }
 
  return true;
}

sf::Image *imageManager::CreateImage(const std::string &ImageLink)
{
  sf::Image *ImgTemp = new sf::Image();
  if(ImgTemp->LoadFromFile(ImageLink))
  {
    Images.insert(std::make_pair(ImageLink, ImgTemp));
    return ImgTemp;
  }
  else
  {
    delete ImgTemp;
  }
 
  return NULL;
}

sf::Image *imageManager::GetImage(const std::string &ImageLink)
{
  sf::Image *ImgTemp;
 
  //Si l'image demandée n'existe pas déjà, on la créée
  if(!SearchImage(ImageLink, ImgTemp))
  {
    ImgTemp = CreateImage(ImageLink);
  }
 
  return ImgTemp;
}

void imageManager::DeleteImage(sf::Image &Img)
{
  std::map<std::string, sf::Image*>::iterator it;
 
  //On cherche l'image demandée
  for(it = Images.begin(); it != Images.end(); it++)
  {
    //Si on la trouve, on la détruit et on la retire
    if(it->second == &Img)
    {
      delete &Img;
      Images.erase(it);
      break;
    }
  }
}

void imageManager::DeleteImage(const std::string& ImageLink)
{
  std::map<std::string, sf::Image*>::iterator it;
 
  it = Images.find(ImageLink);
 
  if(it != Images.end())
  {
    if(it->second != NULL)
    {
      delete (it->second);
      Images.erase(it);
    }
  }
}
