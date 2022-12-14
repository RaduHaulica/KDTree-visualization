#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

class Profiler
{
	std::string m_ctorMessage;
	std::string m_dtorMessage;
	sf::Clock timer;
public:
	Profiler(std::string ctor, std::string dtor) :
		m_ctorMessage{ ctor },
		m_dtorMessage{ dtor }
	{
		if (ctor != "")
		{
			std::cout << ctor << std::endl;
		}
		timer.restart();
	}

	~Profiler()
	{
		if (m_dtorMessage != "")
		{
			std::cout << m_dtorMessage << timer.getElapsedTime().asSeconds() << std::endl;
		}
	}
};