#pragma once
#include "Chromosome.h"
#include "GACrossover.h"
#include <vector>
#include <iostream>
//chromsome and completed testing?
typedef std::pair<Chromosome, bool> individual;

enum class SelectionMethod
{
	None,
	Rank,
	Roulette,
	LinearRanked,
	Elitist,
	Tournament,
	Stochastic,
};

class GASelection
{
public:
	GASelection();

private:
	std::random_device m_rd;

	std::mt19937 m_gen;
	
public:
	const GASelection(const GASelection& other);
	const GASelection& operator=(const GASelection& other);

	void RankSelection(std::vector<individual> population, int matingPoolSize);
	std::vector<individual> RouletteSelection(std::vector<individual> population, int matingPoolSize);

	std::vector<Chromosome> RankChroms(std::vector<Chromosome>* chroms);
	std::vector<Chromosome> RankedSelection(std::vector<Chromosome>* gen, int selectNum, int parentNum);
	std::vector<Chromosome> LinearRankedSelection(std::vector<Chromosome>* gen, unsigned int seed, std::initializer_list<int> bandDists);
	std::vector<Chromosome> ElitistSelection(std::vector<Chromosome>* gen, unsigned int seed, std::vector<Chromosome> indivs, std::initializer_list<int> bandDists);
	std::vector<Chromosome> TournamentSelection(std::vector<Chromosome>* gen, unsigned int seed, int matingPoolSize, int tournySize);

	std::vector<Chromosome> StochasticSamplingSelection(std::vector<Chromosome>* gen, unsigned int seed, int parentNum);
	std::vector<Chromosome> RouletteSelection(std::vector<Chromosome>* gen, unsigned int seed, int parentNum);
private:
	Chromosome RouletteSpin(std::vector<individual> population);
};