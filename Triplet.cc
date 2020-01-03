#include "Triplet.h"

SDL::Triplet::Triplet()
{
}

SDL::Triplet::~Triplet()
{
}

SDL::Triplet::Triplet(const Triplet& tl) :
    TrackletBase(tl),
    tlCand(tl.tlCand)
{
}

SDL::Triplet::Triplet(SDL::Segment* innerSegmentPtr, SDL::Segment* outerSegmentPtr) :
    TrackletBase(innerSegmentPtr, outerSegmentPtr)
{
}

bool SDL::Triplet::passesTripletAlgo(SDL::TPAlgo algo) const
{
    // Each algorithm is an enum shift it by its value and check against the flag
    return passAlgo_ & (1 << algo);
}

void SDL::Triplet::runTripletAlgo(SDL::TPAlgo algo, SDL::LogLevel logLevel)
{
    if (algo == SDL::AllComb_TPAlgo)
    {
        runTripletAllCombAlgo();
    }
    else if (algo == SDL::Default_TPAlgo)
    {
        runTripletDefaultAlgo(logLevel);
    }
    else
    {
        SDL::cout << "Warning: Unrecognized segment algorithm!" << algo << std::endl;
        return;
    }
}

void SDL::Triplet::runTripletAllCombAlgo()
{
    passAlgo_ |= (1 << SDL::AllComb_TPAlgo);
}

void SDL::Triplet::runTripletDefaultAlgo(SDL::LogLevel logLevel)
{

    passAlgo_ &= (0 << SDL::Default_TPAlgo);

    if (not (innerSegmentPtr()->hasCommonMiniDoublet(*(outerSegmentPtr()))))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #1 in " << __FUNCTION__ << std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TPAlgo);
        return;
    }
    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << TripletSelection::commonSegment);

    // Check tracklet algo on triplet
    tlCand = SDL::Tracklet(innerSegmentPtr(), outerSegmentPtr());

    // Run tracklet algo
    tlCand.runTrackletDefaultAlgoDeltaBetaOnlyBarrelBarrelBarrelBarrel(SDL::Log_Nothing);

    if (not (tlCand.passesTrackletAlgo(SDL::Default_TLAlgo)))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #2 in " << __FUNCTION__ << std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TPAlgo);
        return;
    }
    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << TripletSelection::tracklet);

    // const std::map<std::string, float>& recovars = tlCand.getRecoVars();
    // for (auto& key : recovars)
    // {
    //     std::cout <<  " key.first: " << key.first <<  std::endl;
    //     std::cout <<  " key.second: " << key.second <<  std::endl;
    // }

    passAlgo_ |= (1 << SDL::Default_TPAlgo);
}

