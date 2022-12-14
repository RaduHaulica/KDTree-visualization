#include "Point.h"

//Point::Point() :
//	active_(true),
//	color_(sf::Color::Blue),
//	radius_(0)
//{
//	circle_.setFillColor(color_);
//	circle_.setRadius(radius_);
//	circle_.setOrigin({ radius_, radius_ });
//	circle_.setPosition(sf::Vector2f({0,0}));
//}

Point::Point() :
	active_{ true }
{
}

Point::Point(sf::Vector2f origin, sf::Color color, float radius) :
	color_(color),
	active_(true)
{
	circle_.setFillColor(color_);
	circle_.setRadius(radius_);
	circle_.setOrigin({ radius_, radius_ });
	circle_.setPosition(origin);
}

void Point::update(float dt)
{
	circle_.setFillColor(color_);
}

void Point::initialize(sf::Vector2f origin, sf::Color color, float radius)
{
	active_ = true;
	color_ = color;
	radius_ = radius;
	circle_.setFillColor(color_);
	circle_.setRadius(radius_);
	circle_.setOrigin({ radius_, radius_ });
	circle_.setPosition(origin);
}

void Point::markInactive()
{
	sf::Color color = sf::Color::Black;
	color.r = 200;
	color.g = 200;
	color.b = 200;
	circle_.setFillColor(color);
	color_ = color;
}

void Point::markActive()
{
	color_ = sf::Color::Blue;
}

void Point::markVisited()
{
	color_ = sf::Color::Cyan;
}

void Point::markInRange()
{
	color_ = sf::Color::Red;
}

void Point::draw(sf::RenderTarget& target, sf::RenderStates rs) const
{
	target.draw(circle_, rs);
}