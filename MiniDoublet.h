#ifndef MiniDoublet_h
#define MiniDoublet_h

#include <array>
#include <tuple>

#include "Constants.h"
#include "Algo.h"
#include "PrintUtil.h"
#include "MathUtil.h"
#include "EndcapGeometry.h"
#include "TiltedGeometry.h"
#include "Hit.h"
#include "Module.h"

namespace SDL
{
    class Module;
    class Hit;
}

namespace SDL
{
    class MiniDoublet
    {
        private:

            // TODO: Rename lower and upper as inner and outer

            // Lower is always the one closer to the beam position
            Hit* lowerHitPtr_;

            // Upper is always the one further away from the beam position
            Hit* upperHitPtr_;

            // Anchor hit is either Pixel hit (if available) or lower hit
            Hit* anchorHitPtr_;

            // Bits to flag whether this mini-doublet passes some algorithm
            int passAlgo_;

            int passAlgoDZ_;
            int passAlgoDPhi_;
            int passAlgoDrt_;
            int passAlgoDPhiChange_;

            // Some mini-doublet related reconstructon variables
            Hit lowerShiftedHit_;
            Hit upperShiftedHit_;
            float dz_;
            float shiftedDz_;
            float dphi_;
            float dphi_noshift_;
            float dphichange_;
            float dphichange_noshift_;

            //cuts from outside
            float dzCut_;
            float dphiCut_;
            float dphichangeCut_;
            float drtCut_;

        public:
            MiniDoublet();
            MiniDoublet(const MiniDoublet&);
            MiniDoublet(Hit* lowerHit, Hit* upperHit);
            ~MiniDoublet();

            Hit* lowerHitPtr() const;
            Hit* upperHitPtr() const;
            Hit* anchorHitPtr() const;
            const int& getPassAlgo() const;
            const int& getPassAlgoDZ() const;
            const int& getPassAlgoDrt() const;
            const int& passAlgoDrt() const;
            const int& getPassAlgoDPhi() const;
            const int& getPassAlgoDPhiChange() const;
            const Hit& getLowerShiftedHit() const;
            const Hit& getUpperShiftedHit() const;
            const float& getDz() const;
            const float& getShiftedDz() const;
            const float& getDeltaPhi() const;
            const float& getDeltaPhiChange() const;
            const float& getDeltaPhiNoShift() const;
            const float& getDeltaPhiChangeNoShift() const;

            void setAnchorHit();
            void setLowerShiftedHit(float, float, float, int=-1);
            void setUpperShiftedHit(float, float, float, int=-1);
            void setDz(float);
            void setShiftedDz(float);
            void setDeltaPhi(float);
            void setDeltaPhiChange(float);
            void setDeltaPhiNoShift(float);
            void setDeltaPhiChangeNoShift(float);

            const float& getdZCut() const;
            const float& getdphiCut() const;
            const float& getdphichangeCut() const;
            const float& getdrtCut() const;

            void setdzCut(float);
            void setdphiCut(float);
            void setdphichangeCut(float);
            void setdrtCut(float);

            // return whether it passed the algorithm
            bool passesMiniDoubletAlgo(MDAlgo algo) const;
            bool passesMiniDoubletAlgo_dZ(MDAlgo algo) const;
            bool passesMiniDoubletAlgo_drt(MDAlgo algo) const;
            bool passesMiniDoubletAlgo_dPhi(MDAlgo algo) const;
            bool passesMiniDoubletAlgo_dPhiChange(MDAlgo algo) const;

            // The function to run mini-doublet algorithm on a mini-doublet candidate
            void runMiniDoubletAlgo(MDAlgo algo, SDL::LogLevel logLevel=SDL::Log_Nothing);

            // The following algorithm does nothing and accepts the mini-doublet
            void runMiniDoubletAllCombAlgo();

            // The default algorithms;
            void runMiniDoubletDefaultAlgo(SDL::LogLevel logLevel);
            void runMiniDoubletDefaultAlgoBarrel(SDL::LogLevel logLevel);
            void runMiniDoubletDefaultAlgoEndcap(SDL::LogLevel logLevel);

            bool isIdxMatched(const MiniDoublet&) const;
            bool isAnchorHitIdxMatched(const MiniDoublet&) const;

            // cout printing
            friend std::ostream& operator<<(std::ostream& out, const MiniDoublet& md);
            friend std::ostream& operator<<(std::ostream& out, const MiniDoublet* md);

            // The math for the threshold cut value to apply between hits for mini-doublet
            // The main idea is to be consistent with 1 GeV minimum pt
            // Some residual effects such as tilt, multiple scattering, beam spots are considered
            static float dPhiThreshold(const Hit&, const Module&);

            // The math for shifting the pixel hit up or down along the PS module orientation (deprecated)
            static float fabsdPhiPixelShift(const Hit& lowerHit, const Hit& upperHit, const Module& lowerModule, SDL::LogLevel logLevel=SDL::Log_Nothing);

            // The math for shifting the strip hit up or down along the PS module orientation (deprecated)
            static float fabsdPhiStripShift(const Hit& lowerHit, const Hit& upperHit, const Module& lowerModule, SDL::LogLevel logLevel=SDL::Log_Nothing);

            // The math for shifting the strip hit up or down along the PS module orientation, returns new x, y and z position
            static std::tuple<float, float, float> shiftStripHits(const Hit& lowerHit, const Hit& upperHit, const Module& lowerModule, SDL::LogLevel logLevel=SDL::Log_Nothing);

            // The function to actually determine whether a pair of hits is a reco-ed mini doublet or not
            static bool isHitPairAMiniDoublet(const Hit& lowerHit, const Hit& upperHit, const Module& lowerModule, MDAlgo algo, SDL::LogLevel logLevel=SDL::Log_Nothing);

            // Condition that a module falls into "barrel"-logic of the mini-doublet algorithm
            static bool useBarrelLogic(const Module& lowerModule);

            // The function to determine transition region for inner most tilted layer
            static bool isNormalTiltedModules(const Module& lowerModule);

    };
}

#endif
