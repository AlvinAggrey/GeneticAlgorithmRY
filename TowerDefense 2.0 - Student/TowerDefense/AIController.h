#pragma once

#include "Tower.h"
#include "GA.h"
#include "EpochFileHandler.h"

class GameController;
class GameBoard;
class Timer;
class GameState;

class AIController
{
public:
	AIController();
	~AIController();

	void setupBoard();
	int  recordScore();

	void setGameController(GameController* gameController) { m_gameController = gameController; }
	void setGameBoard(GameBoard* gameBoard) { m_gameBoard = gameBoard; }
	void setTimer(Timer* timer) { m_Timer = timer; }
	void setGameState(GameState* gameState) { m_gameState = gameState; }
	void update();
	bool addTower(TowerType type, int gridx, int gridy);
	void gameOver();
	
	void MakeMove();
	void RenderNextTowerPos(int range, sf::Font orderFont, sf::RectangleShape highlight, sf::RenderWindow* window);
	void RenderGenerationLabels(int fontSize, sf::Font font,sf::RenderWindow* window);
	void RenderText(int fontSize, sf::Font font, std::string contents, float x, float y, sf::RenderWindow* window);
private:
	GameController* m_gameController;
	GameBoard*		m_gameBoard;
	Timer*			m_Timer;
	GameState*		m_gameState;

	int CheckCurrency();
	void ResetGeneStep();

	int gen_Num = 2;
	GA ga;
	//bool tested_chrom;
	//bool tested_gen;
	int curChromIndex;
	int m_geneStep;
	bool done;

	Gene m_nextTower;
	EpochFileHandler fileHandler;

	void MainMenu();
	int FindStringEnd(std::ifstream* ifs, std::string str);
	void SaveSettings(int genNum, int popNum, int childAmount,
		int selectionMethod, int crossoverMethod, float mutationChance,
		float selectNum, float matingPoolSize, float tournySize, std::vector<int> bandDists);
	void LoadSettings();
	void SettingsPrompt();
	bool LoadEpochPrompt();

	
	std::string m_dataTags[10] =
	{
		"generation number:",
		"population number:",
		"children number:",
		"selection method:",
		"crossover method:",
		"mutation chance:",
		"selection number:",
		"mating pool number:",
		"tournament size number:",
		"band distribution:"
	};
};

