#ifndef MiniDoublet_h
#define MiniDoublet_h

#include <array>

#include "Algo.h"
#include "Hit.h"
#include "Module.h"
#include "PrintUtil.h"
#include "EndcapGeometry.h"

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

            // The math for the threshold cut value to apply between hits for mini-doublet
            // The main idea is to be consistent with 1 GeV minimum pt
            // Some residual effects such as tilt, multiple scattering, beam spots are considered
            static float dPhiThreshold(const Hit&, const Module&);

            // The math for shifting the pixel hit up or down along the PS module orientation
            static float fabsdPhiPixelShift(const Hit& lowerHit, const Hit& upperHit, const Module& lowerModule, SDL::LogLevel logLevel=SDL::Log_Nothing);

            // The function to actually determine whether a hit is a reco-ed mini doublet or not
            static bool isMiniDoubletPair(const Hit& lowerHit, const Hit& upperHit, const Module& lowerModule, MDAlgo algo, SDL::LogLevel logLevel=SDL::Log_Nothing);
    };
}

#endif
