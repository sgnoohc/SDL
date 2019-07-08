#ifndef Event_h
#define Event_h

#include <vector>
#include <list>
#include <map>
#include <stdlib.h>
#include <stdexcept>
#include <iostream>
#include <cmath>

#include "Module.h"
#include "Hit.h"
#include "MiniDoublet.h"
#include "Segment.h"
#include "PrintUtil.h"
#include "Algo.h"
#include "ModuleConnectionMap.h"

namespace SDL
{
    class Event
    {
        private:

            // map of modules (this holds the actual instances)
            std::map<unsigned int, Module> modulesMapByDetId_;

            // list of hits (this holds the actual instances)
            std::list<Hit> hits_;

            // list of MiniDoublets (this holds the actual instances)
            std::list<MiniDoublet> miniDoublets_;

            // list of MiniDoublets (this holds the actual instances)
            std::list<Segment> segments_;

            // list of module pointers (hold only the pointers to the actual instances)
            std::vector<Module*> modulePtrs_;

            // list of lower module pointers (hold only the pointers to the actual instances)
            // (lower means, the module that is closer to the luminous region)
            std::vector<Module*> lowerModulePtrs_;

            // boolean to turn on debug mode
            SDL::LogLevel logLevel_;

        public:

            // cnstr/destr
            Event();
            ~Event();

            // Module related functions
            bool hasModule(unsigned int detId);
            Module& getModule(unsigned int detId);
            const std::vector<Module*> getModulePtrs() const;
            const std::vector<Module*> getLowerModulePtrs() const;

            // Set debug
            void setLogLevel(SDL::LogLevel logLevel=SDL::Log_Nothing);

            // Hit related functions
            void addHitToModule(Hit hit, unsigned int detId);

            // MiniDoublet related functions
            void addMiniDoubletToLowerModule(MiniDoublet md, unsigned int detId);

            // Segment related functions
            void addSegmentToLowerModule(Segment md, unsigned int detId);

            // Create mini doublets
            void createMiniDoublets(MDAlgo algo=Default_MDAlgo);

            // Create mini doublet for a module
            void createMiniDoubletsFromLowerModule(unsigned int detId, MDAlgo algo=Default_MDAlgo);

            // Create mini doublets
            void createSegments(SGAlgo algo=Default_SGAlgo);

            // Create mini doublet for a module
            void createSegmentsFromInnerLowerModule(unsigned int detId, SGAlgo algo=Default_SGAlgo);

            // cout printing
            friend std::ostream& operator<<(std::ostream& out, const Event& event);
            friend std::ostream& operator<<(std::ostream& out, const Event* event);

    };
}

#endif
