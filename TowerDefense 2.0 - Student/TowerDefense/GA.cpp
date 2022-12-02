#include <iostream>
#include "GA.h"


Generation& GA::CurGen()
{
	return m_gens[m_currentGen];
}

void GA::init()
{
	Generation genenration;
	unsigned int seed;
	for (int i = 0; i < m_genSize; i++)
	{
		seed = rd();
		std::mt19937 gen(seed);
		Chromosome chrom = Chromosome(gen());
		genenration.push_back(chrom);
	}
	m_gens.push_back(genenration);
}

void GA::nextGen()
{
	Generation nextGen =  rouletteSelection();
	m_gens.push_back(nextGen);
	m_currentGen++;
	PrintGen(m_currentGen);
	//onepointCrossOver();
	//mutateGene();
}
void GA::rankSelection()
{
	//int parentNum = m_genSize * 0.2; //about 1/5 of population pass genes
	// idex numbers
	
	std::vector<Chromosome> bestChromRanked(m_genSize);
	Generation unrankedchroms = m_gens[m_currentGen];

	//rank by fitness
	for (int i = 0; i < m_genSize; i++)
	{
		int fittest = 0;
		//find fittest within unranked
		for (int i = (int) unrankedchroms.size() - 1; i >= 0; i--)
		{
			if (unrankedchroms[fittest].m_fitness < unrankedchroms[i].m_fitness)
			{
				fittest = i;
			}
		}
		
		bestChromRanked.push_back(unrankedchroms[fittest]);
		unrankedchroms.erase(unrankedchroms.begin() + fittest);
	}

	
}

Generation GA::rouletteSelection()
{
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0, 1);
	//pick mates
	Generation nextGen;
	for (int i = 0; i < m_genSize; i++)
	{
		Chromosome parent1 = rouletteSpin();
		Chromosome parent2 = rouletteSpin();
		nextGen.push_back(onepointCrossOver(parent1, parent2));
	}
	return nextGen;
}

Chromosome GA::rouletteSpin()
{
	//all fitness values added together
	float totalFitness = 0;
	for (int i = 0; i < m_genSize; i++)
	{
		totalFitness += m_gens[m_currentGen][i].m_fitness;
	}

	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0, totalFitness);

	bool done = false;
	float randomNum;
	int i = 0;

	//go as long as one hasn't been picked
	do
	{
		if (i >= m_genSize)
			i = 0;

		randomNum = dist(gen);
		if (randomNum <= m_gens[m_currentGen][i].m_fitness)
		{
			done = true;
			return m_gens[m_currentGen][i];
		}
		i++;
	} while (done == false);
}

Chromosome GA::onepointCrossOver(Chromosome parent1, Chromosome parent2)
{
	
	Chromosome child;

	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> coin(0, 2);

	std::uniform_real_distribution<float> dist(0, parent1.ChromSize());

	int coinFlip = coin(gen);
	int randomPoint = dist(gen);

	//copy over second portion to the child
	if (coinFlip == 1)
	{
		child = parent1;
		std::copy(parent2.Genes().begin() + randomPoint, parent2.Genes().end(), child.Genes().begin() + randomPoint);
	}
	else if (coinFlip == 2)
	{
		child = parent2;
		std::copy(parent1.Genes().begin() + randomPoint, parent1.Genes().end(), child.Genes().begin() + randomPoint);
	}


	return child;
}

void GA::PrintGen(int index)
{
	std::cout << "Generation: " << index << std::endl;
	//std::cout << index << ": ";

	Generation generation = m_gens[index];

	for (int chromIndex = 0; chromIndex < generation.size(); chromIndex++)
	{
		std::cout << chromIndex << ": ";
		for (int n = 0; n < generation[chromIndex].ChromSize(); n++)
		{
			int x = generation[chromIndex].Genes()[n].m_position[0];
			int y = generation[chromIndex].Genes()[n].m_position[1];
			int towerType = generation[chromIndex].Genes()[n].m_towerType;
			std::cout << "|p-(" << x << "," << y << ")-t-" << towerType << "| ";
		}
		std::cout << "  " << std::endl;
	}
	std::cout << std::endl;
}

void GA::PrintAllGens()
{
	std::cout << "Generations: " << m_currentGen << std::endl;
	for (int i = 0; i < m_gens.size(); i++)
	{
		std::cout << i << ": ";
		PrintGen(i);
		std::cout << std::endl;
	}
}


GA::GA()
{
	m_currentGen = 0;
	m_genSize = 2;
	init();
}

GA::GA(const GA& var)
{
	std::vector<Generation> m_gens = var.m_gens;
	m_currentGen = var.m_currentGen;
	m_genSize = var.m_genSize;
}

GA& GA::operator=(const GA& var)
{
	std::vector<Generation> m_gens = var.m_gens;
	m_currentGen = var.m_currentGen;
	m_genSize = var.m_genSize;

	return *this;
}

GA::GA(int genSize)
{
	m_genSize = genSize;

	//initial generation is 0
	m_currentGen = 0;
	done = false;
	//m_gens = {};
	init();
	//PrintGen();
}