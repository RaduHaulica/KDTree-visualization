#include <SFML/Graphics.hpp>
#include "Utility.h"

#include <iostream>

#include "Point.h"
#include "Node.h"
#include "KDTree.h"
#include "Profiler.h"

int main()
{
    std::srand(std::time(nullptr));
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "SFML works!");

    sf::Font font;
    font.loadFromFile("./Roboto-Bold.ttf");
    sf::Text helpText;
    helpText.setFont(font);
    helpText.setString("Spatial partitioning using k-d tree\nVisualization shows huge performance gains over naive iteration over a large number of points\n\nLight blue points are iterated over in the current search\nDark blue points are untouched\n\nF1 - toggle tree branches\nLEFT, RIGHT - cycle tree level\n\nF2 - toggle spatial partition rectangles\nUP, DOWN - cycle partition level\n\nHold Space - find nearest point\n\nHold RMB - Points in proximity\nMouse wheel up/down - increase/decrease proximity radius");
    helpText.setCharacterSize(12);
    helpText.setFillColor(sf::Color::White);

    float maxX = 1200.0f;
    float maxY = 1000.0f;

    int numberOfPoints{ 10000 };
    std::vector<sf::Vector2f> points;
    std::vector<Point> pointsVector;

    float inputCooldown{ 0.05f };
    float inputCooldownAccumulator{ 0.0f };

    const float maxRadius = 400.0f;
    const float minRadius = 150.0f;
    sf::Vector2f center(maxX / 2, maxY / 2);
    const float pi = 3.14159f;

    for (int i = 0; i < numberOfPoints; i++)
    {
        //sf::Vector2f newPoint(rand() % (int)maxX, rand() % (int)maxY);
        float angle = (rand() % 360) * pi / 180;
        float radius = minRadius + rand() % (int)(maxRadius - minRadius);
        sf::Vector2f newPoint(center + sf::Vector2f(radius * cos(angle), radius * sin(angle)));
        points.push_back(newPoint);
    }
    for (int i = 0; i < points.size(); i++)
    {
        pointsVector.push_back(Point(points[i]));
    }

    KDTree kdt;
    {
        {
            Profiler setup("Setup starting", "Create balanced tree: ");
            kdt.tree_ = kdt.createBalancedTreeFromVector(points);
        }
        {
            Profiler setup("", "Tree visualization: ");
            kdt.createInternalProgressiveVisualizationFromTree(kdt.tree_);
        }
        {
            Profiler setup("", "Box visualization: ");
            kdt.createBoxVisualizationFromTree(kdt.tree_, { 0,0 }, { maxX, maxY });
        }
    }

    Node testPoint;
    Node closestPoint;
    sf::VertexArray closestLine;
    closestLine.setPrimitiveType(sf::PrimitiveType::Lines);
    bool closestPointEnabled{ false };
    bool pointsInRangeEnabled{ false };

    float dt;
    sf::Clock clock;

    float mouseEntityRadius{ 100.0f };
    float mouseEntityRadiusRate{ 10.0f };
    sf::CircleShape mouseEntityShape(100, 100);
    mouseEntityShape.setFillColor(sf::Color::Transparent);
    mouseEntityShape.setOutlineColor(sf::Color::Cyan);
    mouseEntityShape.setOutlineThickness(1.0f);
    mouseEntityShape.setPosition({ 100, 100 });
    mouseEntityShape.setOrigin({ mouseEntityRadius, mouseEntityRadius });

    while (window.isOpen())
    {
        bool mouseWheelMoved{ false };
        bool mouseButtonRightPressed{ false };
        int mouseWheelDelta{ 0 };
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                mouseWheelMoved = true;
                mouseWheelDelta = event.mouseWheelScroll.delta;
                //std::cout << "Mouse wheel delta: "<< mouseWheelDelta << std::endl;
                inputCooldownAccumulator += inputCooldown;
                closestPointEnabled = false;
                pointsInRangeEnabled = true;
            }
        }



        //system("CLS");
        dt = clock.restart().asSeconds();
        kdt.update(dt);

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePos);

        inputCooldownAccumulator = std::fmax(0.0f, inputCooldownAccumulator - dt);
        if (mouseWheelMoved)
        {
            mouseEntityRadius = std::fmax(0, mouseEntityRadius + mouseWheelDelta * mouseEntityRadiusRate);
            mouseEntityShape.setRadius(mouseEntityRadius);
            mouseEntityShape.setOrigin({ mouseEntityShape.getRadius(), mouseEntityShape.getRadius() });
        }
        if (inputCooldownAccumulator == 0.0f)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                closestPointEnabled = true;
                pointsInRangeEnabled = false;
                inputCooldownAccumulator += inputCooldown;
                testPoint.setPoint(Vector2fToVector(mousePosWorld), sf::Color::Green);
                sf::Vector2f closestPointV2F = kdt.closestPoint(kdt.tree_, VectorToVector2f(testPoint.point_));
                closestPoint.setPoint(Vector2fToVector(closestPointV2F), sf::Color::Red);
                closestLine.clear();
                closestLine.append({ closestPoint.pointVisualization_.circle_.getPosition(), sf::Color::Red });
                closestLine.append({ testPoint.pointVisualization_.circle_.getPosition(), sf::Color::Green });
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
            {
                mouseEntityShape.setPosition(mousePosWorld);
                mouseButtonRightPressed = true;
                closestPointEnabled = false;
                pointsInRangeEnabled = true;
            }
        }

        if (mouseButtonRightPressed || mouseWheelMoved)
        {
            kdt.findPointsInRange(kdt.tree_, mouseEntityShape.getPosition(), mouseEntityShape.getRadius());
        }

        // draw everything
        window.clear();
        window.draw(kdt);
        if (closestPointEnabled)
        {
            window.draw(testPoint);
            window.draw(closestPoint);
            window.draw(closestLine);
        }
        if (pointsInRangeEnabled)
        {
            window.draw(mouseEntityShape);
        }
        window.draw(helpText);
        window.display();
    }

    return 0;
}