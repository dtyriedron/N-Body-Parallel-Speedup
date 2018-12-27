#include "stdafx.h"

#include <SFML/Graphics.hpp>

#include "Simulation.h"
#include "Body.h"

#include <iostream>
int frames = 0;
static int totalTime = 0;

void updateFPS(sf::Time &elapsed, sf::Text &text, sf::RenderWindow &window);

int _tmain(int argc, _TCHAR* argv[])
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 9;

    sf::RenderWindow window(sf::VideoMode(1200, 800), "SFML works!", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	sf::Font font;
	if(!font.loadFromFile("sansation.ttf"))
	{
		std::cout << "Failed to load text format.";
	}

	sf::Text text;
	text.setFont(font);
	text.setString("");
	text.setColor(sf::Color::White);
	text.setPosition(0,0);

	Simulation sim;
	sim.populate(2400, window.getSize().x*0.25, window.getSize().y*0.25, window.getSize().x*0.75, window.getSize().y*0.75);

	sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
			if (event.type == sf::Event::Closed)
				window.close();
        }

		sf::Time elapsed = clock.restart();
		window.clear();

		totalTime += sim.runOneIteration(window, elapsed, 0.5);

		if (frames > 249)
		{
			std::cout << totalTime / 1000 <<","<< std::endl;
			window.close();
		}
		updateFPS(elapsed, text, window);
        window.display();
    }
    return 0;
}

void updateFPS(sf::Time &elapsed, sf::Text &text, sf::RenderWindow &window)
{
	static int framesUpdated = 1;
	static int totalframeTime = 0;
	std::string s;
	
	if(framesUpdated == 5)
	{
		if(totalframeTime != 0)
		{
			int average = totalframeTime / 5;
			int fps = 1000 / average;
			s = std::to_string(fps);
			text.setString(s);
			frames++;
			//std::cout << frames << "," << s << std::endl;
		}
		framesUpdated = 0;
		totalframeTime = 0;
	}
	else
	{
		framesUpdated++;
		totalframeTime += elapsed.asMilliseconds();
	}
	window.draw(text);
}