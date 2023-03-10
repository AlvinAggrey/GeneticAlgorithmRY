#include <iostream>
#include "GA.h"

GA::GA()
{
    m_genAmount = 2;
    m_popSize = 2;
    m_childAmount = 1;
    m_curGenIndex = 0;
    m_curIndivIndex = 0;
    m_done = false;
    m_gen.seed(m_rd());

    m_genMaker = GAGenMaker(SelectionMethod::Roulette, CrossoverMethod::Onepoint,m_popSize, m_childAmount);
}

GA::GA(int gens, int popSize, int childAmount, SelectionMethod selectionMethod, CrossoverMethod crossoverMethod)
{
    m_genAmount = gens;
    m_popSize = popSize; 
    m_childAmount = childAmount;
    m_curGenIndex = 0;
    m_curIndivIndex = 0;
    m_done = false;
    m_gen.seed(m_rd());

    m_genMaker = GAGenMaker(selectionMethod, crossoverMethod, m_popSize, m_childAmount);
}

GA::GA(const GA& other)
{
    m_genAmount = other.m_genAmount;
    m_popSize = other.m_popSize;
    m_childAmount = other.m_childAmount;
    m_curGenIndex = other.m_curGenIndex;
    m_curIndivIndex = other.m_curIndivIndex;
    m_done = other.m_done;
    m_gen = other.m_gen;

    m_genMaker = other.m_genMaker;
}

GA GA::operator=(const GA& other)
{
    m_genAmount = other.m_genAmount;
    m_popSize = other.m_popSize;
    m_childAmount = other.m_childAmount;
    m_curGenIndex = other.m_curGenIndex;
    m_curIndivIndex = other.m_curIndivIndex;
    m_done = other.m_done;
    m_gen = other.m_gen;

    m_genMaker = other.m_genMaker;
    return *this;
}

void GA::Init()
{
	m_generations.push_back(m_genMaker.CreateGen());
}

void GA::NextIndiv()
{
    m_curIndivIndex++;
    if (m_curIndivIndex >= m_popSize)
    {
        m_curIndivIndex = m_popSize - 1;
    }
}

void GA::NextGen()
{
    m_nextPopulation = m_genMaker.NextGen(m_generations[m_curGenIndex]);
    m_generations.push_back(m_nextPopulation);
    m_nextPopulation.clear();
    m_curIndivIndex = 0;
    m_curGenIndex++;

    if (m_curGenIndex >= m_genAmount)
    {
        m_curGenIndex = m_genAmount - 1;
    }
    
}

void GA::SetCrossoverMethod(CrossoverMethod method)
{
    m_genMaker.SetCrossoverMethod(method);
}

void GA::SetSelectionMethod(SelectionMethod method)
{
    m_genMaker.SetSelectionMethod(method);
}

bool GA::CheckTestAllPop()
{
    for (individual indiv : m_generations[m_curGenIndex])
    {
        if (indiv.second == false)
        {
            return false;
        }
    }
    return true;
}

bool GA::CheckIsDone()
{
    if (m_curGenIndex >= m_genAmount - 1 && CheckTestAllPop() == true)
    {
        return true;
    }
    return false;
}

individual& GA::CurIndiv()
{
    return m_generations[m_curGenIndex][m_curIndivIndex];
}

std::vector<individual>& GA::CurGen()
{
    return m_generations[m_curGenIndex];
}

int GA::CurIndivIndex()
{
    return m_curIndivIndex;
}

int GA::CurGenIndex()
{
    return m_curGenIndex;
}

void GA::PrintGen(int index)
{
    std::cout << "Generation: " << index << std::endl;
    //std::cout << index << ": ";

    std::vector<individual> generation = m_generations[index];

    for (int chromIndex = 0; chromIndex < generation.size(); chromIndex++)
    {
        std::cout << chromIndex << ": ";
        for (int n = 0; n < generation[chromIndex].first.ChromSize(); n++)
        {
            int x = generation[chromIndex].first.Genes()[n].m_position[0];
            int y = generation[chromIndex].first.Genes()[n].m_position[1];
            int towerType = generation[chromIndex].first.Genes()[n].m_towerType;
            std::cout << "|p-(" << x << "," << y << ")-t-" << towerType << "| ";
        }
        std::cout << "  " << std::endl;
    }
    std::cout << std::endl;
}

void GA::PrintAllGens()
{
    std::cout << "Generations: " << m_curGenIndex << std::endl;
    for (int i = 0; i < m_generations.size(); i++)
    {
        std::cout << i << ": ";
        PrintGen(i);
        std::cout << std::endl;
    }
}
