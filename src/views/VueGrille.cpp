#include <SFML/Graphics.hpp>
#include <iostream>
#include "../../inclusion/VueGrille.h"
#include "../../inclusion/TileMap.h"

VueGrille::VueGrille(int longueur, int hauteur, int tailleCase, int niveau[]) : sf::View(sf::FloatRect(0, 0, 20 * tailleCase, 9 * tailleCase))
{
	compteurLongueur = 0;
	compteurHauteur = 0;
	
	// on cr�e la tilemap avec le niveau pr�c�demment d�fini

	if (carte.load("ressources/textures/textures.png", sf::Vector2u(tailleCase, tailleCase), niveau, longueur, hauteur))
	{
		this->setViewport(sf::FloatRect(0, 0, 1, 0.75f));
	}
}

