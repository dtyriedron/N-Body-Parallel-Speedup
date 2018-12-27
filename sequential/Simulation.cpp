#include "stdafx.h"
#include "Simulation.h"
#include <omp.h>
#include <iostream>

int totalTime = 0;
//----------------------------------CONSTRUCTORS / DECONSTRUCTOR------------------------------------------------------

Simulation::Simulation(void) : tempPlanet(nullptr), tempPlanetMass(200), mouseHeldDown(false)
{
	//Will be added soon
	//if(!averageSpeed.is_open())
	//{
	//	
	//}
}


Simulation::~Simulation(void)
{
}

//---------------------------------------OPERATOR OVERLOADS-----------------------------------------------------------

//-------------------------------------PUBLIC MEMBER FUNCTIONS--------------------------------------------------------

void Simulation::eventHandler(sf::RenderWindow &window)
{
	// Mouse being held down -- Creates a new planet, but the direction and magnitude of its initial velocity has not yet been
	// input from the user.  On mouse release, the initial velocity is set and the planet is updated with the rest.

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left))		//Since there is no function for button released, we must create our own.
	{
		if(!mouseHeldDown)		//code that is executed when the button is first pressed
		{
			mouseHeldDown = true;
			auto position = sf::Mouse::getPosition(window);
			tempPlanet = std::make_shared<Body>(tempPlanetMass, position.x, position.y);
		}
		else    //code that is executed while the button is being pressed
		{
			if(tempPlanet != nullptr)
			{
				auto cursorPosition = sf::Mouse::getPosition(window);
				sf::Vertex line[] =
				{
					sf::Vertex(sf::Vector2f(tempPlanet->xPosition, tempPlanet->yPosition)),
					sf::Vertex(sf::Vector2f(cursorPosition.x, cursorPosition.y))
				};
				window.draw(line, 2, sf::Lines);
			}
		}
	}
	else
	{
		if(mouseHeldDown)		//code that is exected when the button is released
		{
			auto cursorPosition = sf::Mouse::getPosition(window);
			double distance = calculateDistance(tempPlanet->xPosition, tempPlanet->yPosition, cursorPosition.x, cursorPosition.y);
			
			double deltaX = tempPlanet->xPosition - cursorPosition.x;
			double deltaY = tempPlanet->yPosition - cursorPosition.y;

			tempPlanet->setVelocity(deltaX*500/(tempPlanet->mass), deltaY*300/(tempPlanet->mass));

			planetList.push_back(*tempPlanet);
			tempPlanet = nullptr;
			mouseHeldDown = false;
		}
	}

	//increases the mass of the new planet if the up arrow is pressed
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		tempPlanetMass += 25;
	}

	//decreases the mass of the new planet if the down arrow is pressed
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if(tempPlanetMass > 25)
			tempPlanetMass -= 25;
	}

}

int Simulation::runOneIteration(sf::RenderWindow &window, sf::Time &elapsed, double gravConst)
{
	eventHandler(window);
	collisionResolution();		//Each of these functions modifies the members of planetList in a specific way.
	sumForces(gravConst);
	update(elapsed);

	drawToWindow(window);

	return totalTime;
}

void Simulation::populate(int number, int xMin, int yMin, int xMax, int yMax)		//populates the planetList with randomly generated planets
{
	auto middleX = xMax / 1.5;
	auto middleY = yMax / 1.5;

	//place black hole in centre of planets
	planetList.emplace_back(150, middleX, middleY);

	for (int i = 0; i < number / 5; ++i)
	{
		//place planets around in a circle with varying masses
		planetList.emplace_back((i + 1) * 0.03, middleX + 400 * cos(i), middleY + 400 * sin(i));

		//place planets around in a circle with varying masses
		planetList.emplace_back((i + 1) * 0.03, middleX + 300 * cos(i), middleY + 300 * sin(i));

		//place planets in an inner circle with varying masses
		planetList.emplace_back((i + 1) * 0.03, middleX + 200 * cos(i), middleY + 200 * sin(i));

		//place planets in an inner circle with varying masses
		planetList.emplace_back((i + 1) * 0.03, middleX + 100 * cos(i), middleY + 100 * sin(i));

		//place planets in an inner circle with varying masses
		planetList.emplace_back((i + 1) * 0.03, middleX + 50 * cos(i), middleY + 50 * sin(i));
	}
}

void Simulation::addPlanet(double mass, double xPosition, double yPosition, double xVelocity, double yVelocity)
{
	planetList.emplace_back(mass, xPosition, yPosition);
}

//------------------------------------PRIVATE MEMBER FUNCTIONS--------------------------------------------------------

void Simulation::collisionResolution()
{
	
	bool collisionDetected = false;
	auto first = planetList.begin();
	while(first != planetList.end())
	{
		for(auto second = planetList.begin(); second != planetList.end(); second++)
		{
			if( (*first) != (*second) && first->hasCollided(*second))
			{
				planetList.push_back((*first) + (*second));
				planetList.erase(second);
				first = planetList.erase(first);			//erase the two collided planets.
				collisionDetected = true;
				break;
			}
		}
		if(collisionDetected)		//If there is a collision, then "first" automatically points to the next value, so there is no need to increment it.
			collisionDetected = false;
		else
			first++;
	}
	
}


//Adds up all forces from all other bodies.
void Simulation::sumForces(double gravConst)
{	
	sf::Clock clock;
	clock.restart(); 
	for(int i = 0; i < planetList.size(); ++i)
	{
		for(int j = 0; j < planetList.size(); ++j)
		{
			if(i != j)
				planetList[i].addForces(planetList[j], gravConst);
		}
	}
	totalTime += clock.restart().asMilliseconds();
}


//Updates position and velocity from the new acceleration value, and sets up the acceleration value for the next iteration
void Simulation::update(sf::Time &elapsed)
{
	for (int i=0; i<planetList.size();++i)
	{
		planetList[i].updateVelocity(elapsed);
		planetList[i].updatePosition(elapsed);
		planetList[i].zeroAccel();
	}	
}

void Simulation::drawToWindow(sf::RenderWindow &window)
{
	for (Body &c : planetList)
	{
		window.draw(c.circle);
	}

	if(tempPlanet != nullptr)
	{
		window.draw(tempPlanet->circle);
	}
}

double Simulation::calculateDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow( (x2 - x1), 2) + pow( (y2 - y1), 2));
}