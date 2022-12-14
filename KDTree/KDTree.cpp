#include "KDTree.h"
#include <cassert>

KDTree::KDTree() :
	drawTree_{ false },
	drawBox_{ false },
	k_(2),
	currentTreeVisualizationStep_(0),
	currentBoxVisualizationStep_{ 0 },
	numberOfNodes_(0),
	inputAccumulator_{ 0 },
	inputCooldownDuration_{ 0.1f },
	tree_(nullptr)
{
}

void KDTree::update(float dt)
{
	if (inputAccumulator_ < 0)
	{
		inputAccumulator_ = std::fmin(0, inputAccumulator_ + dt);
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			currentTreeVisualizationStep_ = std::max(0, currentTreeVisualizationStep_ - 1);
			inputAccumulator_ -= inputCooldownDuration_;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			currentTreeVisualizationStep_ = std::min((int)treeVisualization_.size() - 1, currentTreeVisualizationStep_ + 1);
			inputAccumulator_ -= inputCooldownDuration_;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			currentBoxVisualizationStep_ = std::max(0, currentBoxVisualizationStep_ - 1);
			inputAccumulator_ -= inputCooldownDuration_;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			currentBoxVisualizationStep_ = std::min((int)boxVisualization_.size() - 1, currentBoxVisualizationStep_ + 1);
			inputAccumulator_ -= inputCooldownDuration_;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
		{
			drawTree_ = !drawTree_;
			inputAccumulator_ -= inputCooldownDuration_;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
		{
			drawBox_ = !drawBox_;
			inputAccumulator_ -= inputCooldownDuration_;
		}
	}

	tree_->update(dt);
}

/*
* draws the graphical representation of a tree (via recursive helper) on the canvas
*/
void KDTree::draw(sf::RenderTarget& target, sf::RenderStates rs) const
{
	//std::cout << "Current tree visualization step: " << currentTreeVisualizationStep_ << std::endl;
	//std::cout << "Current box visualization step: " << currentBoxVisualizationStep_ << std::endl;
	if (drawBox_)
	{
		for (int i = 0; i <= currentBoxVisualizationStep_; i++)
		{
			for (int j = 0; j < boxVisualization_[i].size(); j++)
			{
				target.draw(boxVisualization_[i][j]);
			}
		}
	}
	//for (int j = 0; j < boxVisualization_[currentBoxVisualizationStep_].size(); j++)
	//{
	//	target.draw(boxVisualization_[currentBoxVisualizationStep_][j]);
	//}
	if (drawTree_)
	{
		for (int i = 0; i <= currentTreeVisualizationStep_; i++)
		{
			target.draw(treeVisualization_[i]);
		}
	}
	drawRecursive(target, rs, tree_);
}

/*
* recursive helper that calls draw on all children
*/
void KDTree::drawRecursive(sf::RenderTarget& target, sf::RenderStates rs, Node* root) const
{
	if (root)
	{
		root->pointVisualization_.draw(target, rs);
		if (root->left)
		{
			drawRecursive(target, rs, root->left);
		}
		if (root->right)
		{
			drawRecursive(target, rs, root->right);
		}
	}
}

/*
* recursive helper function for createBalancedTreeFromVector
*/
Node* KDTree::createBalancedTreeRecursive(std::vector<std::vector<float>>& points, int startIndex, int endIndex, int depth)
{
	assert(startIndex <= endIndex);
	//std::cout << "Recursion called: start: " << startIndex << ", end: " << endIndex;
	Node* result = new Node();
	// early paths out
	if (endIndex <= startIndex)
	{
		//std::cout << " Returned" << std::endl;
		result->setPoint(points[startIndex]);
		return result;
	}

	if (endIndex - startIndex == 1)
	{
		//std::cout << "Returned" << std::endl;
		result->setPoint(points[startIndex]);
		result->right = createBalancedTreeRecursive(points, endIndex, endIndex, depth + 1);
		return result;
	}

	// average path
	//std::cout << std::endl;
	int axis = depth % k_;
	//std::cout << "Sorting by " << ((coordinate == 0) ? "x" : "y") << ": ";
	for (int i = startIndex; i <= endIndex; i++)
	{
		//std::cout << "{" << points[i][0] << ", " << points[i][1] << "}, ";
	}
	//std::cout << std::endl;
	// 
	// k-dimension sorting
	std::sort(points.begin() + startIndex, points.begin() + endIndex + 1, [axis](const std::vector<float>& v1, const std::vector<float>& v2) {
		return v1[axis] < v2[axis];
		});
	//std::cout << "Sorted: ";
	//for (int i = startIndex; i <= endIndex; i++)
	//{
	//	std::cout << "{" << points[i][0] << ", " << points[i][1] << "}, ";
	//}
	//std::cout << std::endl;
	result->setPoint(points[startIndex + (endIndex - startIndex) / 2]);
	result->left = createBalancedTreeRecursive(points, startIndex, startIndex + (endIndex - startIndex) / 2 - 1, depth + 1);
	result->right = createBalancedTreeRecursive(points, startIndex + (endIndex - startIndex) / 2 + 1, endIndex, depth + 1);
	return result;
}

/*
* creates a balanced binary tree from a vector of Vector2f points (uses recursive helper function createBalancedTreeRecursive)
*/
Node* KDTree::createBalancedTreeFromVector(std::vector<sf::Vector2f>& points)
{
	Node* result = nullptr;
	std::vector<std::vector<float>> pointsConverted(points.size());
	for (int i = 0; i < points.size(); i++)
	{
		pointsConverted[i].push_back(Vector2fToVector(points[i])[0]);
		pointsConverted[i].push_back(Vector2fToVector(points[i])[1]);
	}
	result = createBalancedTreeRecursive(pointsConverted, 0, pointsConverted.size() - 1, 0);
	return result;
}

void KDTree::findPointsInRange(Node* root, sf::Vector2f point, float range, int depth)
{
	if (root == nullptr)
		return;
	markActiveRecursive(root);
	root->pointVisualization_.markVisited();
	if (distanceBetweenPoints(VectorToVector2f(root->point_), point) <= range)
	{
		root->pointVisualization_.markInRange();
	}

	int c = depth % k_;

	if (root->left && Vector2fToVector(point)[c] - root->point_[c] < range)
	{
		//std::cout << "Checking left..." << std::endl;
		findPointsInRange(root->left, point, range, depth + 1);
	}

	if (root->right && Vector2fToVector(point)[c] - root->point_[c] >= -range)
	{
		//std::cout << "Checking right..." << std::endl;
		findPointsInRange(root->right, point, range, depth + 1);
	}
}

/*
* recursive helper function for firstPass
*/
sf::Vector2f KDTree::firstPassRecursive(Node* root, sf::Vector2f point, int depth)
{
	if (root->left == nullptr && root->right == nullptr)
	{
		return VectorToVector2f(root->point_);
	}
	int c = depth % k_;
	if (root->left && Vector2fToVector(point)[c] < root->point_[c])
	{
		return firstPassRecursive(root->left, point, depth + 1);
	}
	else
	{
		if (root->right)
		{
			return firstPassRecursive(root->right, point, depth + 1);
		}
	}
}

/*
* recursively marks nodes as inactive in the current graphical representation of a KDTree search
*/
void KDTree::markInactiveRecursive(Node* root)
{
	if (!root)
		return;

	root->pointVisualization_.markInactive();
	markInactiveRecursive(root->left);
	markInactiveRecursive(root->right);
}

void KDTree::markActiveRecursive(Node* root)
{
	if (!root)
		return;

	root->pointVisualization_.markActive();
	markActiveRecursive(root->left);
	markActiveRecursive(root->right);
}

/*
* finds a first candidate (minimum distance) for closest node to a given point based on simple k-dimension comparisons
*/
float KDTree::firstPass(Node* root, sf::Vector2f point)
{
	sf::Vector2f closest = firstPassRecursive(root, point, 0);
	//_firstPassPoint = Point(closest, sf::Color::Yellow);
	return distanceBetweenPoints(closest, point);
}

/*
* recursive helper function for closestPoint
*/
sf::Vector2f KDTree::closestPointRecursive(Node* root, sf::Vector2f point, int depth, float min_dist)
{
	sf::Vector2f minLeft, minRight;
	sf::Vector2f currentPoint = VectorToVector2f(root->point_);
	sf::Vector2f result = currentPoint;
	markActiveRecursive(root);
	root->pointVisualization_.markVisited();

	float current_min_dist = distanceBetweenPoints(currentPoint, point);
	//std::cout << "Min dist: " << current_min_dist << std::endl;
	int c = depth % k_;

	if (root->left && Vector2fToVector(point)[c] - root->point_[c] < min_dist)
	{
		//std::cout << "Checking left..." << std::endl;
		minLeft = closestPointRecursive(root->left, point, depth + 1, min_dist);
		if (distanceBetweenPoints(minLeft, point) < distanceBetweenPoints(result, point))
		{
			//std::cout << "Min was left: {" << minLeft.x << ", " << minLeft.y << "}" << std::endl;
			result = minLeft;
		}
	}
	else
	{
		//markInactiveRecursive(root->left);
	}

	if (root->right && Vector2fToVector(point)[c] - root->point_[c] >= -min_dist)
	{
		//std::cout << "Checking right..." << std::endl;
		minRight = closestPointRecursive(root->right, point, depth + 1, min_dist);
		if (distanceBetweenPoints(minRight, point) < distanceBetweenPoints(result, point))
		{
			//std::cout << "Min was right: {" << minRight.x << ", " << minRight.y << "}" << std::endl;
			result = minRight;
		}
	}
	else
	{
		//markInactiveRecursive(root->right);
	}
	return result;
}

/*
* finds the closest node in the KDTree to a given point
*/
sf::Vector2f KDTree::closestPoint(Node* root, sf::Vector2f point)
{
	float baseline = firstPass(root, point);
	//std::cout << "First baseline distance: " << baseline << std::endl;
	return closestPointRecursive(root, point, 0, baseline);
}

/*
* recursive helper function for createVisualizationFromTree
*/
void KDTree::visualizationFromTreeRecursive(Node* root, sf::VertexArray& va, sf::Color color)
{
	if (root && root->left)
	{
		va.append({ VectorToVector2f(root->point_), color });
		va.append({ VectorToVector2f(root->left->point_), color });
		visualizationFromTreeRecursive(root->left, va);
	}
	if (root && root->right)
	{
		va.append({ VectorToVector2f(root->point_), color });
		va.append({ VectorToVector2f(root->right->point_), color });
		visualizationFromTreeRecursive(root->right, va);
	}
}

/*
* creates a graphical representation (sf::VertexArray) of a KDTree's parent -> child lines
*/
void KDTree::createVisualizationFromTree(Node* root, sf::VertexArray& va)
{
	va.clear();
	va.setPrimitiveType(sf::PrimitiveType::Lines);
	visualizationFromTreeRecursive(root, va);
}

/*
* recursive helper function for createInternalProgressiveVisualizationFromTree
*/
void KDTree::createInternalProgressiveVisualizationFromTreeRecursive(Node* root, int level, sf::Color color)
{
	std::vector<sf::Color> colors{ sf::Color::Green, sf::Color::Red, sf::Color::Blue, sf::Color::Magenta, sf::Color::Cyan, sf::Color::Yellow };

	if (root == nullptr)
		return;
	if (level >= treeVisualization_.size())
	{
		treeVisualization_.push_back(sf::VertexArray(sf::PrimitiveType::Lines));
	}

	if (root->left)
	{
		treeVisualization_[level].append({ VectorToVector2f(root->point_), colors[level % (colors.size())] });
		treeVisualization_[level].append({ VectorToVector2f(root->left->point_), colors[level % (colors.size())] });
		createInternalProgressiveVisualizationFromTreeRecursive(root->left, level + 1);
	}
	if (root->right)
	{
		treeVisualization_[level].append({ VectorToVector2f(root->point_), colors[level % (colors.size())] });
		treeVisualization_[level].append({ VectorToVector2f(root->right->point_), colors[level % (colors.size())] });
		createInternalProgressiveVisualizationFromTreeRecursive(root->right, level + 1);
	}
}

/*
* creates progressive visualization (std::vector<sf::VertexArray> of a KDTree's parent -> child lines
*/
void KDTree::createInternalProgressiveVisualizationFromTree(Node* root)
{
	treeVisualization_.clear();
	createInternalProgressiveVisualizationFromTreeRecursive(root, 0);
	currentTreeVisualizationStep_ = treeVisualization_.size() - 1;
}

void KDTree::getNodesRecursive(Node* root, std::vector<Node*>& nodeList)
{
	if (root)
	{
		nodeList.push_back(root);
		getNodesRecursive(root->left, nodeList);
		getNodesRecursive(root->right, nodeList);
	}
}

KDTree::~KDTree()
{
	std::vector<Node*> nodes;
	getNodesRecursive(tree_, nodes);
	for (auto node : nodes)
	{
		delete node;
	}
}

void KDTree::createBoxVisualizationFromTree(Node* root, sf::Vector2f topLeft, sf::Vector2f bottomRight)
{
	boxVisualization_.clear();
	boxVisualizationFromTreeRecursive(root, 0, topLeft, bottomRight);
	currentBoxVisualizationStep_ = boxVisualization_.size() - 1;
}

void KDTree::boxVisualizationFromTreeRecursive(Node* root, int level, sf::Vector2f topLeft, sf::Vector2f bottomRight, sf::Color color)
{
	std::vector<sf::Color> colors{ sf::Color::Red, sf::Color::Green, sf::Color::Yellow };

	int coordinate = level % k_;

	if (root == nullptr)
		return;

	if (level >= boxVisualization_.size())
	{
		boxVisualization_.push_back(std::vector<sf::RectangleShape>());
	}

	sf::Vector2f offset{ 1,1 };
	sf::RectangleShape rs;
	rs.setPosition(topLeft + offset);
	rs.setSize(bottomRight - topLeft - 2.0f * offset);
	sf::Color fill = colors[level % (colors.size())];
	//sf::Color fill;
	//int gray = 200;
	//fill.r = gray;
	//fill.g = gray;
	//fill.b = gray;
	fill.a = 10;
	rs.setFillColor(fill);
	rs.setOutlineColor(sf::Color::White);
	rs.setOutlineThickness(1.0f);
	boxVisualization_[level].push_back(rs);

	if (root->left)
	{
		sf::Vector2f topLeftPoint = topLeft;
		sf::Vector2f bottomRightPoint = bottomRight;
		std::vector<float> temp = Vector2fToVector(bottomRightPoint);
		temp[coordinate] = root->point_[coordinate];
		bottomRightPoint = VectorToVector2f(temp);
		boxVisualizationFromTreeRecursive(root->left, level + 1, topLeftPoint, bottomRightPoint);
	}
	if (root->right)
	{
		sf::Vector2f topLeftPoint = topLeft;
		sf::Vector2f bottomRightPoint = bottomRight;
		std::vector<float> temp = Vector2fToVector(topLeft);
		temp[coordinate] = root->point_[coordinate];
		topLeftPoint = VectorToVector2f(temp);
		boxVisualizationFromTreeRecursive(root->right, level + 1, topLeftPoint, bottomRightPoint);
	}
}