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

std::vector<individual> GASelection::RankChroms(std::vector<individual> indivs)
{
    std::vector<individual> rankedList;
    std::vector<individual> tempList = indivs;

    //order chromosomes by fitness biggest to lowests
    for (int i = 0; i < (int)indivs.size(); i++)
    {
        individual fittest;
        int fittestIndex = 0;
        for (int i = 0; i < tempList.size(); i++)
        {
            if (i == 0)
            {
                fittest = tempList[i];
                fittestIndex = i;
            }
            else if (fittest.first.m_fitness < tempList[i].first.m_fitness)
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
std::vector<individual> GASelection::Ranked(std::vector<individual> gen, int selectNum, int parentNum)
{

    std::vector<individual> rankedList = RankChroms(gen);

    //select n out of rankedList
    std::vector<individual> selected;
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
    std::vector<individual> matingPool;
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
std::vector<individual> GASelection::LinearRanked(std::vector<individual> gen, std::vector<int> bandDists)
{
    std::vector<individual> rankedList = RankChroms(gen);
    std::vector<individual> matingPool;

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
            randNum = dist(m_gen);
            matingPool.push_back(rankedList[randNum]); //select indiv in current band randomly
        }
        bandStart += bandDist; //move to next lower fitness band
    }
    return matingPool;
}

//adds indivduals to returned mating pool - linear ranked selection
std::vector<individual> GASelection::Elitist(std::vector<individual> gen, std::vector<individual> indivs, std::vector<int> bandDists)
{
    std::vector<individual> matingPool = LinearRanked(gen, bandDists);
    if (indivs.size() != 0)
    {
        for (individual indiv : indivs)
        {
            matingPool.push_back(indiv);
        }
    }
    return matingPool;
}

std::vector<individual> GASelection::Tournament(std::vector<individual> gen, int matingPoolSize, int tournySize)
{
    std::vector<individual> matingPool;
    std::vector<individual> roster = gen;
    std::vector<individual> tournament;
    std::uniform_int_distribution<int> rosterIndex(0, (int)roster.size() - 1);

    int randNum;

    for (int i = 0; i < matingPoolSize; i++)
    {
        //picks participants
        for (int i = 0; i < tournySize; i++)
        {
            randNum = rosterIndex(m_gen);
            tournament.push_back(roster[randNum]);
        }
        //pick tournament winner aka most fit
        matingPool.push_back(RankChroms(tournament)[0]);
        tournament.clear();
    }
    return matingPool;
}

//puts individuals on a wheel and spins it, whatever the picker falls on joins the mating pool
std::vector<individual> GASelection::Roulette(std::vector<individual> gen, int parentNum)
{
    float totalFitness = 0;
    for (individual indiv : gen)
    {
        totalFitness += indiv.first.m_fitness;
    }

    float pickerSpot;
    std::uniform_real_distribution<float> randSpot(0, totalFitness);

    std::vector<individual> wheel = gen;
    wheel = RankChroms(wheel);
    std::vector<individual> matingPool;

    float spinDistance;
    for (int i = 0; i < parentNum; i++)
    {
        pickerSpot = randSpot(m_gen);
        spinDistance = pickerSpot;
        for (individual indivSlot : wheel)
        {
            if (spinDistance >= 0 && spinDistance <= indivSlot.first.m_fitness)
            {
                matingPool.push_back(indivSlot);
                break;
            }
            spinDistance -= indivSlot.first.m_fitness;
        }
    }

    return matingPool;
}

//march around wheel a distance a set number of times add whatever the picker lands on each time
std::vector<individual> GASelection::StochasticSampling(std::vector<individual> gen, int parentNum)
{
    float totalFitness = 0;
    for (individual indiv : gen)
    {
        totalFitness += indiv.first.m_fitness;
    }

    std::vector<individual> wheel = gen;
    wheel = RankChroms(wheel);
    std::vector<individual> matingPool;

    float pickerSpot = 0;
    int index = 0;
    float spinDistance = 0;
    int curPoolSize;
    float march = totalFitness / (float)wheel.size(); //step = total fitness / size of mating pool 
    for (int i = 0; i < parentNum; i++)
    {
        if (i == 0)
        {
            //std::mt19937 mt(seed);
            std::uniform_real_distribution<float> randSpot(0, march);
            pickerSpot = randSpot(m_gen);
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
            individual indivSlot = wheel[index];
            if (spinDistance >= 0 && spinDistance <= indivSlot.first.m_fitness)
            {
                matingPool.push_back(indivSlot);
                break;
            }
            spinDistance -= indivSlot.first.m_fitness;
            index++;
        }
    }

    return matingPool;
}