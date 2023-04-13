#include "SelectionMethods.h"



std::vector<Chromosome> GASelection::RankChroms(std::vector<Chromosome>* chroms)
{
    std::vector<Chromosome> rankedList;
    std::vector<Chromosome> tempList = *chroms;

    //order chromosomes by fitness biggest to lowests
    for (int i = 0; i < (int)chroms->size(); i++)
    {
        Chromosome fittest;
        int fittestIndex = 0;
        for (int i = 0; i < tempList.size(); i++)
        {
            if (i == 0)
            {
                fittest = tempList[i];
                fittestIndex = i;
            }
            else if (fittest.m_fitness < tempList[i].m_fitness)
            {
                fittest = tempList[i];
                fittestIndex = i;
            }

        }
        rankedList.push_back(fittest);
        tempList.erase(tempList.begin() + fittestIndex);
    }
    return rankedList;
}
//ranks generation, selects n from ranked list from the fittest, returns copies of selected individuals
std::vector<Chromosome> GASelection::RankedSelection(std::vector<Chromosome>* gen, int selectNum, int parentNum)
{

    std::vector<Chromosome> rankedList = RankChroms(gen);

    //select n out of rankedList
    std::vector<Chromosome> selected;
    if (selectNum - 1 > rankedList.size())
    {
        std::cout << "Generation is too small to select this many indviduals." << std::endl;
        selectNum = (int)rankedList.size();
    }
    for (int i = 0; i < selectNum; i++)
    {
        selected.push_back(rankedList[i]);
    }

    //add to selected chroms to mating pool - copies are allowed
    std::vector<Chromosome> matingPool;
    int selectedIndex = 0;
    do
    {
        matingPool.push_back(selected[selectedIndex]);

        selectedIndex++;

        if (selectedIndex >= selected.size())
        {
            selectedIndex = 0;
        }
    } while (matingPool.size() < parentNum);

    return matingPool;
}

//split generation into bands select n from each band
std::vector<Chromosome> GASelection::LinearRankedSelection(std::vector<Chromosome>* gen, unsigned int seed, std::initializer_list<int> bandDists)
{
    std::vector<Chromosome> rankedList = RankChroms(gen);
    std::vector<Chromosome> matingPool;

    std::mt19937 mt(seed);

    int bandStart = 0;
    int bandDist;
    int randNum;
    //for as many bands as therre are
    for (int i = 0; i < bandDists.size(); i++)
    {
        //how many selected from current band?
        bandDist = *(bandDists.begin() + i);
        std::uniform_int_distribution<int> dist(bandStart, bandStart + bandDist - 1);
        for (int n = 0; n < bandDist; n++)
        {
            randNum = dist(mt);
            matingPool.push_back(rankedList[randNum]); //select indiv in current band randomly
        }
        bandStart += bandDist; //move to next lower fitness band
    }
    return matingPool;
}

//adds indivduals to returned mating pool - linear ranked selection
std::vector<Chromosome> GASelection::ElitistSelection(std::vector<Chromosome>* gen, unsigned int seed, std::vector<Chromosome> indivs, std::initializer_list<int> bandDists)
{
    std::vector<Chromosome> matingPool = LinearRankedSelection(gen, seed, bandDists);
    if (indivs.size() != 0)
    {
        for (Chromosome indiv : indivs)
        {
            matingPool.push_back(indiv);
        }
    }
    return matingPool;
}

std::vector<Chromosome> GASelection::TournamentSelection(std::vector<Chromosome>* gen, unsigned int seed, int matingPoolSize, int tournySize)
{
    std::vector<Chromosome> matingPool;
    std::vector<Chromosome> roster = *gen;
    std::vector<Chromosome> tournament;
    std::mt19937 mt(seed);
    std::uniform_int_distribution<int> rosterIndex(0, (int)roster.size() - 1);

    int randNum;

    for (int i = 0; i < matingPoolSize; i++)
    {
        //picks participants
        for (int i = 0; i < tournySize; i++)
        {
            randNum = rosterIndex(mt);
            tournament.push_back(roster[randNum]);
        }
        //pick tournament winner aka most fit
        matingPool.push_back(RankChroms(&tournament)[0]);
        tournament.clear();
    }
    return matingPool;
}

//puts individuals on a wheel and spins it, whatever the picker falls on joins the mating pool
std::vector<Chromosome> GASelection::RouletteSelection(std::vector<Chromosome>* gen, unsigned int seed, int parentNum)
{
    float totalFitness = 0;
    for (Chromosome indiv : *gen)
    {
        totalFitness += indiv.m_fitness;
    }

    float pickerSpot;
    std::mt19937 mt(seed);
    std::uniform_real_distribution<float> randSpot(0, totalFitness);

    std::vector<Chromosome> wheel = *gen;
    wheel = RankChroms(&wheel);
    std::vector<Chromosome> matingPool;

    float spinDistance;
    for (int i = 0; i < parentNum; i++)
    {
        pickerSpot = randSpot(mt);
        spinDistance = pickerSpot;
        for (Chromosome indivSlot : wheel)
        {
            if (spinDistance >= 0 && spinDistance <= indivSlot.m_fitness)
            {
                matingPool.push_back(indivSlot);
                break;
            }
            spinDistance -= indivSlot.m_fitness;
        }
    }

    return matingPool;
}

//march around wheel a distance a set number of times add whatever the picker lands on each time
std::vector<Chromosome> GASelection::StochasticSamplingSelection(std::vector<Chromosome>* gen, unsigned int seed, int parentNum)
{
    float totalFitness = 0;
    for (Chromosome indiv : *gen)
    {
        totalFitness += indiv.m_fitness;
    }

    std::vector<Chromosome> wheel = *gen;
    wheel = RankChroms(&wheel);
    std::vector<Chromosome> matingPool;

    float pickerSpot = 0;
    int index = 0;
    float spinDistance = 0;
    int curPoolSize;
    float march = totalFitness/(float)wheel.size(); //step = total fitness / size of mating pool 
    for (int i = 0; i < parentNum; i++)
    {
        if (i == 0)
        {
            std::mt19937 mt(seed);
            std::uniform_real_distribution<float> randSpot(0, march);
            pickerSpot = randSpot(mt);
        }
        else
        {
            pickerSpot += march * (i + 1);
        }

        spinDistance = pickerSpot;
        curPoolSize = (int)matingPool.size();

        while (matingPool.size() < curPoolSize + 1)
        {
            if (index >= wheel.size())
            {
                index = 0;
            }
            Chromosome indivSlot = wheel[index];
            if (spinDistance >= 0 && spinDistance <= indivSlot.m_fitness)
            {
                matingPool.push_back(indivSlot);
                break;
            }
            spinDistance -= indivSlot.m_fitness;
            index++;
        }
    }

    return matingPool;
}