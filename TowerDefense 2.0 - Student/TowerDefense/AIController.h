#pragma once

#include "Tower.h"
#include "GA.h"

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
private:
	GameController* m_gameController;
	GameBoard*		m_gameBoard;
	Timer*			m_Timer;
	GameState*		m_gameState;

	int CheckCurrency();

	int gen_Num = 2;
	GA ga = GA(10);
	//bool tested_chrom;
	//bool tested_gen;
	int curChromIndex;
	int curGeneIndex;
	bool done;
};

