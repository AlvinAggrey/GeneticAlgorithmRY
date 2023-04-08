#pragma once
#include "GASelection.h"
#include "GACrossover.h"
#include "Chromosome.h"
#include <vector>
#include <random>

typedef std::pair<Chromosome, bool> individual;

class GAGenMaker
{
public:
	GAGenMaker();
	GAGenMaker(SelectionMethod selectionMethod, CrossoverMethod crossoverMethod, int popSize, int childAmount);

private:
	std::random_device m_rd;
	std::mt19937 m_gen;
	int m_popSize;
	int m_childAmount;
	GASelection m_selector;
	GACrossover m_geneCrossover;

	int m_chromSize = 6;
	int m_selectNum;
	int m_matingPoolSize;
	int m_tournySize;
	std::vector<int> m_bandDists;

	SelectionMethod m_selectionMethod;
	CrossoverMethod m_crossoverMethod;

	std::vector<individual> Selection(std::vector<individual> population);
	std::vector<individual> Crossover(std::vector<individual> matingPool, int childAmount);
public:
	GAGenMaker(const GAGenMaker& other);
	GAGenMaker operator=(const GAGenMaker& other);

	std::vector<individual> CreateGen();
	//returns next generation
	std::vector<individual> NextGen(std::vector<individual> population);

	//void SetSelectionMethod(SelectionMethod method);
	//void SetSelectionMethod(SelectionMethod method);

	//void SetCrossoverMethod(CrossoverMethod method);

	void UseRanked(int selectNum, int matingPoolSize);
	void UseRoulette(int matingPoolSize);
	void UseLinearRanked(std::initializer_list<int> bandDists);
	void UseElitist(std::initializer_list<int> bandDists);
	void UseTournament(int matingPoolSize);
	void UseStochastic(int matingPoolSize, int tournySize);

	void UseOnePoint();

};

