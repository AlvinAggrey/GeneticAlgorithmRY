#include "AIController.h"
#include "Timer.h"
#include "GameState.h"
#include <iostream>
#include <Windows.h>
#include <sstream>

#include "GameController.h"

using namespace std;

int AIController::FindStringEnd(std::ifstream* ifs, std::string str)
{
	ifs->clear();

	while (ifs->eof() == false)
	{
		std::string temp;
		int stringPos = 0;
		char character;
		for (int i = 0; i < str.size(); i++)
		{
			character = ifs->get();

			if (character == str[stringPos])
			{
				temp += character;
				stringPos++;
			}
			else
			{
				break;
			}
		}
		if (temp == str)
		{
			return ifs->tellg();
		}
	}
	return -1;
}
void AIController::SaveSettings(int genNum, int popNum, int childAmount,
	int selectionMethod, int crossoverMethod, float mutationChance,
	float selectNum, float matingPoolSize, float tournySize, std::vector<int> bandDists)
{
	std::ofstream ofs("Settings.txt");
	ofs << m_dataTags[0] << " " << genNum << std::endl;
	ofs << m_dataTags[1] << " " << popNum << std::endl;
	ofs << m_dataTags[2] << " " << childAmount << std::endl;
	ofs << m_dataTags[3] << " " << selectionMethod << std::endl;
	ofs << m_dataTags[4] << " " << crossoverMethod << std::endl;
	ofs << m_dataTags[5] << " " << mutationChance << std::endl;
	ofs << m_dataTags[6] << " " << selectNum;
	ofs << m_dataTags[7] << " " << matingPoolSize << std::endl;
	ofs << m_dataTags[8] << " " << tournySize << std::endl;
	ofs << m_dataTags[9] << " [";

	for (int i = 0; i < bandDists.size(); i++)
	{
		if (i == bandDists.size() - 1)
		{
			ofs << bandDists[i] << ' ';
		}
		else
		{
			ofs << bandDists[i];
		}
	}
	ofs << "]" << std::endl;
}
void AIController::LoadSettings()
{
	std::vector<std::vector<float>> m_settingsData[10];
	std::ifstream ifs("Settings.txt");
	if (ifs.is_open() == false)
	{
		std::cout << "Cannot load setting - Defaults will be used." << std::endl;
		SaveSettings(2, 2, 1, 1, 1, 0.1,2,4,4,{2,2});
	}
	else
	{
		int genNum;
		int popNum;
		int childAmount;
		SelectionMethod selectionMethod;
		CrossoverMethod crossoverMethod;
		float mutationChance; 
		float selectNum;
		float matingPoolSize;
		float tournySize;
		std::vector<int> bandDists;

		int tempNum;

		FindStringEnd(&ifs, m_dataTags[0]);
		ifs >> genNum;
		FindStringEnd(&ifs, m_dataTags[1]);
		ifs >> popNum;
		FindStringEnd(&ifs, m_dataTags[2]);
		ifs >> childAmount;
		
		FindStringEnd(&ifs, m_dataTags[3]);
		tempNum = 0;
		ifs >> tempNum;
		selectionMethod = (SelectionMethod) tempNum;
		FindStringEnd(&ifs, m_dataTags[4]);
		tempNum = 0;
		ifs >> tempNum;
		crossoverMethod = (CrossoverMethod)tempNum;

		FindStringEnd(&ifs, m_dataTags[5]);
		ifs >> mutationChance;
		FindStringEnd(&ifs, m_dataTags[6]);
		ifs >> selectNum;
		FindStringEnd(&ifs, m_dataTags[7]);
		ifs >> matingPoolSize;
		FindStringEnd(&ifs, m_dataTags[8]);
		ifs >> tournySize;

		FindStringEnd(&ifs, m_dataTags[9]);
		char character;
		tempNum = 0;
		ifs >> character;
		if (character == '[')
		{
			while (ifs.peek() != ']')
			{
				ifs >> tempNum;
				bandDists.push_back(tempNum);
			}
		}
		
		ga = GA(genNum,popNum,childAmount,8,selectionMethod,crossoverMethod);
		
		switch (selectionMethod)
		{
		case SelectionMethod::Ranked:
			ga.UseRanked(selectNum, matingPoolSize);
			break;
		case SelectionMethod::Roulette:
			ga.UseRoulette(matingPoolSize);
			break;
		case SelectionMethod::LinearRanked:
			ga.UseLinearRanked(bandDists);
			break;
		case SelectionMethod::Elitist:
			ga.UseElitist(bandDists);
			break;
		case SelectionMethod::Tournament:
			ga.UseTournament(matingPoolSize, tournySize);
			break;
		case SelectionMethod::Stochastic:
			ga.UseStochastic(matingPoolSize, tournySize);
			break;
		}
		//ifs.ignore(256, );
	}
	ga.Init();
}


