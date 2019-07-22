#include "Tracklet.h"

SDL::Tracklet::Tracklet()
{
}

SDL::Tracklet::~Tracklet()
{
}

SDL::Tracklet::Tracklet(const Tracklet& tl) :
    innerSegmentPtr_(tl.innerSegmentPtr()),
    outerSegmentPtr_(tl.outerSegmentPtr())
{
}

SDL::Tracklet::Tracklet(SDL::Segment* innerSegmentPtr, SDL::Segment* outerSegmentPtr) :
    innerSegmentPtr_(innerSegmentPtr),
    outerSegmentPtr_(outerSegmentPtr)
{
}

SDL::Segment* SDL::Tracklet::innerSegmentPtr() const
{
    return innerSegmentPtr_;
}

SDL::Segment* SDL::Tracklet::outerSegmentPtr() const
{
    return outerSegmentPtr_;
}

bool SDL::Tracklet::isIdxMatched(const Tracklet& md) const
{
    if (not innerSegmentPtr_->isIdxMatched(*(md.innerSegmentPtr())))
        return false;
    if (not outerSegmentPtr_->isIdxMatched(*(md.outerSegmentPtr())))
        return false;
    return true;
}

bool SDL::Tracklet::isSegmentPairATracklet(const Segment& innerSegment, const Segment& outerSegment, TLAlgo algo, SDL::LogLevel logLevel)
{
    // If the algorithm is "do all combination" (e.g. used for efficiency calculation)
    if (algo == SDL::AllComb_TLAlgo)
    {
        return true;
    }
    else if (algo == SDL::Default_TLAlgo)
    {
        return false;
    }
    else
    {
        SDL::cout << "Warning: Unrecognized segment algorithm!" << algo << std::endl;
        return false;
    }
}

bool SDL::Tracklet::isSegmentPairATrackletBarrel(const Segment& innerSegment, const Segment& outerSegment, TLAlgo algo, SDL::LogLevel logLevel)
{
    return false;
}

bool SDL::Tracklet::isSegmentPairATrackletEndcap(const Segment& innerSegment, const Segment& outerSegment, TLAlgo algo, SDL::LogLevel logLevel)
{
    return false;
}

namespace SDL
{
    std::ostream& operator<<(std::ostream& out, const Tracklet& tl)
    {
        out << "Tracklet()" << std::endl;
        out << "    Lower " << tl.innerSegmentPtr_ << std::endl;;
        out << "    Upper " << tl.outerSegmentPtr_;
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const Tracklet* tl)
    {
        out << *tl;
        return out;
    }
}

