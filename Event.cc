#include "Event.h"

SDL::Event::Event() : logLevel_(SDL::Log_Nothing)
{
    createLayers();
}

SDL::Event::~Event()
{
}

bool SDL::Event::hasModule(unsigned int detId)
{
    if (modulesMapByDetId_.find(detId) == modulesMapByDetId_.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void SDL::Event::setLogLevel(SDL::LogLevel logLevel)
{
    logLevel_ = logLevel;
}

SDL::Module& SDL::Event::getModule(unsigned int detId)
{
    // using std::map::emplace
    std::pair<std::map<unsigned int, Module>::iterator, bool> emplace_result = modulesMapByDetId_.emplace(detId, detId);

    // Retreive the module
    auto& inserted_or_existing = (*(emplace_result.first)).second;

    // If new was inserted, then insert to modulePtrs_ pointer list
    if (emplace_result.second) // if true, new was inserted
    {

        // The pointer to be added
        Module* module_ptr = &((*(emplace_result.first)).second);

        // Add the module pointer to the list of modules
        modulePtrs_.push_back(module_ptr);

        // If the module is lower module then add to list of lower modules
        if (module_ptr->isLower())
            lowerModulePtrs_.push_back(module_ptr);
    }

    return inserted_or_existing;
}

const std::vector<SDL::Module*> SDL::Event::getModulePtrs() const
{
    return modulePtrs_;
}

const std::vector<SDL::Module*> SDL::Event::getLowerModulePtrs() const
{
    return lowerModulePtrs_;
}

void SDL::Event::createLayers()
{
    // Create barrel layers
    for (int ilayer = SDL::Layer::BarrelLayer0; ilayer < SDL::Layer::nBarrelLayer; ++ilayer)
    {
        barrelLayers_[ilayer] = SDL::Layer(ilayer, SDL::Layer::Barrel);
        layerPtrs_.push_back(&(barrelLayers_[ilayer]));
    }

    // Create endcap layers
    for (int ilayer = SDL::Layer::EndcapLayer0; ilayer < SDL::Layer::nEndcapLayer; ++ilayer)
    {
        endcapLayers_[ilayer] = SDL::Layer(ilayer, SDL::Layer::Endcap);
        layerPtrs_.push_back(&(endcapLayers_[ilayer]));
    }
}

SDL::Layer& SDL::Event::getLayer(int ilayer, SDL::Layer::SubDet subdet)
{
    if (subdet == SDL::Layer::Barrel)
        return barrelLayers_[ilayer];
    else // if (subdet == SDL::Layer::Endcap)
        return endcapLayers_[ilayer];
}

const std::vector<SDL::Layer*> SDL::Event::getLayerPtrs() const
{
    return layerPtrs_;
}

void SDL::Event::addHitToModule(SDL::Hit hit, unsigned int detId)
{
    // Add to global list of hits, where it will hold the object's instance
    hits_.push_back(hit);

    // And get the module (if not exists, then create), and add the address to Module.hits_
    getModule(detId).addHit(&(hits_.back()));
}

void SDL::Event::addMiniDoubletToLowerModule(SDL::MiniDoublet md, unsigned int detId)
{
    // Add to global list of mini doublets, where it will hold the object's instance
    miniDoublets_.push_back(md);

    // And get the module (if not exists, then create), and add the address to Module.hits_
    getModule(detId).addMiniDoublet(&(miniDoublets_.back()));
}

void SDL::Event::addSegmentToLowerModule(SDL::Segment sg, unsigned int detId)
{
    // Add to global list of segments, where it will hold the object's instance
    segments_.push_back(sg);

    // And get the module (if not exists, then create), and add the address to Module.hits_
    getModule(detId).addSegment(&(segments_.back()));
}

void SDL::Event::addSegmentToLowerLayer(SDL::Segment sg, int layerIdx, SDL::Layer::SubDet subdet)
{
    // Add to global list of segments, where it will hold the object's instance
    segments_.push_back(sg);

    // And get the layer
    getLayer(layerIdx, subdet).addSegment(&(segments_.back()));
}

void SDL::Event::addTrackletToLowerLayer(SDL::Tracklet tl, int layerIdx, SDL::Layer::SubDet subdet)
{
    // Add to global list of tracklets, where it will hold the object's instance
    tracklets_.push_back(tl);

    // And get the layer
    getLayer(layerIdx, subdet).addTracklet(&(tracklets_.back()));
}

void SDL::Event::createMiniDoublets(MDAlgo algo)
{
    // Loop over lower modules
    for (auto& lowerModulePtr : getLowerModulePtrs())
    {

        // Create mini doublets
        createMiniDoubletsFromLowerModule(lowerModulePtr->detId(), algo);

    }
}

void SDL::Event::createMiniDoubletsFromLowerModule(unsigned int detId, SDL::MDAlgo algo)
{
    // Get reference to the lower Module
    Module& lowerModule = getModule(detId);

    // Get reference to the upper Module
    Module& upperModule = getModule(lowerModule.partnerDetId());

    // Double nested loops
    // Loop over lower module hits
    for (auto& lowerHitPtr : lowerModule.getHitPtrs())
    {

        // Get reference to lower Hit
        SDL::Hit& lowerHit = *lowerHitPtr;

        // Loop over upper module hits
        for (auto& upperHitPtr : upperModule.getHitPtrs())
        {

            // Get reference to upper Hit
            SDL::Hit& upperHit = *upperHitPtr;

            // Create a mini-doublet candidate
            SDL::MiniDoublet mdCand(lowerHitPtr, upperHitPtr);

            // Run mini-doublet algorithm on mdCand (mini-doublet candidate)
            mdCand.runMiniDoubletAlgo(algo, logLevel_);

            if (mdCand.passesMiniDoubletAlgo(algo))
                addMiniDoubletToLowerModule(mdCand, lowerModule.detId());

        }

    }

}

void SDL::Event::createSegments(SGAlgo algo)
{
    // Loop over lower modules
    for (auto& lowerModulePtr : getLowerModulePtrs())
    {

        // Create mini doublets
        createSegmentsFromInnerLowerModule(lowerModulePtr->detId(), algo);

    }
}

void SDL::Event::createSegmentsFromInnerLowerModule(unsigned int detId, SDL::SGAlgo algo)
{

    // x's and y's are mini doublets
    // -------x--------
    // --------x------- <--- outer lower module
    //
    // --------y-------
    // -------y-------- <--- inner lower module

    // Get reference to the inner lower Module
    Module& innerLowerModule = getModule(detId);

    // Triple nested loops
    // Loop over inner lower module mini-doublets
    for (auto& innerMiniDoubletPtr : innerLowerModule.getMiniDoubletPtrs())
    {

        // Get reference to mini-doublet in inner lower module
        SDL::MiniDoublet& innerMiniDoublet = *innerMiniDoubletPtr;

        // Get connected outer lower module detids
        std::vector<unsigned int> connectedModuleDetIds = moduleConnectionMap.getConnectedModuleDetIds(detId);

        // Loop over connected outer lower modules
        for (auto& outerLowerModuleDetId : connectedModuleDetIds)
        {

            if (not hasModule(outerLowerModuleDetId))
                continue;

            // Get reference to the outer lower module
            Module& outerLowerModule = getModule(outerLowerModuleDetId);

            // Loop over outer lower module mini-doublets
            for (auto& outerMiniDoubletPtr : outerLowerModule.getMiniDoubletPtrs())
            {

                // Get reference to mini-doublet in outer lower module
                SDL::MiniDoublet& outerMiniDoublet = *outerMiniDoubletPtr;

                // Create a segment candidate
                SDL::Segment sgCand(innerMiniDoubletPtr, outerMiniDoubletPtr);

                // Run segment algorithm on sgCand (segment candidate)
                sgCand.runSegmentAlgo(algo, logLevel_);

                if (sgCand.passesSegmentAlgo(algo))
                {
                    addSegmentToLowerModule(SDL::Segment(innerMiniDoubletPtr, outerMiniDoubletPtr), innerLowerModule.detId());
                    if (innerLowerModule.subdet() == SDL::Module::Barrel)
                        addSegmentToLowerLayer(SDL::Segment(innerMiniDoubletPtr, outerMiniDoubletPtr), innerLowerModule.layer(), SDL::Layer::Barrel);
                    else
                        addSegmentToLowerLayer(SDL::Segment(innerMiniDoubletPtr, outerMiniDoubletPtr), innerLowerModule.layer(), SDL::Layer::Endcap);
                }

            }
        }
    }
}

void SDL::Event::createTracklets(TLAlgo algo)
{
    for (auto& tracklet_compatible_layer_pair : SDL::Layer::getListOfTrackletCompatibleLayerPairs())
    {
        int innerLayerIdx = tracklet_compatible_layer_pair.first.first;
        SDL::Layer::SubDet innerLayerSubDet = tracklet_compatible_layer_pair.first.second;
        int outerLayerIdx = tracklet_compatible_layer_pair.second.first;
        SDL::Layer::SubDet outerLayerSubDet = tracklet_compatible_layer_pair.second.second;
        createTrackletsFromTwoLayers(innerLayerIdx, innerLayerSubDet, outerLayerIdx, outerLayerSubDet, algo);
    }
}

// Create tracklets from two layers (inefficient way)
void SDL::Event::createTrackletsFromTwoLayers(int innerLayerIdx, SDL::Layer::SubDet innerLayerSubDet, int outerLayerIdx, SDL::Layer::SubDet outerLayerSubDet, TLAlgo algo)
{
    Layer& innerLayer = getLayer(innerLayerIdx, innerLayerSubDet);
    Layer& outerLayer = getLayer(outerLayerIdx, outerLayerSubDet);

    for (auto& innerSegmentPtr : innerLayer.getSegmentPtrs())
    {
        Segment& innerSegment = *innerSegmentPtr;
        for (auto& outerSegmentPtr : outerLayer.getSegmentPtrs())
        {
            Segment& outerSegment = *outerSegmentPtr;

            if (SDL::Tracklet::isSegmentPairATracklet(innerSegment, outerSegment, algo, logLevel_))
                addTrackletToLowerLayer(SDL::Tracklet(innerSegmentPtr, outerSegmentPtr), innerLayerIdx, innerLayerSubDet);

        }
    }
}


namespace SDL
{
    std::ostream& operator<<(std::ostream& out, const Event& event)
    {

        out << "" << std::endl;
        out << "==============" << std::endl;
        out << "Printing Event" << std::endl;
        out << "==============" << std::endl;
        out << "" << std::endl;

        for (auto& modulePtr : event.modulePtrs_)
        {
            out << modulePtr;
        }

        for (auto& layerPtr : event.layerPtrs_)
        {
            out << layerPtr;
        }

        return out;
    }

    std::ostream& operator<<(std::ostream& out, const Event* event)
    {
        out << *event;
        return out;
    }

}
