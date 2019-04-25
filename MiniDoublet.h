#ifndef MiniDoublet_h
#define MiniDoublet_h

#include <array>

#include "Algo.h"
#include "Hit.h"
#include "Module.h"
#include "PrintUtil.h"

namespace SDL
{
    class Module;
}

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

            static float dPhiThreshold(const Hit&, const Module&);
            static bool isMiniDoubletPair(const Hit& lowerHit, const Hit& upperHit, const Module& lowerModule, MDAlgo algo, bool debug=false);
    };
}

#endif
