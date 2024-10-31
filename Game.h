#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

/*
* class that acts as the game engine.
* Wrapper class.
*/

class Game
{

private:
	//variables 
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Game objects
	sf::RectangleShape enemy;
	std::vector<sf::RectangleShape> enemies;

	//Resource
	sf::Font font;

	//text
	sf::Text uiText;

	//Game logic
	bool endGame;
	unsigned points;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;

	//private functions
	void InitVariables();
	void InitWindow();
	void initFonts();
	void InitEnemies();
	void InitText();
public:
	//constructors / Destructors
	Game();

	~Game();

	//Accessors
	const bool Running() const;
	const bool GetEndGame() const;

	//Functions
	void SpawnEnemies();
	void PollEvents();
	void UpdateMousePositions();
	void UpdateEnemies();
	void Update();
	void UpdateText();
	void RenderEnemies(sf::RenderTarget& target);
	void RenderText(sf::RenderTarget& target);
	void Render();
	void IncreaseDifficulty();
};

