#ifndef EndcapGeometry_h
#define EndcapGeometry_h

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace SDL
{

    class EndcapGeometry
    {

        private:
            std::map<unsigned int, float> avgr2s_;
            std::map<unsigned int, float> yls_; // lower hits
            std::map<unsigned int, float> sls_; // lower slope
            std::map<unsigned int, float> yus_; // upper hits
            std::map<unsigned int, float> sus_; // upper slope

        public:
            EndcapGeometry();
            EndcapGeometry(std::string filename);
            ~EndcapGeometry();

            void load(std::string);

            float getAverageR2(unsigned int detid);
            float getYInterceptLower(unsigned int detid);
            float getSlopeLower(unsigned int detid);
            float getYInterceptUpper(unsigned int detid);
            float getSlopeUpper(unsigned int detid);

    };

    extern EndcapGeometry endcapGeometry;

}

#endif
