#include "GASelection.h"


GASelection::GASelection()
{
	m_gen.seed(m_rd());
}


GASelection::GASelection(const GASelection& other)
{
	m_gen = other.m_gen;
}

const GASelection& GASelection::operator=(const GASelection& other)
{
	m_gen = other.m_gen;

	return *this;
}

void GASelection::RankSelection( std::vector<individual> population, int matingPoolSize)
{
	//int parentNum = m_genSize * 0.2; //about 1/5 of population pass genes
// idex numbers
	int popSize = population.size();
	std::vector<individual> bestChromRanked(popSize);
	std::vector<individual> unrankedchroms = population;
	//rank by fitness
	for (int i = 0; i < popSize; i++)
	{
		int fittest = 0;
		//find fittest within unranked
		for (int i = (int)unrankedchroms.size() - 1; i >= 0; i--)
		{
			if (unrankedchroms[fittest].first.m_fitness < unrankedchroms[i].first.m_fitness)
			{
				fittest = i;
			}
		}

		bestChromRanked.push_back(unrankedchroms[fittest]);
		unrankedchroms.erase(unrankedchroms.begin() + fittest);
	}


}

Chromosome GASelection::RouletteSpin(std::vector<individual> population)
{
	int popSize = population.size();

	//all fitness values added together
	float totalFitness = 0;
	for (int i = 0; i < popSize; i++)
	{
		totalFitness += population[i].first.m_fitness;
	}

	std::uniform_real_distribution<float> dist(0, totalFitness);

	bool done = false;
	float randomNum;
	int i = 0;

	//go as long as one hasn't been picked
	do
	{
		if (i >= popSize)
			i = 0;

		randomNum = dist(m_gen);
		if (randomNum <= population[i].first.m_fitness)
		{
			done = true;
			return population[i].first;
		}
		i++;
	} while (done == false);
}

std::vector<individual> GASelection::RouletteSelection(std::vector<individual> population, int matingPoolSize)
{

	std::vector<individual> matingPool;
	for (int i = 0; i < matingPoolSize; i++)
	{
		matingPool.push_back(individual(RouletteSpin(population), false));
	}
	return matingPool;
}