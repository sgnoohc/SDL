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
