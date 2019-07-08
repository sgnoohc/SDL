#include "Segment.h"

SDL::Segment::Segment()
{
}

SDL::Segment::~Segment()
{
}

SDL::Segment::Segment(const Segment& sg) :
    innerMiniDoubletPtr_(sg.innerMiniDoubletPtr()),
    outerMiniDoubletPtr_(sg.outerMiniDoubletPtr())
{
}

SDL::Segment::Segment(SDL::MiniDoublet* innerMiniDoubletPtr, SDL::MiniDoublet* outerMiniDoubletPtr) :
    innerMiniDoubletPtr_(innerMiniDoubletPtr),
    outerMiniDoubletPtr_(outerMiniDoubletPtr)
{
}

SDL::MiniDoublet* SDL::Segment::innerMiniDoubletPtr() const
{
    return innerMiniDoubletPtr_;
}

SDL::MiniDoublet* SDL::Segment::outerMiniDoubletPtr() const
{
    return outerMiniDoubletPtr_;
}

bool SDL::Segment::isMiniDoubletPairASegment(
        const MiniDoublet& innerMiniDoublet,
        const MiniDoublet& outerMiniDoublet,
        const Module& innerLowerModule,
        const Module& innerOuterModule,
        SGAlgo algo,
        SDL::LogLevel logLevel)
{
    // If the algorithm is "do all combination" (e.g. used for efficiency calculation)
    if (algo == SDL::AllComb_SGAlgo)
    {
        return true;
    }
    else if (algo == SDL::Default_SGAlgo)
    {
        // Port your favorite segment formation algorithm code here
        return true;
    }
    else
    {
        SDL::cout << "Warning: Unrecognized segment algorithm!" << algo << std::endl;
        return false;
    }
}

namespace SDL
{
    std::ostream& operator<<(std::ostream& out, const Segment& sg)
    {
        out << "Segment()" << std::endl;
        out << "    Lower " << sg.innerMiniDoubletPtr_ << std::endl;;
        out << "    Upper " << sg.outerMiniDoubletPtr_;
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const Segment* sg)
    {
        out << *sg;
        return out;
    }
}

