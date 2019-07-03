#ifndef MiniDoublet_h
#define MiniDoublet_h

#include "MiniDoublet.h"

namespace SDL
{
    class Module;
}

namespace SDL
{
    class Segment
    {
        private:

            // Lower MiniDoublet (lower means one closer to the beam position, i.e. lower "layer")
            MiniDoublet* lowerMiniDoubletPtr_;

            // Upper MiniDoublet (upper means one further away from the beam position, i.e. upper "layer")
            MiniDoublet* upperMiniDoubletPtr_;

        public:
            Segment();
            Segment(const Segment&);
            Segment(MiniDoublet* lowerMiniDoubletPtr, MiniDoublet* upperMiniDoubletPtr);
            ~Segment();

            MiniDoublet* lowerMiniDoubletPtr() const;
            MiniDoublet* upperMiniDoubletPtr() const;

            // cout printing
            friend std::ostream& operator<<(std::ostream& out, const Segment& md);
            friend std::ostream& operator<<(std::ostream& out, const Segment* md);

    };

}

