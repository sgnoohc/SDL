#ifndef MiniDoublet_h
#define MiniDoublet_h

#include "Hit.h"

namespace SDL
{
    class MiniDoublet
    {
        private:

            // Lower is always the one closer to the beam position
            Hit* lowerHit_;

            // Upper is always the one further away from the beam position
            Hit* upperHit_;

        public:
            MiniDoublet();
            MiniDoublet(const MiniDoublet&);
            MiniDoublet(Hit* lowerHit, Hit* upperHit);
            ~MiniDoublet();

            Hit* lowerHit() const;
            Hit* upperHit() const;

            // cout printing
            friend std::ostream& operator<<(std::ostream& out, const MiniDoublet& md);
            friend std::ostream& operator<<(std::ostream& out, const MiniDoublet* md);
    };
}

#endif
