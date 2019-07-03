#include "Segment.h"

SDL::Segment::Segment()
{
}

SDL::Segment::~Segment()
{
}

SDL::Segment::Segment(const Segment& sg) : lowerMiniDoubletPtr_(sg.lowerMiniDoubletPtr()), upperMiniDoubletPtr_(sg.upperMiniDoubletPtr())
{
}

SDL::Segment::Segment(SDL::MiniDoublet* lowerMiniDoubletPtr, SDL::MiniDoublet* upperMiniDoubletPtr) : lowerMiniDoubletPtr_(lowerMiniDoubletPtr), upperMiniDoubletPtr_(upperMiniDoubletPtr)
{
}

SDL::MiniDoublet* SDL::Segment::lowerMiniDoubletPtr() const
{
    return lowerMiniDoubletPtr_;
}

SDL::MiniDoublet* SDL::Segment::upperMiniDoubletPtr() const
{
    return upperMiniDoubletPtr_;
}

namespace SDL
{
    std::ostream& operator<<(std::ostream& out, const Segment& sg)
    {
        out << "Lower MiniDoublet " << sg.lowerMiniDoubletPtr_ << std::endl;;
        out << "Upper MiniDoublet " << sg.upperMiniDoubletPtr_;
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const Segment* sg)
    {
        out << *md;
        return out;
    }
}

