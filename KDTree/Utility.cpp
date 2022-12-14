#include "Utility.h"

float distanceBetweenPoints(sf::Vector2f p1, sf::Vector2f p2)
{
	return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

std::vector<float> Vector2fToVector(sf::Vector2f v)
{
	std::vector<float> result;
	result.push_back(v.x);
	result.push_back(v.y);
	return result;
}

sf::Vector2f VectorToVector2f(std::vector<float> v)
{
	sf::Vector2f result;
	result.x = v[0];
	result.y = v[1];
	return result;
}