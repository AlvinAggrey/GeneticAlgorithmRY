#include "AIController.h"
#include "Timer.h"
#include "GameState.h"
#include <iostream>
#include <Windows.h>

#include "GameController.h"

using namespace std;

AIController::AIController()
{
	m_gameController = nullptr;
	m_gameBoard = nullptr;
	m_Timer = nullptr;
	m_gameState = nullptr;
	ga = GA(10);
}

AIController::~AIController()
{

}

void AIController::gameOver()
{
	int waveReached = m_gameState->getCurrentWave();
	int healthLeft = m_gameState->getHealth();
	int avgkillCount = m_gameState->getMonsterEliminated()/ waveReached;
	//int tamsLeft = ;
	int fitness = healthLeft + waveReached + avgkillCount;

	ga.CurGen()[curChromIndex].m_fitness = fitness;
	if (ga.done)
	{
		done = true;
		return;
	}

	curChromIndex++;
	if (ga.m_currentGen <= gen_Num)
	{
		done = true;
	}
	if (curChromIndex >= ga.m_genSize)
	{
		curGeneIndex = 0;
		curChromIndex = 0;
		ga.nextGen();
	}
	//tested_chrom = true;
}

void AIController::update()
{
	if (m_Timer == nullptr)
		return;

	// HINT
	// a second has elapsed - your GA manager (GA Code) may decide to do something at this time...
	static double elapsedSeconds = 0;
	double seconds = floor(m_Timer->elapsedSeconds());
	if (seconds > elapsedSeconds)
	{
		elapsedSeconds = seconds;

	}
	if (!done)
	{
		MakeMove();
	}
	//GAManager::Instance()->Update(m_Timer->elapsedSeconds());

	// this might be useful? Monsters killed
	static int monstersKilled = 0;

	if (m_gameState->getMonsterEliminated() > monstersKilled)
	{
		monstersKilled = m_gameState->getMonsterEliminated();
	}

	recordScore();
}

bool AIController::addTower(TowerType type, int gridx, int gridy)
{
	// grid position can be from 0,0 to 25,17
	/*
	enum class TowerType {
	empty, slammer, swinger, thrower };
	*/

	bool towerAdded = m_gameBoard->addTower(type, gridx, gridy);

	// NOTE towerAdded might be false if the tower can't be placed in that position, is there isn't enough funds

	return towerAdded;
}

void AIController::MakeMove()
{
	if (curGeneIndex < ga.CurGen()[curChromIndex].ChromSize())
	{
		Chromosome chrom  = ga.CurGen()[curChromIndex];
		Gene gene = chrom.Genes()[curGeneIndex];
		if (addTower((TowerType)gene.m_towerType, gene.m_position[0],gene.m_position[1]))
		{
			curGeneIndex++;
		}
	}
}


void AIController::setupBoard()
{
	m_Timer->start();
}

int AIController::recordScore()
{
	int currentWave = m_gameState->getCurrentWave();
	int killCount = m_gameState->getMonsterEliminated();
	currentWave *= 10; // living longer is good
	int score = currentWave + killCount;

	static int iteration = 0;

	if (iteration == 0)
		cout << "iteration" << "," << "wave" << "," << "kills" << "," << "score" << endl;

	cout << iteration << "," << m_gameState->getCurrentWave() << "," << m_gameState->getMonsterEliminated() << "," << score << endl;
	iteration++;

	m_gameState->setScore(score);

	return score;
}