#include <SFML/Graphics.hpp>
#include "../inclusion/FenetreJeu.h"
#include "string"
#include "vector"
# if defined (__linux__)
# include "../systemes/interfaceDebian.h"
# elif defined (_WIN32) || (_WIN64)
# include "../../systemes/interfaceWindows.h"
# endif

FenetreJeu::FenetreJeu(int longueurFenetre, int hauteurFenetre, std::string nomFenetre, Niveau niveau, int longueurGrille, int hauteurGrille,
	int tailleCase, int hauteurPanneau, std::string imagePanneau, std::vector<Unite*> &unites, std::vector<Batiment*> &batiments) :
	sf::RenderWindow(sf::VideoMode(longueurFenetre, hauteurFenetre), nomFenetre, sf::Style::Close),
	longueurFenetre(longueurFenetre), hauteurFenetre(hauteurFenetre), nomFenetre(nomFenetre),
	longueurGrille(longueurGrille), hauteurGrille(hauteurGrille), tailleCase(tailleCase),
	hauteurPanneau(hauteurPanneau), imagePanneau(imagePanneau), niveau(niveau), unites(unites), batiments(batiments),
	vueGrille(niveau, longueurGrille, hauteurGrille, tailleCase), vueGenerale(niveau, tailleCase),
	panneauBoisUnite(longueurGrille, hauteurPanneau, tailleCase, unites.front(), imagePanneau),
	panneauBoisBatiment(longueurGrille, hauteurPanneau, tailleCase, imagePanneau, batiments.front()),
	panneauBois(longueurGrille, hauteurPanneau, tailleCase, imagePanneau)
{
	this->uniteSelect = nullptr;
	this->batimentSelect = nullptr;
	this->estUnite = false;
	this->estBatiment= false;
	this->setView(vueGrille);
	estVueGrille = true;

	if (!textureCurseur.loadFromFile(Configuration::cheminTextures + "textures64.png", sf::IntRect(64*13, 0, 64, 64)))
		std::cerr << "Impossible de charger la texture du curseur. \n ( " << Configuration::cheminTextures + "textures64.png" << " )" << std::endl;
	spriteCurseur = sf::Sprite(textureCurseur);
	//spriteCurseur.setColor(sf::Color(255, 0, 0, 128));
	spriteCurseur.setPosition(0, 0);
}

