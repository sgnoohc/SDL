#ifndef MiniDoublet_h
#define MiniDoublet_h

#include "Hit.h"
#include <array>

namespace SDL
{
    class MiniDoublet
    {
        private:

            // Lower is always the one closer to the beam position
            Hit* lowerHitPtr_;

            // Upper is always the one further away from the beam position
            Hit* upperHitPtr_;

        public:
            MiniDoublet();
            MiniDoublet(const MiniDoublet&);
            MiniDoublet(Hit* lowerHit, Hit* upperHit);
            ~MiniDoublet();

            Hit* lowerHitPtr() const;
            Hit* upperHitPtr() const;

            bool isIdxMatched(const MiniDoublet&) const;

            // cout printing
            friend std::ostream& operator<<(std::ostream& out, const MiniDoublet& md);
            friend std::ostream& operator<<(std::ostream& out, const MiniDoublet* md);

            static float dPhiThresholdBarrel(float rt, unsigned int iL);
    };
}

#endif
