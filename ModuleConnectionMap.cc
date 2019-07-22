#include "ModuleConnectionMap.h"

SDL::ModuleConnectionMap SDL::moduleConnectionMap;

SDL::ModuleConnectionMap::ModuleConnectionMap()
{
}

SDL::ModuleConnectionMap::ModuleConnectionMap(std::string filename)
{
    load(filename);
}

SDL::ModuleConnectionMap::~ModuleConnectionMap()
{
}

void SDL::ModuleConnectionMap::load(std::string filename)
{
    moduleConnections_.clear();

    std::ifstream ifile;
    ifile.open(filename.c_str());
    std::string line;

    while (std::getline(ifile, line))
    {

        unsigned int detid;
        int number_of_connections;
        std::vector<unsigned int> connected_detids;
        unsigned int connected_detid;

        std::stringstream ss(line);

        ss >> detid >> number_of_connections;

        for (int ii = 0; ii < number_of_connections; ++ii)
        {
            ss >> connected_detid;
            connected_detids.push_back(connected_detid);
        }

        moduleConnections_[detid] = connected_detids;

    }
}

void SDL::ModuleConnectionMap::print()
{
    std::cout << "Printing ModuleConnectionMap" << std::endl;
    for (auto& pair : moduleConnections_)
    {
        unsigned int detid = pair.first;
        std::vector<unsigned int> connected_detids = pair.second;
        std::cout <<  " detid: " << detid <<  std::endl;
        for (auto& connected_detid : connected_detids)
        {
            std::cout <<  " connected_detid: " << connected_detid <<  std::endl;
        }

    }
}

std::vector<unsigned int> SDL::ModuleConnectionMap::getConnectedModuleDetIds(unsigned int detid)
{
    return moduleConnections_[detid];
}
