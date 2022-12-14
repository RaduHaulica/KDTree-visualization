#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Point.h"

class Node : public sf::Drawable
{
public:
	std::vector<float> point_;
	Point pointVisualization_;
	Node* left, * right;
	sf::RectangleShape rectangleVisualization_;
	//sf::Vector2f boundaryTopLeft, boundaryBottomRight;
	Node();

	void update(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates rs) const;
	void setPoint(std::vector<float> point, sf::Color = sf::Color::Blue);
};