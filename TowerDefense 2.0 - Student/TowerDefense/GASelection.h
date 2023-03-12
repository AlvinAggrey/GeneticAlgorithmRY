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
	Ranked,
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

	std::vector<individual> RankChroms(std::vector<individual> indivs);
	std::vector<individual> Ranked(std::vector<individual> gen, int selectNum, int matingPoolSize);
	std::vector<individual> LinearRanked(std::vector<individual> gen, std::initializer_list<int> bandDists);
	std::vector<individual> Elitist(std::vector<individual> gen, std::vector<individual> indivs, std::initializer_list<int> bandDists);
	std::vector<individual> Tournament(std::vector<individual> gen, int matingPoolSize, int tournySize);
	std::vector<individual> StochasticSampling(std::vector<individual> gen, int matingPoolSize);
	std::vector<individual> Roulette(std::vector<individual> gen, int matingPoolSize);
private:
	//Chromosome RouletteSpin(std::vector<individual> population);
};