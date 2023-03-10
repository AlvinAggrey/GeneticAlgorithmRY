#pragma once
#include "Chromosome.h"
#include "GACrossover.h"
#include <vector>
//chromsome and completed testing?
typedef std::pair<Chromosome, bool> individual;

enum class SelectionMethod
{
	None,
	Rank,
	Roulette
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

private:
	Chromosome RouletteSpin(std::vector<individual> population);
};