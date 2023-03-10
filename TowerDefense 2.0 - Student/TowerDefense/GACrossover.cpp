#include "GACrossover.h"

GACrossover::GACrossover()
{
	m_gen.seed(m_rd());
}

GACrossover::GACrossover(const GACrossover& other)
{
	m_gen = other.m_gen;
}


GACrossover GACrossover::operator=(const GACrossover& other)
{
	m_gen = other.m_gen;
	return *this;
}



Chromosome GACrossover::OnepointCrossover(Chromosome parent1, Chromosome parent2)
{

	Chromosome child;

	std::uniform_int_distribution<int> coin(0, 1);

	std::uniform_int_distribution<int> dist(0, parent1.ChromSize()-1);

	int coinFlip = coin(m_gen);
	int randomPoint = dist(m_gen);

	//copy over second portion to the child
	if (coinFlip == 0)
	{
		child = parent1;
		std::copy(parent2.Genes().begin() + randomPoint, parent2.Genes().end(), child.Genes().begin() + randomPoint);
	}
	else if (coinFlip == 1)
	{
		child = parent2;
		std::copy(parent1.Genes().begin() + randomPoint, parent1.Genes().end(), child.Genes().begin() + randomPoint);
	}


	return child;
}
