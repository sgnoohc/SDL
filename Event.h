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
#include "Layer.h"
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

            // map of barrel layers (this holds the actual instances)
            std::map<int, Layer> barrelLayers_;

            // map of endcap layers (this holds the actual instances)
            std::map<int, Layer> endcapLayers_;

            // list of hits (this holds the actual instances)
            std::list<Hit> hits_;

            // list of MiniDoublets (this holds the actual instances)
            std::list<MiniDoublet> miniDoublets_;

            // list of Segments (this holds the actual instances)
            std::list<Segment> segments_;

            // list of Tracklets (this holds the actual instances)
            std::list<Tracklet> tracklets_;

            // list of TrackCandidates (this holds the actual instances)
            std::list<TrackCandidate> trackcandidates_;

            // list of module pointers (hold only the pointers to the actual instances)
            std::vector<Module*> modulePtrs_;

            // list of layer pointers (hold only the pointers to the actual instances)
            std::vector<Layer*> layerPtrs_;

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

            // Layer related functions
            void createLayers();
            Layer& getLayer(int layer, SDL::Layer::SubDet subdet);
            const std::vector<Layer*> getLayerPtrs() const;

            // Set debug
            void setLogLevel(SDL::LogLevel logLevel=SDL::Log_Nothing);

            // Hit related functions
            void addHitToModule(Hit hit, unsigned int detId);

            // MiniDoublet related functions
            void addMiniDoubletToLowerModule(MiniDoublet md, unsigned int detId);

            // Segment related functions
            void addSegmentToLowerModule(Segment md, unsigned int detId);

            // Segment related functions
            void addSegmentToLowerLayer(Segment md, int layerIdx, SDL::Layer::SubDet subdet);

            // Tracklet related functions
            void addTrackletToLowerLayer(Tracklet tl, int layerIdx, SDL::Layer::SubDet subdet);

            // TrackCandidate related functions
            void addTrackCandidateToLowerLayer(TrackCandidate tc, int layerIdx, SDL::Layer::SubDet subdet);

            // Create mini doublets
            void createMiniDoublets(MDAlgo algo=Default_MDAlgo);

            // Create mini doublet for a module
            void createMiniDoubletsFromLowerModule(unsigned int detId, MDAlgo algo=Default_MDAlgo);

            // Create segments
            void createSegments(SGAlgo algo=Default_SGAlgo);

            // Create segments for a lower module
            void createSegmentsFromInnerLowerModule(unsigned int detId, SGAlgo algo=Default_SGAlgo);

            // Create tracklets
            void createTracklets(TLAlgo algo=Default_TLAlgo);

            // Create tracklets from two layers (inefficient way)
            void createTrackletsFromTwoLayers(int innerLayerIdx, SDL::Layer::SubDet innerLayerSubDet, int outerLayerIdx, SDL::Layer::SubDet outerLayerSubDet, TLAlgo algo=Default_TLAlgo);

            // Create trackcandidates
            void createTrackCandidates(TCAlgo algo=Default_TCAlgo);

            // Create trackcandidates from two layers (inefficient way)
            void createTrackCandidatesFromTwoLayers(int innerLayerIdx, SDL::Layer::SubDet innerLayerSubDet, int outerLayerIdx, SDL::Layer::SubDet outerLayerSubDet, TCAlgo algo=Default_TCAlgo);

            // cout printing
            friend std::ostream& operator<<(std::ostream& out, const Event& event);
            friend std::ostream& operator<<(std::ostream& out, const Event* event);

    };
}

#endif
