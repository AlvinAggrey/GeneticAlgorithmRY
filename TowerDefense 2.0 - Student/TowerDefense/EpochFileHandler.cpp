#include "EpochFileHandler.h"
#include <chrono>
#include <ctime>


void EpochFileHandler::WriteGenToFile(std::vector<Chromosome> generation, std::string filename)
{
    Chromosome chrom;
    std::ofstream of;
    of.open(filename, std::ofstream::app | std::ofstream::out);
    std::stringstream data;auto curTimepoint = std::chrono::system_clock::now();

    //std::time_t time = std::chrono::system_clock::to_time_t(curTimepoint);
    //data << std::endl << std::ctime(&time) << std::endl;
    //convert to format
    data << "\n{\n";
    for (int i = 0; i < generation.size(); i++)
    {
        chrom = generation[i];
        data << "[";
        std::vector<Gene> genes = chrom.Genes();
        for (int n = 0; n < chrom.ChromSize(); n++)
        {
            Gene gene = genes[n];
            data << gene.m_position[0] << " " << gene.m_position[1] << " " << (int)gene.m_towerType;
            if (n < chrom.ChromSize() - 1)
            {
                data << ", ";
            }
        }
        data << "]=" << chrom.m_fitness << ";";
        if (i < generation.size())
        {
            data << '\n';
        }
    }
    data << "}\n";

    //write to file
    std::string s = data.str();
    data.seekp(0, std::ios::end);
    int count = data.tellp();
    of.write(s.c_str(), count);
    of.close();
}

Chromosome EpochFileHandler::ExtractChromosome(std::string seq)
{
    std::stringstream ss;
    ss.str(seq);
    std::vector<int> data;

    ss.ignore(256, '[');
    if (ss.fail() || ss.eof())
    {
        return Chromosome();
    }
    //extract data
    //loop until end of sequence
    char peek;
    while (ss.peek() != ']')
    {
        while ((peek = ss.peek()) != ']')
        {
            if (std::isdigit(ss.peek()))
            {
                int num;
                ss >> num;
                data.push_back(num);
                if (ss.fail() || ss.eof())
                {
                    return Chromosome();
                }
            }
            else if (peek == ' ' || peek == ',')
            {
                ss.ignore(1);
            }
        }
    }
    float fitness;
    ss.ignore(256, '=') >> fitness;
    if (ss.fail() || ss.eof())
    {
        return Chromosome();
    }

    //Create Chromosome
    Chromosome chrom;
    int geneNum = data.size() / 3;

    for (int i = 0; i < geneNum; i++)
    {
        int entryNum = i * 3;
        chrom.AddGene((Gene(data[entryNum], data[entryNum + 1], data[entryNum + 2])));
    }
    chrom.m_fitness = fitness;
    //blah blah balh add data into each gene in chromosome

    return chrom;
}

std::vector<Chromosome> EpochFileHandler::ReadGenFromFile(std::string filename)
{
    std::ifstream ifs(filename);

    std::vector<Chromosome> chroms;
    std::vector<Chromosome> NoData;
    int lastValidFilePos = 0;
    int curFilePos = 0;
    //find last epoch in file
    bool end;
    do
    {
        lastValidFilePos = curFilePos;
        curFilePos = ifs.ignore(std::numeric_limits<std::streamsize>::max(), '{').tellg();
    } while ((end = ifs.eof()) == false);

    ifs.clear();
    ifs.seekg(lastValidFilePos);
    //extract chromosomes
    do
    {
        int g = ifs.fail();
        int seqStart = ifs.ignore(256, '[').tellg(); seqStart -= 1;
        int seqEnd = ifs.ignore(256, ';').tellg();
        int seqLength = seqEnd - seqStart;
        if (ifs.eof())
        {
            ifs.close();
            return chroms;
        }
        else if (ifs.fail())
        {
            ifs.close();
            return NoData;
        }
        else
        {
            ifs.seekg(seqStart);//move back to beginning of sequence	
            char* seq = new char[seqLength];

            ifs.read(seq, seqLength);
            g = ifs.fail();
            chroms.push_back(ExtractChromosome(seq));
            ifs.seekg(seqEnd);
        };
    } while (ifs.eof() == false);
    ifs.close();
    return chroms;
}
