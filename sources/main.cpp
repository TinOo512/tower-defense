#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#include "includes.h"

// Cretion de la fenetre principal
sf::RenderWindow App(sf::VideoMode(800, 450, 32), "SFML Graphics", sf::Style::Close);

// Initialisation des class singleton
carte			*carte::instance		 = NULL;
grille			*grille::instance		 = NULL;
imageManager	*imageManager::instance  = NULL;
objetManager	*objetManager::instance  = NULL;

int main(int argc, char *p_argv[])
{	
	// limite le FPS a 60 et force la synchronisation vertical
	App.UseVerticalSync(true);
	App.SetFramerateLimit(60);
	App.ShowMouseCursor(false);

	// Stoke les entrée clavier et souris
	const sf::Input& Input = App.GetInput();

	// Objet contenant la position de la souris sur la grille 
	Coordonnees _Mouse;

	// Carte
	carte *Carte = carte::getInstance();
	// Grille
	grille *Grille = grille::getInstance();
	// Image Manager
	imageManager *imageManager = imageManager::getInstance();
	// Objet Manager
	objetManager *objetManager = objetManager::getInstance();

	// Conteneur Terre (zone fermer)
	std::vector<Terre*> allTerre;

	// Numero du block mur en cour
	int iBlockMur = 0;
	// Numero du canon en cour
	int iCanon = 0;

	int murTouche = 0;

	//Differente phase de jeux
	std::string phaseJeux = "placementMur";

	// Chargement de la carte
	Carte->load(sf::Randomizer::Random(1, 4));

	// Chargement des chateaux
	for(int i=0 ; i<5 ; i++){
		objetManager->add("chateau",new Chateau());
	}

	// Chargement du Menu
	sf::Sprite SpriteMenu;
	SpriteMenu.SetImage(*imageManager->GetImage("images/menu.psd"));
	SpriteMenu.SetCenter(0,0);
	SpriteMenu.SetPosition(0,395);

	// Creation des chaînes affichables à l'écran
	sf::String scoreTitre("Score : ", sf::Font::GetDefaultFont(), 12);
	scoreTitre.SetPosition(175.f, 410.f);
	sf::String score("0", sf::Font::GetDefaultFont(), 20);
	score.SetPosition(160.f, 425.f);
	sf::String objectif("Veuillez entourer un Chateau de mur", sf::Font::GetDefaultFont(), 16);
	sf::String chronoTitre("Temps restant : ", sf::Font::GetDefaultFont(), 12);
	sf::String chrono("30", sf::Font::GetDefaultFont(), 20);

	//Chargement du Viseur
	sf::Sprite SpriteViseur;
	SpriteViseur.SetImage(*imageManager->GetImage("images/viseur.psd"));
	SpriteViseur.SetCenter(5,5);

	bool isMouseUp = true;
	bool boucleAssauts = false;
	bool gameOver = false;

	sf::Clock ClockTimer;
	sf::Clock ClockTir;
	float TimeTimer;
	float TimeTir;
	int tempsRestant;
	int nbBateau = 5;
	int nbCanon = 2;

	int _score = 0;

    // Start game loop
    while (App.IsOpened())
    {
		if(!gameOver){
			// Récupération du temps
			TimeTimer = ClockTimer.GetElapsedTime();
			TimeTir = ClockTir.GetElapsedTime();
			if(phaseJeux == "assauts") tempsRestant = 15 - TimeTimer;
			else tempsRestant = 30 - TimeTimer;

			chrono.SetText(to_string(tempsRestant));

			// Capture la position de la souris sur la grille
			_Mouse = Grille->getPos(Coordonnees(Input.GetMouseX(),Input.GetMouseY()),1);
		}

        // Process events
        sf::Event Event;
		while (App.GetEvent(Event))
        {
			if(!gameOver){
				// Gestion cursor de la souris
				if ((Event.Type == sf::Event::MouseMoved) && (Event.MouseMove.Y >= 400)){
					App.ShowMouseCursor(true);
				}else{
					App.ShowMouseCursor(false);
				}
			}else{
				App.ShowMouseCursor(true);
			}

            // Close window : exit
			if ((Event.Type == sf::Event::Closed) || ((Event.Type == sf::Event::MouseButtonPressed) && (Event.MouseButton.X >= 668 && Event.MouseButton.X <= 766) && (Event.MouseButton.Y >= 420 && Event.MouseButton.Y <= 442) && (Event.MouseButton.Button == sf::Mouse::Left)))
				App.Close();
			if ((Event.Type == sf::Event::MouseButtonPressed) && (Event.MouseButton.X >= 38 && Event.MouseButton.X <= 134) && (Event.MouseButton.Y >= 420 && Event.MouseButton.Y <= 442) && (Event.MouseButton.Button == sf::Mouse::Left)){
				//Bouton Rejouer

				Grille->delAll();

				// detection des zone fermé floodFill
				Grille->start_checkClose();
				
				// recuperation des zones fermé
				allTerre = Grille->grilleClose();

				phaseJeux = "placementMur";
				iBlockMur = 0;
				iCanon = 0;
				isMouseUp = false;
				boucleAssauts = false;
				murTouche = 0;
				gameOver = false;
				nbBateau = 5;
				nbCanon = 2;
				_score = 0;
				score.SetText(to_string(_score));
				tempsRestant = 30;
				ClockTimer.Reset();
				objetManager->delAll();
			}

			// Mouse Wheel
			if(phaseJeux == "placementMur" && Event.Type == sf::Event::MouseWheelMoved)
			{
				((BlockMur*)objetManager->get("blockmur",iBlockMur))->Rotation(Event.MouseWheel.Delta);
			}

			if(phaseJeux == "assauts"){
				//Rotation Canon
				if(Event.Type == sf::Event::MouseMoved)
				{
					for(int i=0;i<objetManager->getSize("canon");i++){
						((Canon*)objetManager->get("canon",i))->rotate(Coordonnees(Event.MouseMove.X, Event.MouseMove.Y));
					}
				}

				// Tir des canon
				if(Event.Type == sf::Event::MouseButtonPressed && Event.MouseButton.Button == sf::Mouse::Left)
				{
					for(int i=0;i<objetManager->getSize("canon");i++){
						Coordonnees canon = ((Canon*)objetManager->get("canon",i))->getSpritePos();
						((Canon*)objetManager->get("canon",i))->tir(canon,Coordonnees(Event.MouseButton.X, Event.MouseButton.Y));
					}
				}
			}
        }

/**/	if(phaseJeux == "placementMur"){
			if(!objetManager->get("blockmur",iBlockMur)){
				objetManager->add("blockmur", new BlockMur(sf::Randomizer::Random(2,5)));
				objectif.SetText("Veuillez entourer un Chateau de mur");
				objectif.SetPosition(250.f, 415.f);
				chronoTitre.SetPosition(550.f, 410.f);
				chrono.SetPosition(575.f, 425.f);
			}

			if(Input.IsMouseButtonDown(sf::Mouse::Left) && isMouseUp)
			{
				isMouseUp = false;
				if(((BlockMur*)objetManager->get("blockmur",iBlockMur))->ToGrille())
				{
					objetManager->add("blockmur", new BlockMur(sf::Randomizer::Random(2,5)));
					iBlockMur++;
					_score += 100;

					// detection des zone fermé floodFill
					Grille->start_checkClose();
				
					// recuperation des zones fermé
					allTerre = Grille->grilleClose();
				}
			}
			else if(!Input.IsMouseButtonDown(sf::Mouse::Left) && !isMouseUp)
			{
				isMouseUp = true;
			}

			// Positionement du block mur en cours sur la position souris de la grille
			((BlockMur*)objetManager->get("blockmur",iBlockMur))->SetPosition(_Mouse.getX(),_Mouse.getY());

			if(tempsRestant < 0){
				int murFerme = 0;
				score.SetText(to_string(_score));

				for(int i=0;i<5;i++){
					int x = (((Chateau*)objetManager->get("chateau",i))->getSprite().GetPosition().x);
					int y = (((Chateau*)objetManager->get("chateau",i))->getSprite().GetPosition().y);
					if(Grille->getListeCases((x-5)/10, (y-5)/10) == 0){
						murFerme++;
						_score += 1000;
					}
				}if(murFerme == 5){
					//Victoire
					gameOver = true;
					phaseJeux = "";
					objectif.SetText("Victoire");
					objectif.SetPosition(350.f, 415.f);
					chrono.SetText("0");
				}else if(murFerme > 0){
					phaseJeux = "placementCanon";
					ClockTimer.Reset();
					objetManager->del("blockmur", iBlockMur);
				}else{
					//Game Over
					gameOver = true;
					phaseJeux = "";
					objectif.SetText("Game Over");
					objectif.SetPosition(340.f, 415.f);
					chrono.SetText("0");
				}
			}
/**/	}else if(phaseJeux == "placementCanon"){
			if(!objetManager->get("canon",iCanon)){
				objetManager->add("canon", new Canon(_Mouse));
				objectif.SetText("Veuillez placer "+to_string(nbCanon)+" canon dans une zone fermé");
				chronoTitre.SetPosition(590.f, 410.f);
				chrono.SetPosition(615.f, 425.f);
			}

			if(Input.IsMouseButtonDown(sf::Mouse::Left) && isMouseUp)
			{
				isMouseUp = false;
				if((Grille->getListeCases((_Mouse.getX()-5)/10, (_Mouse.getY()-5)/10) == 0) && Grille->isFree(Coordonnees(Input.GetMouseX(), Input.GetMouseY()), 3) && Grille->isFree(_Mouse, 1))
				{
					objetManager->add("canon", new Canon(_Mouse));
					iCanon++;
				}
			}
			else if(!Input.IsMouseButtonDown(sf::Mouse::Left) && !isMouseUp)
			{
				isMouseUp = true;
			}

			// Positionement du canon en cours sur la position souris de la grille
			((Canon*)objetManager->get("canon",iCanon))->SetPosition(_Mouse.getX(),_Mouse.getY());

			if(iCanon == nbCanon){
				phaseJeux = "assauts";
				ClockTimer.Reset();
				objetManager->del("canon", iCanon);
				nbCanon += 2;
				chronoTitre.SetPosition(550.f, 410.f);
				chrono.SetPosition(575.f, 425.f);
			}
			if(tempsRestant < 0 && iCanon < nbCanon){
				//Game Over
				gameOver = true;
				phaseJeux = "";
				objectif.SetText("Game Over");
				objectif.SetPosition(340.f, 415.f);
				chronoTitre.SetPosition(550.f, 410.f);
				chrono.SetText("0");
				chrono.SetPosition(575.f, 425.f);
			}
/**/	}else if(phaseJeux == "assauts"){
			if(boucleAssauts == false){
				for(int i=0 ; i<nbBateau ; i++){
					objetManager->add("bateau",new Bateau());
				}
				objectif.SetText("Defendez vos chateaux !");
				objectif.SetPosition(310.f, 415.f);
				boucleAssauts = true;
			}

			// Deplacement et tir des bateaux
			for(int i=0;i<objetManager->getSize("bateau");i++){
				((Bateau*)objetManager->get("bateau",i))->move();
				if(TimeTir > 2){
					((Bateau*)objetManager->get("bateau",i))->tir();
					ClockTir.Reset();
				}
			}

			// Deplacement des missill
			for(int i=objetManager->getSize("missill")-1;i>=0;i--){
				int result = ((Missill*)objetManager->get("missill", i))->move();
				if(result == -1) objetManager->del("missill", i);
				else if(result == 1){ 
					objetManager->del("missill", i);
					murTouche++;
					_score -= 100;
				}
				else if(result == 2){
					objetManager->del("missill", i);
					_score += 200;
				}
			}

			// Positionement du viseur sur la position de la souris
			SpriteViseur.SetPosition(Input.GetMouseX(), Input.GetMouseY());

			if(tempsRestant < 0 || objetManager->getSize("bateau") == 0){
				phaseJeux = "placementMur";
				ClockTimer.Reset();
				objetManager->del("missill");
				objetManager->del("bateau");
				score.SetText(to_string(_score));
				nbBateau += 5;
				boucleAssauts = false;
				if(murTouche > 0){
					murTouche = 0;
					// detection des zone fermé floodFill
					Grille->start_checkClose();
				
					// recuperation des zones fermé
					allTerre = Grille->grilleClose();

					for(int i=objetManager->getSize("canon")-1;i>=0;i--){
						Coordonnees posCanon = ((Canon*)objetManager->get("canon",i))->getSpritePos();
						if(Grille->getListeCases((posCanon.getX()-5)/10,(posCanon.getY()-5)/10) != 0){
							iCanon --;
							objetManager->del("canon", i);
						}
					}

					int stop = 0;
				}
			}
		}

        // Clear screen
        App.Clear();

		// affiche la carte
		App.Draw(Carte->getSprite());

		// Affichage des terres zone fermé
		for(int i=0; i<allTerre.size(); i++) App.Draw(allTerre[i]->getSprite());
		
		// Affichage des bateaux
		for(int i=0;i<objetManager->getSize("bateau");i++) App.Draw(objetManager->get("bateau",i)->getSprite());

		// Affichage des chateaux
		for(int i=0;i<objetManager->getSize("chateau");i++) App.Draw(objetManager->get("chateau",i)->getSprite());

		// Affichage des blocks murs
		for(int i=0;i<objetManager->getSize("blockmur");i++)
		{
			std::vector<Mur*> allMur = ((BlockMur*)objetManager->get("blockmur",i))->getMur();
			
			// Affichage des murs du block actuelle
			for(int i2=0; i2<allMur.size(); i2++){
				if(allMur[i2]->getIsVisible()) App.Draw(allMur[i2]->getSprite());
			}
		}

		for(int i=0;i<objetManager->getSize("canon");i++) App.Draw(objetManager->get("canon", i)->getSprite());

		for(int i=0;i<objetManager->getSize("missill");i++) App.Draw(objetManager->get("missill", i)->getSprite());

		if(phaseJeux == "assauts") App.Draw(SpriteViseur);
		
		App.Draw(SpriteMenu);
		App.Draw(scoreTitre);
		App.Draw(score);
		App.Draw(objectif);
		App.Draw(chronoTitre);
		App.Draw(chrono);

        // Display window contents on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}