#include "Game.h"

//Private functions
void Game::InitVariables()
{
	this->window = nullptr;
	this->points = 0;
	this->enemySpawnTimerMax = 10.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 3;
	this->mouseHeld = false;
	this->health = 10;
	this->endGame = false;
}

void Game::InitWindow()
{
	this->videoMode.width = 800;
	this->videoMode.height = 600;
	this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void Game::initFonts()
{
	if (this->font.loadFromFile("Fonts/Roboto-Regular.ttf")) {
		std::cout << "ERROR::GAME::INITFONTS::Failed to load font!" << "\n";
	}

}

void Game::InitEnemies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(sf::Vector2f(50.f, 50.f));
	this->enemy.setFillColor(sf::Color(0, 255, 255, 255));
	this->enemy.setOutlineColor(sf::Color(0, 255, 0, 255));
	this->enemy.setOutlineThickness(1.f);



}

void Game::InitText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(20);
	this->uiText.setFillColor(sf::Color(255, 255, 255, 255));

	this->uiText.setString("Score:0");
}

//Constructors / Destructors
Game::Game()
{
	this->InitVariables();
	this->initFonts();
	this->InitText();
	this->InitWindow();
	this->InitEnemies();
}

Game::~Game()
{
	delete this->window;
}

//Accessors
const bool Game::Running() const
{
	return this->window->isOpen();
}

const bool Game::GetEndGame() const
{
	return this->endGame;
}


//public Functions

void Game::SpawnEnemies()
{
	/*
		@return void

		Spawns enemies and sets their colors and positions.
			-Sets a random position
			-Sets a random color
			-Adds enemy to the vector
			-Move the enemies downwards
			-Remove the enemies at the edge of the screen
	*/
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0
	);
	this->enemy.setFillColor(sf::Color(0, 255, 0));
	this->enemies.push_back(this->enemy);

	//Remove enemies


}

void Game::PollEvents()
{
	//Event polling
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape) {
				this->window->close();
			}
			break;
		}
	}
}

void Game::UpdateMousePositions()
{
	/*
		@ return void
		Update the mouse positions:
		 - Mouse position relative to wndow (vector2i)
	*/
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}



void Game::Update()
{
	this->PollEvents();

	if (this->endGame == false) {
		this->UpdateMousePositions();
		this->UpdateEnemies();
	}
	this->UpdateText();

	//End game condition
	if (this->health <= 0) {
		this->endGame = true;
	}

}

void Game::UpdateText()
{
	std::stringstream ss;

	if (!this->endGame) {
		ss << "Score: " << this->points << "\n health: " << this->health;
	}
	else {
		ss << "Game Over " << "\n" << "Score: " << this->points;
		this->uiText.setFillColor(sf::Color(255, 0, 0, 255));
	}
	this->uiText.setString(ss.str());
}

void Game::RenderEnemies(sf::RenderTarget& target)
{
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}

void Game::UpdateEnemies()
{
	/*
		@return void
		Updates the enmy spawn timer and spawn enemies
		when the total amount of enemies is smaller than the maximum.
	*/
	//updating the timer for enemy spawning

	if (this->enemies.size() < this->maxEnemies) {

		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			//spawn the enemy and reset the timer
			this->SpawnEnemies();
			this->enemySpawnTimer = 0.f;
		}
		else {
			this->enemySpawnTimer += 1.f;
		}
	}
	//Moving and upadting enemies
	for (int i = 0; i < this->enemies.size();i++)
	{
		this->enemies[i].move(0.f, 5.f);
		bool deleted = false;

		//check if enemy passed the bottom of screen
		if (this->enemies[i].getPosition().y > this->window->getSize().y) {
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
			std::cout << "You have total health of " << health << "\n";
		}
	}

	//check if clicked upon
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		bool deleted = false;
		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;
			for (int i = 0; i < this->enemies.size() && deleted == false;i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
					//gain points
					this->points += 1;
					IncreaseDifficulty();
					std::cout << "You have total points of " << points << "\n";
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	else
	{
		this->mouseHeld = false;
	}


}



void Game::RenderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}


void Game::Render()
{
	/*
		@return void
		Render the game objects :
		-clear frames
		-render objects
		-display frame in window
	*/

	this->window->clear();

	this->RenderEnemies(*this->window);

	this->RenderText(*this->window);

	this->window->display();
}

void Game::IncreaseDifficulty()
{
	if (this->points!= 0 && this->points % 50 == 0) {
		this->maxEnemies += 1;
	}
}
