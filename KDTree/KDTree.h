#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Utility.h"
#include "Node.h"

class KDTree : public sf::Drawable
{
public:
	Node* tree_;
	std::vector<sf::VertexArray> treeVisualization_;
	std::vector<sf::VertexArray> algorithmVisualization_;
	std::vector<std::vector<sf::RectangleShape>> boxVisualization_;
	int k_, numberOfNodes_, currentTreeVisualizationStep_, currentBoxVisualizationStep_;
	float inputAccumulator_, inputCooldownDuration_;
	bool drawTree_, drawBox_;

	KDTree();
	~KDTree();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates) const;

	Node* createBalancedTreeFromVector(std::vector<sf::Vector2f>& points);
	void createInternalProgressiveVisualizationFromTree(Node* root);
	void createVisualizationFromTree(Node* root, sf::VertexArray& va);
	void createBoxVisualizationFromTree(Node* root, sf::Vector2f topLeft, sf::Vector2f topRight);
	sf::Vector2f closestPoint(Node* root, sf::Vector2f point);
	void findPointsInRange(Node* root, sf::Vector2f point, float range, int depth = 0);

	void update(float dt);

private:
	void createInternalProgressiveVisualizationFromTreeRecursive(Node* root, int level, sf::Color color = sf::Color::Green);

	void drawRecursive(sf::RenderTarget& target, sf::RenderStates, Node* root) const;

	Node* createBalancedTreeRecursive(std::vector<std::vector<float>>& points, int startIndex, int endIndex, int depth);

	void markInactiveRecursive(Node* root);
	void markActiveRecursive(Node* root);

	float firstPass(Node* root, sf::Vector2f point);
	sf::Vector2f firstPassRecursive(Node* root, sf::Vector2f point, int depth);
	sf::Vector2f closestPointRecursive(Node* root, sf::Vector2f point, int depth, float min_dist);

	void visualizationFromTreeRecursive(Node* root, sf::VertexArray& va, sf::Color color = sf::Color::Green);
	void boxVisualizationFromTreeRecursive(Node* root, int level, sf::Vector2f topLeft, sf::Vector2f bottomRight, sf::Color color = sf::Color::Green);
	void getNodesRecursive(Node* root, std::vector<Node*>& nodeList);
};