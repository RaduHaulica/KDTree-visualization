#include "Node.h"

Node::Node() :
	left(nullptr),
	right(nullptr)
{
	point_ = Vector2fToVector(sf::Vector2f({ 0, 0 }));
}

void Node::update(float dt)
{
	pointVisualization_.update(dt);
	if (left) left->update(dt);
	if (right) right->update(dt);
}

void Node::draw(sf::RenderTarget& target, sf::RenderStates rs) const
{
	target.draw(pointVisualization_);
	if (left) target.draw(*left);
	if (right) target.draw(*right);
	target.draw(rectangleVisualization_);
}

void Node::setPoint(std::vector<float> point, sf::Color color)
{
	point_ = point;
	pointVisualization_.initialize(VectorToVector2f(point), color);
}