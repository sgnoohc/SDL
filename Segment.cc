#include "Segment.h"

SDL::Segment::Segment()
{
}

SDL::Segment::~Segment()
{
}

SDL::Segment::Segment(const Segment& sg) :
    innerMiniDoubletPtr_(sg.innerMiniDoubletPtr()),
    outerMiniDoubletPtr_(sg.outerMiniDoubletPtr())
{
}

SDL::Segment::Segment(SDL::MiniDoublet* innerMiniDoubletPtr, SDL::MiniDoublet* outerMiniDoubletPtr) :
    innerMiniDoubletPtr_(innerMiniDoubletPtr),
    outerMiniDoubletPtr_(outerMiniDoubletPtr)
{
}

SDL::MiniDoublet* SDL::Segment::innerMiniDoubletPtr() const
{
    return innerMiniDoubletPtr_;
}

SDL::MiniDoublet* SDL::Segment::outerMiniDoubletPtr() const
{
    return outerMiniDoubletPtr_;
}

bool SDL::Segment::isIdxMatched(const Segment& md) const
{
    if (not innerMiniDoubletPtr_->isIdxMatched(*(md.innerMiniDoubletPtr())))
        return false;
    if (not outerMiniDoubletPtr_->isIdxMatched(*(md.outerMiniDoubletPtr())))
        return false;
    return true;
}

bool SDL::Segment::isMiniDoubletPairASegment(const MiniDoublet& innerMiniDoublet, const MiniDoublet& outerMiniDoublet, SGAlgo algo, SDL::LogLevel logLevel)
{
    // If the algorithm is "do all combination" (e.g. used for efficiency calculation)
    if (algo == SDL::AllComb_SGAlgo)
    {
        return true;
    }
    else if (algo == SDL::Default_SGAlgo)
    {
        const Module& innerLowerModule = innerMiniDoublet.lowerHitPtr()->getModule();
        const Module& outerLowerModule = outerMiniDoublet.lowerHitPtr()->getModule();
        // Port your favorite segment formation algorithm code here
        // Case 1: Barrel - Barrel
        if (innerLowerModule.subdet() == SDL::Module::Barrel and outerLowerModule.subdet() == SDL::Module::Barrel)
        {
            return isMiniDoubletPairASegmentBarrelBarrel(innerMiniDoublet, outerMiniDoublet, algo, logLevel);
        }
        return false;
    }
    else
    {
        SDL::cout << "Warning: Unrecognized segment algorithm!" << algo << std::endl;
        return false;
    }
}

bool SDL::Segment::isMiniDoubletPairASegmentBarrelBarrel(const MiniDoublet& innerMiniDoublet, const MiniDoublet& outerMiniDoublet, SGAlgo algo, SDL::LogLevel logLevel)
{

    const Module& innerLowerModule = innerMiniDoublet.lowerHitPtr()->getModule();
    const Module& outerLowerModule = outerMiniDoublet.lowerHitPtr()->getModule();

    // Constants
    const float kRinv1GeVf = (2.99792458e-3 * 3.8);
    const float k2Rinv1GeVf = kRinv1GeVf / 2.;
    const float ptCut = 1.0;
    const float sinAlphaMax = 0.95;
    const float deltaZLum = 15.f;
    std::array<float, 6> miniMulsPtScaleBarrel {0.0052, 0.0038, 0.0034, 0.0034, 0.0032, 0.0034};
    // std::array<float, 5> miniMulsPtScaleEndcap {0.006, 0.006, 0.006, 0.006, 0.006}; //inter/extra-polated from L11 and L13 both roughly 0.006 [larger R have smaller value by ~50%]
    const float sdMuls = miniMulsPtScaleBarrel[innerLowerModule.layer()] * 3.f / ptCut * 2.f;//will need a better guess than x2?

    // MiniDoublet information
    float innerMiniDoubletLowerHitRt = innerMiniDoublet.lowerHitPtr()->rt();
    float outerMiniDoubletLowerHitRt = outerMiniDoublet.lowerHitPtr()->rt();
    float innerMiniDoubletLowerHitZ = innerMiniDoublet.lowerHitPtr()->z();
    float outerMiniDoubletLowerHitZ = outerMiniDoublet.lowerHitPtr()->z();

    Hit& innerMiniDoubletLowerHit = *innerMiniDoublet.lowerHitPtr();
    Hit& outerMiniDoubletLowerHit = *outerMiniDoublet.lowerHitPtr();

    const float sdSlope = std::asin(std::min(outerMiniDoubletLowerHitRt * k2Rinv1GeVf / ptCut, sinAlphaMax));
    const float sdPVoff = 0.1f / outerMiniDoubletLowerHitRt;
    const float dzDrtScale = std::tan(sdSlope) / sdSlope; //FIXME: need approximate value
    const float pixelPSZpitch = 0.15;
    const float strip2SZpitch = 5.0;

    const float zGeom = innerLowerModule.layer() <= 2 ? 2.f * pixelPSZpitch : 2.f * strip2SZpitch; //twice the macro-pixel or strip size

    float zLo = innerMiniDoubletLowerHitZ + (innerMiniDoubletLowerHitZ - deltaZLum) * (outerMiniDoubletLowerHitRt / innerMiniDoubletLowerHitRt - 1.f) * (innerMiniDoubletLowerHitZ > 0.f ? 1.f : dzDrtScale) - zGeom; //slope-correction only on outer end
    float zHi = innerMiniDoubletLowerHitZ + (innerMiniDoubletLowerHitZ + deltaZLum) * (outerMiniDoubletLowerHitRt / innerMiniDoubletLowerHitRt - 1.f) * (innerMiniDoubletLowerHitZ < 0.f ? 1.f : dzDrtScale) + zGeom;

    // Cut #1: Z compatibility
    if (not (outerMiniDoubletLowerHitZ > zLo and outerMiniDoubletLowerHitZ < zHi))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #1 in " << __FUNCTION__ << std::endl;
            std::cout <<  " zLo: " << zLo <<  " outerMiniDoubletLowerHitZ: " << outerMiniDoubletLowerHitZ <<  " zHi: " << zHi <<  std::endl;
        }
        return false;
    }

    const float sdCut = sdSlope + sqrt(sdMuls * sdMuls + sdPVoff * sdPVoff);
    const float deltaPhi = innerMiniDoubletLowerHit.deltaPhi(outerMiniDoubletLowerHit);

    // Cut #2: phi differences between the two minidoublets
    if (not (std::abs(deltaPhi) <= sdCut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #2 in " << __FUNCTION__ << std::endl;
            std::cout <<  " deltaPhi: " << deltaPhi <<  " sdCut: " << sdCut <<  std::endl;
        }
        return false;
    }

    const float deltaPhiChange = innerMiniDoubletLowerHit.deltaPhiChange(outerMiniDoubletLowerHit);

    // Cut #3: phi change between the two minidoublets
    if (not (std::abs(deltaPhiChange) <= sdCut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #3 in " << __FUNCTION__ << std::endl;
            std::cout <<  " deltaPhiChange: " << deltaPhiChange <<  " sdCut: " << sdCut <<  std::endl;
        }
        return false;
    }

    return true;
}

namespace SDL
{
    std::ostream& operator<<(std::ostream& out, const Segment& sg)
    {
        out << "Segment()" << std::endl;
        out << "    Lower " << sg.innerMiniDoubletPtr_ << std::endl;;
        out << "    Upper " << sg.outerMiniDoubletPtr_;
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const Segment* sg)
    {
        out << *sg;
        return out;
    }
}

