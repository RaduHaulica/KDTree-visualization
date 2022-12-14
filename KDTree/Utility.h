#pragma once
#include <SFML/Graphics.hpp>

float distanceBetweenPoints(sf::Vector2f p1, sf::Vector2f p2);

std::vector<float> Vector2fToVector(sf::Vector2f v);

sf::Vector2f VectorToVector2f(std::vector<float> v);