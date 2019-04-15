#include "MiniDoublet.h"

SDL::MiniDoublet::MiniDoublet()
{
}

SDL::MiniDoublet::~MiniDoublet()
{
}

SDL::MiniDoublet::MiniDoublet(const MiniDoublet& md): lowerHit_(md.lowerHit()), upperHit_(md.upperHit())
{
}

SDL::MiniDoublet::MiniDoublet(SDL::Hit* lowerHit, SDL::Hit* upperHit) : lowerHit_(lowerHit), upperHit_(upperHit)
{
}

SDL::Hit* SDL::MiniDoublet::lowerHit() const
{
    return lowerHit_;
}

SDL::Hit* SDL::MiniDoublet::upperHit() const
{
    return upperHit_;
}

bool SDL::MiniDoublet::isMatched(const MiniDoublet& md) const
{
    if (not lowerHit_->isMatched(*(md.lowerHit())))
        return false;
    if (not upperHit_->isMatched(*(md.upperHit())))
        return false;
    return true;
}

namespace SDL
{
    std::ostream& operator<<(std::ostream& out, const MiniDoublet& md)
    {
        out << "Lower " << md.lowerHit_ << std::endl;;
        out << "Upper " << md.upperHit_;
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const MiniDoublet* md)
    {
        out << *md;
        return out;
    }
}