void AIController::SettingsPrompt()
{
	int input;
	bool done = false;
	std::vector<std::string> changeSettingPrompts
	{
		"How many Generations would you like to run?",
		"How big should each generation be?",
		"How many children should each generation have?",
		"What Selection method do you want to use? (0 to 6)",
		"What crossover method do you want to use?",
		"What mutation chance do you want to have?"
	};
	while (done == false)
	{
		input = 0;
		std::cout << "-- Settings --" << std::endl;
		std::cout << "Would you like to change settings?" << std::endl;
		std::cout << "1. Yes" << std::endl;
		std::cout << "2. No" << std::endl;
		std::cin >> input;
		float settingsData[6];
			std::vector<int> bandDists;
		switch (input)
		{
		case 1:
		{
			for (int i = 0; i < 6; i++)
			{
				std::cout << changeSettingPrompts[i] << std::endl;
				//float settingInput;

				std::cin >> settingsData[i];
			}

			SelectionMethod selection = (SelectionMethod)settingsData[3];
			int matingpoolSize = 0;
			int tournySize = 0;
			int selectNum = 0;

			if (selection == SelectionMethod::Ranked ||
				selection == SelectionMethod::Tournament ||
				selection == SelectionMethod::Stochastic ||
				selection == SelectionMethod::Roulette)
			{
				std::cout << "How many individuals to select for mating pool?" << std::endl;
				std::cin >> selectNum;
			}
			if (selection == SelectionMethod::Tournament)
			{
				std::cout << "How many individuals in the tournament?" << std::endl;
				std::cin >> tournySize;
			}
			if (selection == SelectionMethod::Ranked||
				selection == SelectionMethod::Tournament||
				selection == SelectionMethod::Roulette)
			{
				std::cout << "How many indivduals in the mating pool?" << std::endl;
				std::cin >> matingpoolSize;
			}
			if (selection == SelectionMethod::LinearRanked||
				selection == SelectionMethod::Elitist)
			{
				std::cout << "How many individuals in each band? (perate each band with a ' ')" << std::endl;
				std::stringstream ss;
				std::string bands;
				ss.str(bands);

				if (bands.size() < 0)
				{
					bandDists.push_back(0);
				}
				else
				{
					while(ss.eof() == false)
					{
						int num;
						ss >> num;
						bandDists.push_back(num);
					}
				}
			}

			SaveSettings(settingsData[0], settingsData[1], settingsData[2], settingsData[3], settingsData[4], 
				settingsData[5],selectNum, matingpoolSize,tournySize, bandDists);
			std::cout << "Saved." << std::endl;
			done = false;
			break;
		}

		case 2:
			done = true;
			break;
		}
	}
}
bool AIController::LoadEpochPrompt()
{
	std::string input;

	std::cout << "Please Write the Epoch Files Name: ";
	std::cin >> input;
	std::vector<Chromosome> epoch = fileHandler.ReadGenFromFile(input);

	
	if (epoch.size() < 2)
	{
		//return to main menu
		return false;
	}
	else
	{
		//run the program
		std::vector<individual> epochIndivs;
		for (int i = 0; i < epoch.size(); i++)
		{
			epochIndivs.push_back(individual(epoch[i], false));
		}
		
		ga.Init(epochIndivs);
		//ga.NextGen();
		return true;
	}
}
void AIController::MainMenu()
{
	bool run = false;
	int input;
	while (run != true)
	{
		std::cout << "-- Main menu --" << std::endl;
		std::cout << "1. Start From Scratch" << std::endl;
		std::cout << "2. Load Epoch File" << std::endl;
		std::cout << "3. Change Settings" << std::endl;
		std::cin >> input;
		switch (input)
		{
		case 1:
			run = true;
			break;
		case 2:
			run = LoadEpochPrompt();
			break;
		case 3:
			SettingsPrompt();
			break;
		default:
			std::cout << "invalid input" << std::endl;
			run = false;
			break;
		}
		input = 0;//default
	}

}

AIController::AIController()
{
	m_gameController = nullptr;
	m_gameBoard = nullptr;
	m_Timer = nullptr;
	m_gameState = nullptr;
	//ga = GA(2,2,2,SelectionMethod::Roulette, CrossoverMethod::OnePoint);
	LoadSettings();
	MainMenu();
}

AIController::~AIController()
{

}

