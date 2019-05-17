#include "EndcapGeometry.h"

SDL::EndcapGeometry SDL::endcapGeometry;

SDL::EndcapGeometry::EndcapGeometry()
{
}

SDL::EndcapGeometry::EndcapGeometry(std::string filename)
{
    load(filename);
}

SDL::EndcapGeometry::~EndcapGeometry()
{
}

void SDL::EndcapGeometry::load(std::string filename)
{
    avgr2s_.clear();
    yls_.clear();
    sls_.clear();
    yus_.clear();
    sus_.clear();

    std::ifstream ifile;
    ifile.open(filename.c_str());
    std::string line;

    while (std::getline(ifile, line))
    {

        unsigned int detid;
        float avgr2;
        float yl;
        float sl;
        float yh;
        float sh;

        std::stringstream ss(line);

        ss >> detid >> avgr2 >> yl >> sl >> yh >> sh;

        // std::cout <<  " detid: " << detid <<  " avgr2: " << avgr2 <<  " yl: " << yl <<  " sl: " << sl <<  " yh: " << yh <<  " sh: " << sh <<  std::endl;

        avgr2s_[detid] = avgr2;
        yls_[detid] = yl;
        sls_[detid] = sl;
        yus_[detid] = yh;
        sus_[detid] = sh;
    }
}

float SDL::EndcapGeometry::getAverageR2(unsigned int detid)
{
    return avgr2s_[detid];
}

float SDL::EndcapGeometry::getYInterceptLower(unsigned int detid)
{
    return yls_[detid];
}

float SDL::EndcapGeometry::getSlopeLower(unsigned int detid)
{
    return sls_[detid];
}

float SDL::EndcapGeometry::getYInterceptUpper(unsigned int detid)
{
    return yus_[detid];
}

float SDL::EndcapGeometry::getSlopeUpper(unsigned int detid)
{
    return sus_[detid];
}
