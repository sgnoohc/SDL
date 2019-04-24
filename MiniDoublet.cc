#include "MiniDoublet.h"

SDL::MiniDoublet::MiniDoublet()
{
}

SDL::MiniDoublet::~MiniDoublet()
{
}

SDL::MiniDoublet::MiniDoublet(const MiniDoublet& md): lowerHitPtr_(md.lowerHitPtr()), upperHitPtr_(md.upperHitPtr())
{
}

SDL::MiniDoublet::MiniDoublet(SDL::Hit* lowerHitPtr, SDL::Hit* upperHitPtr) : lowerHitPtr_(lowerHitPtr), upperHitPtr_(upperHitPtr)
{
}

SDL::Hit* SDL::MiniDoublet::lowerHitPtr() const
{
    return lowerHitPtr_;
}

SDL::Hit* SDL::MiniDoublet::upperHitPtr() const
{
    return upperHitPtr_;
}

bool SDL::MiniDoublet::isIdxMatched(const MiniDoublet& md) const
{
    if (not lowerHitPtr_->isIdxMatched(*(md.lowerHitPtr())))
        return false;
    if (not upperHitPtr_->isIdxMatched(*(md.upperHitPtr())))
        return false;
    return true;
}

namespace SDL
{
    std::ostream& operator<<(std::ostream& out, const MiniDoublet& md)
    {
        out << "Lower " << md.lowerHitPtr_ << std::endl;;
        out << "Upper " << md.upperHitPtr_;
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const MiniDoublet* md)
    {
        out << *md;
        return out;
    }
}

float SDL::MiniDoublet::dPhiThresholdBarrel(float rt, unsigned int iL)
{
    // =================================================================
    // Various constants
    // =================================================================
    const float kRinv1GeVf = (2.99792458e-3 * 3.8);
    const float k2Rinv1GeVf = kRinv1GeVf / 2.;
    const float ptCut = 1.0;
    const float sinAlphaMax = 0.95;
    // p2Sim.directionT-r2Sim.directionT smearing around the mean computed with ptSim,rSim
    // (1 sigma based on 95.45% = 2sigma at 2 GeV)
    std::array<float, 6> miniMulsPtScaleBarrel {0.0052, 0.0038, 0.0034, 0.0034, 0.0032, 0.0034};
    std::array<float, 5> miniMulsPtScaleEndcap {0.006, 0.006, 0.006, 0.006, 0.006}; //inter/extra-polated from L11 and L13 both roughly 0.006 [larger R have smaller value by ~50%]
    //mean of the horizontal layer position in y; treat this as R below
    std::array<float, 6> miniRminMeanBarrel {21.8, 34.6, 49.6, 67.4, 87.6, 106.8};
    std::array<float, 5> miniRminMeanEndcap {131.4, 156.2, 185.6, 220.3, 261.5};// use z for endcaps

    // =================================================================
    // Computing cut threshold
    // =================================================================
    // float rt = lowerHit.rt();
    // unsigned int iL = lowerModule.layer() - 1;
    const float miniSlope = std::asin(std::min(rt * k2Rinv1GeVf / ptCut, sinAlphaMax));
    const float rLayNominal = miniRminMeanBarrel[iL];
    const float miniPVoff = 0.1 / rLayNominal;
    const float miniMuls = miniMulsPtScaleBarrel[iL] * 3.f / ptCut;
    const bool isTilted = false;
    const bool tiltedOT123 = false;
    const float pixelPSZpitch = 0.15;
    const float miniTilt = isTilted && tiltedOT123 ? /*0.5f * pixelPSZpitch * drdz / sqrt(1.f + drdz * drdz) / miniDelta[iL]*/ : 0;
    const float miniCut = miniSlope + sqrt(pow(miniMuls, 2) + pow(miniPVoff, 2) + pow(miniTilt * miniSlope, 2));

    return miniCut;
}
