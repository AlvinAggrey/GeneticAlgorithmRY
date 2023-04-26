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

//std::random_device rd;
Chromosome GACrossover::OnePoint(Chromosome parent1, Chromosome parent2)
{
	Chromosome child = parent1;
	child.m_fitness = 1;

	std::mt19937 gen(1);
	std::uniform_int_distribution<int> dist(0, parent1.ChromSize());

	int randomPoint = dist(gen);
	//copy over second portion to the child
	std::copy(parent2.Genes().begin() + randomPoint, parent2.Genes().end(), child.Genes().begin() + randomPoint);

	return child;
}
//std::random_device rd;
Chromosome GACrossover::TwoPoint(Chromosome parent1, Chromosome parent2)
{
	Chromosome child = parent1;
	child.m_fitness = 1;

	std::mt19937 gen(1);
	std::uniform_int_distribution<int> distPoint1(0, parent1.ChromSize());
	int point1 = distPoint1(gen);

	std::uniform_int_distribution<int> distPoint2(point1, parent1.ChromSize());
	int point2 = distPoint2(gen);
	//copy over second portion to the child
	std::copy(parent2.Genes().begin() + point1, parent2.Genes().begin() + point2, child.Genes().begin() + point1);

	return child;
}