void FenetreJeu::lancerBoucle(Menu* menu)
{
	menu->close();
	while (this->isOpen())
	{
		sf::Event event;
		while (this->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				this->close();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::R:
					if (estVueGrille)
					{
						estVueGrille = false;	// nom pas super super clair...
					}
					else
					{
						estVueGrille = true;
					}
					break;

				case sf::Keyboard::D: case sf::Keyboard::Right:
					if (estVueGrille&&vueGrille.getCompteurLongueur() != (tailleCase*niveau.getLongueur()) - (tailleCase * 20))
					{
						vueGrille.move(tailleCase, 0);
						vueGrille.setCompteurLongueur(vueGrille.getCompteurLongueur() + tailleCase);
					}
					break;

				case sf::Keyboard::Z: case sf::Keyboard::Up:
					if (estVueGrille&&vueGrille.getCompteurHauteur() != 0)
					{
						vueGrille.move(0, -tailleCase);
						vueGrille.setCompteurHauteur(vueGrille.getCompteurHauteur() - tailleCase);
					}
					break;

				case sf::Keyboard::S: case sf::Keyboard::Down:
					if (estVueGrille&&vueGrille.getCompteurHauteur() != (tailleCase*niveau.getHauteur()) - tailleCase*hauteurGrille)
					{
						vueGrille.move(0, tailleCase);
						vueGrille.setCompteurHauteur(vueGrille.getCompteurHauteur() + tailleCase);
					}
					break;

				case sf::Keyboard::Q: case sf::Keyboard::Left:
					if (estVueGrille&&vueGrille.getCompteurLongueur() != 0)
					{
						vueGrille.move(-tailleCase, 0);
						vueGrille.setCompteurLongueur(vueGrille.getCompteurLongueur() - tailleCase);
					}
					break;
				}
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				int clicX = sf::Mouse::getPosition(*this).x;
				int clicY = sf::Mouse::getPosition(*this).y;

				if (clicX < longueurFenetre && clicX > 0 && clicY < tailleCase*hauteurGrille && clicY > 0 && estVueGrille)
				{
					//Pour avoir les coordonnees du coin haut-gauche
					clicX = clicX - clicX % tailleCase;
					clicY = clicY - clicY % tailleCase;

					// Pour avoir les coordonnees absolues
					clicX += vueGrille.getCompteurLongueur();
					clicY += vueGrille.getCompteurHauteur();

					sf::Vector2f positionSouris(clicX, clicY);

					if (event.mouseButton.button == sf::Mouse::Left)
					{
						estUnite = false;
						estBatiment = false;
						uniteSelect = nullptr;
						batimentSelect = nullptr;
						
						spriteCurseur.setPosition(positionSouris);

						for (Unite* unite : unites)
						{
							if (positionSouris == unite->getVraiePosition())
							{
								estUnite = true;
								uniteSelect = unite;	// après le for, "unite" est désalloué,
								// donc "uniteSelect" pointe dans le vide.
							}
						}
						if (!estUnite)
						{
							for (Batiment* batiment : batiments)
							{
								if (positionSouris == batiment->getVraiePosition())
								{
									estBatiment = true;
									batimentSelect = batiment;	// idem
								}
							}
						}
					} else if (event.mouseButton.button == sf::Mouse::Right && uniteSelect != nullptr)
					{
						// Verification du contenu de la case cliquee
						bool caseOccupee = false;
						for (Unite* unite : unites)
						{
							if (positionSouris == unite->getVraiePosition())
							{
								caseOccupee = true;
								
							}
						}
						if(!caseOccupee){
							for (Batiment* batiment : batiments)
							{
								if (positionSouris == batiment->getVraiePosition())
								{
									caseOccupee = true;
								}
							}
						}

						// Mettre l'unite sur la case cliquee
						if (!caseOccupee)
						{
							uniteSelect->setPosition(positionSouris);
							uniteSelect->setVraiePosition(positionSouris.x, positionSouris.y);

							//uniteSelect = nullptr;
							spriteCurseur.setPosition(positionSouris);
						}
					}
				}
			}
		}

		this->clear(sf::Color::Black);

		//AFFICHER LA TILEMAP ET LE PANNEAU
		if (estVueGrille)
		{
			//Affichage de la carte, des entites et du curseur
			this->setView(vueGrille);
			this->draw(vueGrille.getCarte());
			
			for (Unite* unite : unites)
			{
				unite->setImage(Configuration::cheminTextures + "textures64.png", unite->getNumTexture());
				this->draw(*unite);
			}
			for (Batiment* batiment : batiments)
			{
				batiment->setImage(Configuration::cheminTextures + "textures64.png", batiment->getNumTexture());
				this->draw(*batiment);
			}
			this->draw(spriteCurseur);

			//Affichage du panneauBois
			if (estUnite)
			{
				panneauBoisUnite.setUnite(uniteSelect); // !!!!! "uniteSelect" est à nullptr les 3/4 du temps!
				this->setView(panneauBoisUnite);
				panneauBoisUnite.dessiner(*this);
			}
			else if (estBatiment)
			{
				panneauBoisBatiment.setBatiment(batimentSelect);
				this->setView(panneauBoisBatiment);
				panneauBoisBatiment.dessiner(*this);
			}
			else
			{
				this->setView(panneauBois);
				this->draw(panneauBois.getSprite());
			}
		}
		else
		{
			this->setView(vueGenerale);
			this->draw(vueGenerale.getCarte());
			for (Unite* unite : unites)
			{
				unite->setImage(Configuration::cheminTextures + "textures64.png", unite->getNumTexture());
				this->draw(*unite);
			}
			for (Batiment* batiment : batiments)
			{
				batiment->setImage(Configuration::cheminTextures + "textures64.png", batiment->getNumTexture());
				this->draw(*batiment);
			}
			this->draw(spriteCurseur);
		}
		this->display();
	}
}
