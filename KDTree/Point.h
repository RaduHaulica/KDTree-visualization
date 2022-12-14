#pragma once
#include <SFML/Graphics.hpp>
#include "Utility.h"

class Point : public sf::Drawable
{
public:
	sf::CircleShape circle_;
	float radius_;
	sf::Color color_;
	bool active_;

	Point();
	Point(sf::Vector2f origin, sf::Color color = sf::Color::Blue, float radius = 2.0f);

	void initialize(sf::Vector2f origin, sf::Color color = sf::Color::Blue, float radius = 2.0f);
	void update(float dt);
	void markInactive();
	void markActive();
	void markVisited();
	void markInRange();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates rs) const;
};