void AIController::gameOver()
{
	int waveReached = m_gameState->getCurrentWave();
	//int avgkillCount = m_gameState->getMonsterEliminated()/ waveReached;
	//int tamsLeft = ;
	int fitness = 0;
	if (m_unplaceableTowercount >= ga.GetChromSize())
	{
		fitness = 0;
	}
	else
	{
		fitness = recordScore() + m_gameState->getTams() - (m_unplaceableTowercount *= 10);
	}

	ga.CurIndiv().first.m_fitness = fitness;
	ga.CurIndiv().second = true;

	if (ga.CheckIsDone())
	{
		std::vector<individual> lastGen = ga.CurGen();
		std::vector<Chromosome> epoch;
		for (int i = 0; i < lastGen.size(); i++)
		{
			epoch.push_back(lastGen[i].first);
		}
		fileHandler.WriteGenToFile(epoch);
		done = true;
	}
	else if (ga.CheckTestAllPop())
	{
		ga.NextGen();
	}
	else
	{
		if (recordScore() >= 200)
		{
			fileHandler.WriteGenToFile({ ga.CurIndiv().first }, "Highscorers.txt");
		}
		ga.NextIndiv();
	}
	
	m_unplaceableTowercount = 0;
	ResetGeneStep();
}

void AIController::update()
{
	if (m_Timer == nullptr)
		return;

	// HINT
	// a second has elapsed - your GA manager (GA Code) may decide to do something at this time...
	static double elapsedSeconds = 0;
	double seconds = floor(m_Timer->elapsedSeconds());
	if (seconds > elapsedSeconds)
	{
		elapsedSeconds = seconds;

	}
	if (!done)
	{
		MakeMove();
	}
	//GAManager::Instance()->Update(m_Timer->elapsedSeconds());

	// this might be useful? Monsters killed
	static int monstersKilled = 0;

	if (m_gameState->getMonsterEliminated() > monstersKilled)
	{
		monstersKilled = m_gameState->getMonsterEliminated();
	}

	recordScore();
}

bool AIController::addTower(TowerType type, int gridx, int gridy)
{
	// grid position can be from 0,0 to 25,17
	/*
	enum class TowerType {
	empty, slammer, swinger, thrower };
	*/

	bool towerAdded = m_gameBoard->addTower(type, gridx, gridy);

	// NOTE towerAdded might be false if the tower can't be placed in that position, is there isn't enough funds

	return towerAdded;
}

void AIController::MakeMove()
{
	if (m_geneStep < ga.CurIndiv().first.ChromSize())
	{
		Chromosome chrom  = ga.CurIndiv().first;
		Gene gene = chrom.Genes()[m_geneStep];
		m_nextTower = gene;
		if (m_gameBoard->gridSpaceAvailable(gene.m_position[0], gene.m_position[1]) == true)
		{
			if (addTower((TowerType)gene.m_towerType, gene.m_position[0],gene.m_position[1]))
			{
				m_geneStep++;
			}
		}
		else
		{
			m_unplaceableTowercount++;
			//skip
			m_geneStep++;
		}

	}
}
void AIController::RenderNextTowerPos(int range, sf::Font orderFont, sf::RectangleShape highlight, sf::RenderWindow* window)
{
	if (done)
	{
		RenderText(70, orderFont, "done", 13 * 60, 6 * 60, window);
		return;
	}
	Chromosome chrom = ga.CurIndiv().first;
	Gene gene;
	for (int i = 0; i < chrom.ChromSize(); i++)
	{
		gene = chrom.Genes()[i];
		int gridX = gene.m_position[0];
		int gridY = gene.m_position[1];
		highlight.setSize(sf::Vector2f((float)range * 60, (float)range * 60));
		highlight.setPosition((float)gridX * 60, (float)gridY * 60);
		window->draw(highlight);
		RenderText(15, orderFont, std::to_string(i) + ". TowerType:" + std::to_string(gene.m_towerType), gridX * 60, gridY * 60, window);
	}
}

void AIController::RenderGenerationLabels(int fontSize, sf::Font font, sf::RenderWindow* window)
{
	std::stringstream genInfo;
	std::stringstream chromInfo;
	genInfo << "Generation: " << ga.CurGenIndex() + 1; ///<< " / " << ga.m_gens.size() - 1;
	chromInfo << "Chromosome: " << ga.CurIndivIndex() + 1 << " / " << ga.CurGen().size();
	RenderText(fontSize, font, genInfo.str(), 140, 300, window);
	RenderText(fontSize, font, chromInfo.str(), 140, 400, window);
}

void AIController::RenderText(int fontSize, sf::Font font, std::string contents, float x, float y, sf::RenderWindow* window)
{
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(fontSize);
	text.setPosition(sf::Vector2f(x, y));
	text.setString(contents);
	window->draw(text);
}

void AIController::ResetGeneStep()
{
	m_geneStep = 0;
}


void AIController::setupBoard()
{
	m_Timer->start();
}

int AIController::recordScore()
{
	int currentWave = m_gameState->getCurrentWave();
	int killCount = m_gameState->getMonsterEliminated();
	currentWave *= 10; // living longer is good
	int score = currentWave + killCount;

	static int iteration = 0;

	if (iteration == 0)
		cout << "iteration" << "," << "wave" << "," << "kills" << "," << "score" << endl;

	cout << iteration << "," << m_gameState->getCurrentWave() << "," << m_gameState->getMonsterEliminated() << "," << score << endl;
	iteration++;

	m_gameState->setScore(score);

	return score;
}