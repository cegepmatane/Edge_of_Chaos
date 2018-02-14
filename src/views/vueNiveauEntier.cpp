#include <SFML/Graphics.hpp>
#include <iostream>
#include "../../inclusion/vueNiveauEntier.h"
#include "../../inclusion/TileMap.h"

VueNiveauEntier::VueNiveauEntier(int longueurNiveau, int hauteurNiveau, int tailleCase, int niveau[]) : sf::View(sf::FloatRect(0, 0, longueurNiveau * tailleCase, hauteurNiveau * tailleCase))
{
	// on cr�e la tilemap avec le niveau pr�c�demment d�fini

	carte.load("ressources/textures/textures.png", sf::Vector2u(tailleCase, tailleCase), niveau, longueurNiveau, hauteurNiveau);
}