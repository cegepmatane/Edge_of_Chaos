#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "TileMap.h"

class VueGenerale : public sf::View
{
private:
	TileMap carte;

public:
	VueGenerale(int longueurNiveau, int hauteurNiveau, int tailleCase, int niveau[]);
	TileMap getCarte() { return carte; }
};
