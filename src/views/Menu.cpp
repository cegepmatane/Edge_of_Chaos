# include <SFML/Graphics.hpp>
# include <iostream>
# include <string>
# include <vector>
# include "../../inclusion/Menu.h"
# include "../../inclusion/Bouton.h"
# include "../../inclusion/FenetreJeu.h"
#include "../../inclusion/Serveur.h"
# if defined (__linux__)
# include "../../systemes/interfaceDebian.h"
# elif defined (_WIN32) || (_WIN64)
# include "../../systemes/interfaceWindows.h"
# endif


Menu::Menu() : RenderWindow(sf::VideoMode(sf::VideoMode::getDesktopMode().width / 3, sf::VideoMode::getDesktopMode().height / 2), "Edge of Chaos", sf::Style::Close)
{
	std::string policeTitre("fraktur-bt.ttf");
	const std::string policeBoutons("urw-chancery-l-medium-Menus.ttf");
	const double dimensionBoutons = 5 / 9.0;
	this->jeu = nullptr;

	initFenetre();
	initTitre(policeTitre);
	initBoutons(policeBoutons, dimensionBoutons);
	initErreur(policeBoutons);

}

void Menu::afficher()
{
	this->draw(this->titre);
	this->draw(*(this->actionHeberger));
	this->draw(*(this->actionJoindre));
	this->draw(*(this->actionParametres));
	if(!(this->erreur.getString().isEmpty())){
		this->draw(this->cadreErreur);
		this->draw(erreur);
	}
}

void Menu::lancerErreur(std::string& messageErreur)
{
	this->erreur.setString(messageErreur);
}

Bouton Menu::getHeberger()
{
	return *(this->actionHeberger);
}

Bouton Menu::getJoindre()
{
	return *(this->actionJoindre);
}

Bouton Menu::getParametres()
{
	return *(this->actionParametres);
}

std::vector<int> Menu::getHebergerPosition()
{
	std::vector<int> position;
	position.push_back(actionHeberger->getPosition().x);
	position.push_back(actionHeberger->getPosition().y);
	return position;
}

std::vector<int> Menu::getJoindrePosition()
{
	std::vector<int> position;
	position.push_back(actionJoindre->getPosition().x);
	position.push_back(actionJoindre->getPosition().y);
	return position;
}

Menu::~Menu()
{
	if(this->actionHeberger != nullptr){
		delete this->actionHeberger;
		this->actionHeberger = nullptr;
	}
	if(this->actionJoindre != nullptr){
		delete this->actionJoindre;
		this->actionJoindre = nullptr;
	}
	if(this->actionParametres != nullptr){
		delete this->actionParametres;
		this->actionParametres = nullptr;
	}
}

void Menu::initFenetre()
{
	this->setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 3, sf::VideoMode::getDesktopMode().height / 2 - sf::VideoMode::getDesktopMode().height / 4));
	this->setVerticalSyncEnabled(true);
}

void Menu::initTitre(const std::string& police)
{
	if(!(this->policeTitre.loadFromFile(Configuration::cheminPolices + police))){
		std::cerr << "Impossible de charger la police du titre ( " << Configuration::cheminPolices + police << " )." << std::endl;
	}
	this->titre.setFont(this->policeTitre);
	this->titre.setString("Edge Of Chaos");
	this->titre.setCharacterSize(35);
	this->titre.setFillColor(sf::Color::Black);
	this->titre.setPosition(this->getSize().x / 2 - this->getSize().x / 4, 10);
}

void Menu::initBoutons(const std::string& police, const double proportion)
{
	this->positionXBoutons = this->getSize().x * (1 - proportion) / 2 ;	// Somme des fractions doit etre egale a 1.
	this->offsetYBoutons = this->titre.getCharacterSize() + this->titre.getPosition().y;
	this->espacementBoutons = 20;
	
	const int _HAUTEUR = this->getSize().y / 6;	
	const int _LONGUEUR = this->getSize().x * proportion;
	
	this->actionHeberger = new Bouton(_LONGUEUR, _HAUTEUR, "Heberger", police);
	this->actionHeberger->setBtnPosition(this->positionXBoutons, this->offsetYBoutons + 50 + (_HAUTEUR + this->espacementBoutons) * 0);
	this->actionJoindre = new Bouton(_LONGUEUR, _HAUTEUR, "Joindre", police);
	this->actionJoindre->setBtnPosition(this->positionXBoutons, this->offsetYBoutons + 50 + (_HAUTEUR + this->espacementBoutons) * 1);
	this->actionParametres = new Bouton(_LONGUEUR, _HAUTEUR, "Parametres", police);
	this->actionParametres->setBtnPosition(this->positionXBoutons, this->offsetYBoutons + 50 + (_HAUTEUR + this->espacementBoutons) * 2);
}

void Menu::initErreur(const std::string& police)
{
	this->cadreErreur.setSize(sf::Vector2f(this->getSize().x, this->getSize().y / 20));
	this->cadreErreur.setFillColor(sf::Color::Red);
	this->cadreErreur.setPosition(0, this->getSize().y - this->cadreErreur.getSize().y);

	if(!this->policeErreur.loadFromFile(Configuration::cheminPolices + police))
	{
		std::cerr << "Impossible de charger la police d'erreur \" " << police << "\"." << std::endl;
	}
	this->erreur.setFont(this->policeErreur);
	this->erreur.setFillColor(sf::Color::White);
	this->erreur.setCharacterSize(this->cadreErreur.getSize().y);
	this->erreur.setPosition(0, this->cadreErreur.getPosition().y - 4);
}

void Menu::initJeu(Niveau niveau, std::vector<Unite*>* unites, std::vector<Batiment*>* batiments, bool serveurPresent)
{
	if (serveurPresent)
	{
		Serveur* serveur = new Serveur();
		serveur->demarrer();
		this->jeu = new FenetreJeu(niveau, unites, batiments, serveur);
	} else
	{
		this->jeu = new FenetreJeu(niveau, unites, batiments);
	}
	this->jeu->lancerBoucle(this);
}
