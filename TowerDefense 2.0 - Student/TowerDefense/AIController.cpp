#include "AIController.h"
#include "Timer.h"
#include "GameState.h"
#include <iostream>
#include <Windows.h>
#include <sstream>

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
		m_nextTower = gene;
		if (addTower((TowerType)gene.m_towerType, gene.m_position[0],gene.m_position[1]))
		{
			curGeneIndex++;
		}
	}
}
void AIController::RenderNextTowerPos(int range, sf::RectangleShape highlight, sf::RenderWindow* window)
{
	if (ga.done)
	{
		return;
	}

	for (Gene gene : ga.CurGen()[0].Genes())
	{
		int gridX = gene.m_position[0];
		int gridY = gene.m_position[1];
		highlight.setSize(sf::Vector2f((float)range * 60, (float)range * 60));
		highlight.setPosition((float)gridX * 60, (float)gridY * 60);
		window->draw(highlight);
	}
}

void AIController::RenderGenerationLabels(int fontSize, sf::Font font, sf::RenderWindow* window)
{
	std::stringstream genInfo;
	std::stringstream chromInfo;
	genInfo << "Generation: " << ga.m_currentGen; ///<< " / " << ga.m_gens.size() - 1;
	chromInfo << "Chromosome: " << curChromIndex + 1 << " / " << ga.CurGen().size();
	RenderText(fontSize, font, genInfo.str(), 140, 300, window);
	RenderText(fontSize, font, chromInfo.str(), 140, 400, window);
}

void AIController::RenderText(int fontSize, sf::Font font, std::string contents, float x, float y, sf::RenderWindow* window)
{
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(fontSize);
	text.setPosition(sf::Vector2f(x, y));
	text.setString(contents);
	window->draw(text);
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