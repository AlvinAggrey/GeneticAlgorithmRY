#pragma once
#include "Chromosome.h"
#include <vector>
#include <random>

typedef std::vector<Chromosome> Generation;
class GA
{
public:
	GA();
	GA(int genSize);
	//copy constructor
	GA(const GA& var);
	GA& operator=(const GA& var);

	bool done;

	Generation& CurGen();
	void init();
	void nextGen();

	std::vector<Generation> m_gens;
	int m_currentGen;
	int m_genSize;

	std::random_device rd;
	//fitness function

	//selection
	void rankSelection();
	Generation rouletteSelection();
	Chromosome rouletteSpin();
	//crossover
	Chromosome onepointCrossOver(Chromosome parent1, Chromosome parent2);
	//mutation
	void mutateGene();

	void PrintGen(int index);
	void PrintAllGens();
};

