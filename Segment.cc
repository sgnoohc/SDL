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
        // const Module& outerLowerModule = outerMiniDoublet.lowerHitPtr()->getModule();
        // Port your favorite segment formation algorithm code here
        // Case 1: Barrel - Barrel
        // if (innerLowerModule.subdet() == SDL::Module::Barrel and outerLowerModule.subdet() == SDL::Module::Barrel)
        if (innerLowerModule.subdet() == SDL::Module::Barrel)
        {
            return isMiniDoubletPairASegmentBarrel(innerMiniDoublet, outerMiniDoublet, algo, logLevel);
        }
        else // if (innerLowerModule.subdet() == SDL::Module::Endcap)
        {
            return isMiniDoubletPairASegmentEndcap(innerMiniDoublet, outerMiniDoublet, algo, logLevel);
        }
        return false;
    }
    else
    {
        SDL::cout << "Warning: Unrecognized segment algorithm!" << algo << std::endl;
        return false;
    }
}

bool SDL::Segment::isMiniDoubletPairASegmentBarrel(const MiniDoublet& innerMiniDoublet, const MiniDoublet& outerMiniDoublet, SGAlgo algo, SDL::LogLevel logLevel)
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

    // Get the relevant anchor hits
    const Hit& innerMiniDoubletAnchorHit = innerLowerModule.moduleType() == SDL::Module::PS ? ( innerLowerModule.moduleLayerType() == SDL::Module::Pixel ? *innerMiniDoublet.lowerHitPtr() : *innerMiniDoublet.upperHitPtr()): *innerMiniDoublet.lowerHitPtr();
    const Hit& outerMiniDoubletAnchorHit = outerLowerModule.moduleType() == SDL::Module::PS ? ( outerLowerModule.moduleLayerType() == SDL::Module::Pixel ? *outerMiniDoublet.lowerHitPtr() : *outerMiniDoublet.upperHitPtr()): *outerMiniDoublet.lowerHitPtr();

    // MiniDoublet information
    float innerMiniDoubletAnchorHitRt = innerMiniDoubletAnchorHit.rt();
    float outerMiniDoubletAnchorHitRt = outerMiniDoubletAnchorHit.rt();
    float innerMiniDoubletAnchorHitZ = innerMiniDoubletAnchorHit.z();
    float outerMiniDoubletAnchorHitZ = outerMiniDoubletAnchorHit.z();

    const float sdSlope = std::asin(std::min(outerMiniDoubletAnchorHitRt * k2Rinv1GeVf / ptCut, sinAlphaMax));
    const float sdPVoff = 0.1f / outerMiniDoubletAnchorHitRt;
    const float dzDrtScale = std::tan(sdSlope) / sdSlope; //FIXME: need approximate value
    const float pixelPSZpitch = 0.15;
    const float strip2SZpitch = 5.0;

    const float zGeom = innerLowerModule.layer() <= 2 ? 2.f * pixelPSZpitch : 2.f * strip2SZpitch; //twice the macro-pixel or strip size

    float zLo = innerMiniDoubletAnchorHitZ + (innerMiniDoubletAnchorHitZ - deltaZLum) * (outerMiniDoubletAnchorHitRt / innerMiniDoubletAnchorHitRt - 1.f) * (innerMiniDoubletAnchorHitZ > 0.f ? 1.f : dzDrtScale) - zGeom; //slope-correction only on outer end
    float zHi = innerMiniDoubletAnchorHitZ + (innerMiniDoubletAnchorHitZ + deltaZLum) * (outerMiniDoubletAnchorHitRt / innerMiniDoubletAnchorHitRt - 1.f) * (innerMiniDoubletAnchorHitZ < 0.f ? 1.f : dzDrtScale) + zGeom;

    // Cut #1: Z compatibility
    if (not (outerMiniDoubletAnchorHitZ >= zLo and outerMiniDoubletAnchorHitZ <= zHi))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #1 in " << __FUNCTION__ << std::endl;
            std::cout <<  " zLo: " << zLo <<  " outerMiniDoubletAnchorHitZ: " << outerMiniDoubletAnchorHitZ <<  " zHi: " << zHi <<  std::endl;
        }
        return false;
    }

    const float sdCut = sdSlope + sqrt(sdMuls * sdMuls + sdPVoff * sdPVoff);
    const float deltaPhi = innerMiniDoubletAnchorHit.deltaPhi(outerMiniDoubletAnchorHit);

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

    const float deltaPhiChange = innerMiniDoubletAnchorHit.deltaPhiChange(outerMiniDoubletAnchorHit);

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

bool SDL::Segment::isMiniDoubletPairASegmentEndcap(const MiniDoublet& innerMiniDoublet, const MiniDoublet& outerMiniDoublet, SGAlgo algo, SDL::LogLevel logLevel)
{

    const Module& innerLowerModule = innerMiniDoublet.lowerHitPtr()->getModule();
    const Module& outerLowerModule = outerMiniDoublet.lowerHitPtr()->getModule();

    // Constants
    const float kRinv1GeVf = (2.99792458e-3 * 3.8);
    const float k2Rinv1GeVf = kRinv1GeVf / 2.;
    const float ptCut = 1.0;
    const float sinAlphaMax = 0.95;
    const float deltaZLum = 15.f;
    // std::array<float, 6> miniMulsPtScaleBarrel {0.0052, 0.0038, 0.0034, 0.0034, 0.0032, 0.0034};
    // std::array<float, 5> miniMulsPtScaleEndcap {0.006, 0.006, 0.006, 0.006, 0.006}; //inter/extra-polated from L11 and L13 both roughly 0.006 [larger R have smaller value by ~50%]
    // const float sdMuls = miniMulsPtScaleEndcap[innerLowerModule.layer()] * 3.f / ptCut * 2.f;//will need a better guess than x2?

    // Get the relevant anchor hits
    const Hit& innerMiniDoubletAnchorHit = innerLowerModule.moduleType() == SDL::Module::PS ? ( innerLowerModule.moduleLayerType() == SDL::Module::Pixel ? *innerMiniDoublet.lowerHitPtr() : *innerMiniDoublet.upperHitPtr()): *innerMiniDoublet.lowerHitPtr();
    const Hit& outerMiniDoubletAnchorHit = outerLowerModule.moduleType() == SDL::Module::PS ? ( outerLowerModule.moduleLayerType() == SDL::Module::Pixel ? *outerMiniDoublet.lowerHitPtr() : *outerMiniDoublet.upperHitPtr()): *outerMiniDoublet.lowerHitPtr();

    // MiniDoublet information
    float innerMiniDoubletAnchorHitRt = innerMiniDoubletAnchorHit.rt();
    float outerMiniDoubletAnchorHitRt = outerMiniDoubletAnchorHit.rt();
    float innerMiniDoubletAnchorHitZ = innerMiniDoubletAnchorHit.z();
    float outerMiniDoubletAnchorHitZ = outerMiniDoubletAnchorHit.z();

    const float sdSlope = std::asin(std::min(outerMiniDoubletAnchorHitRt * k2Rinv1GeVf / ptCut, sinAlphaMax));
    // const float sdPVoff = 0.1f / outerMiniDoubletAnchorHitRt;
    // const float dzDrtScale = std::tan(sdSlope) / sdSlope; //FIXME: need approximate value
    const float pixelPSZpitch = 0.15;
    const float strip2SZpitch = 5.0;
    const float disks2SMinRadius = 60.f;

    // const float zGeom = innerLowerModule.layer() <= 2 ? 2.f * pixelPSZpitch : 2.f * strip2SZpitch; //twice the macro-pixel or strip size

    const float rtGeom = (innerMiniDoubletAnchorHitRt < disks2SMinRadius && outerMiniDoubletAnchorHitRt < disks2SMinRadius ? 2.f * pixelPSZpitch
            : (innerMiniDoubletAnchorHitRt < disks2SMinRadius || outerMiniDoubletAnchorHitRt < disks2SMinRadius) ? (pixelPSZpitch + strip2SZpitch)
            : 2.f * strip2SZpitch);

    // float zLo = innerMiniDoubletAnchorHitZ + (innerMiniDoubletAnchorHitZ - deltaZLum) * (outerMiniDoubletAnchorHitRt / innerMiniDoubletAnchorHitRt - 1.f) * (innerMiniDoubletAnchorHitZ > 0.f ? 1.f : dzDrtScale) - zGeom; //slope-correction only on outer end
    // float zHi = innerMiniDoubletAnchorHitZ + (innerMiniDoubletAnchorHitZ + deltaZLum) * (outerMiniDoubletAnchorHitRt / innerMiniDoubletAnchorHitRt - 1.f) * (innerMiniDoubletAnchorHitZ < 0.f ? 1.f : dzDrtScale) + zGeom;

    // Cut #0: preliminary cut (if the combo is between negative and positive don't even bother...)
    if (innerMiniDoubletAnchorHitZ * outerMiniDoubletAnchorHitZ < 0)
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #0 in " << __FUNCTION__ << std::endl;
            std::cout <<  " innerMiniDoubletAnchorHitZ: " << innerMiniDoubletAnchorHitZ <<  " outerMiniDoubletAnchorHitZ: " << outerMiniDoubletAnchorHitZ <<  std::endl;
        }
        return false; //do not even accumulate stats for wrong side combinations
    }

    const float dz = outerMiniDoubletAnchorHitZ - innerMiniDoubletAnchorHitZ;

    // Cut #1: Z compatibility
    const float dLum = std::copysign(deltaZLum, innerMiniDoubletAnchorHitZ);
    const float drtDzScale = sdSlope / std::tan(sdSlope); //FIXME: need approximate value
    float rtLo = std::max(innerMiniDoubletAnchorHitRt * (1.f + dz / (innerMiniDoubletAnchorHitZ + dLum) * drtDzScale) - rtGeom, innerMiniDoubletAnchorHitRt - 0.5f * rtGeom); //rt should increase
    float rtHi = innerMiniDoubletAnchorHitRt * (outerMiniDoubletAnchorHitZ - dLum) / (innerMiniDoubletAnchorHitZ - dLum) + rtGeom; //dLum for luminous; rGeom for measurement size; no tanTheta_loc(pt) correction
    if (not (outerMiniDoubletAnchorHitRt >= rtLo and outerMiniDoubletAnchorHitRt <= rtHi))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #1 in " << __FUNCTION__ << std::endl;
            std::cout <<  " rtLo: " << rtLo <<  " outerMiniDoubletAnchorHitRt: " << outerMiniDoubletAnchorHitRt <<  " rtHi: " << rtHi <<  std::endl;
        }
        return false;
    }

    // Cut #2: dPhi compatibility
    // const float sdLum = deltaZLum / std::abs(innerMiniDoubletAnchorHitZ);
    // const float sdCut = sdSlope + sqrt(sdMuls * sdMuls + sdPVoff * sdPVoff + sdLum * sdLum);
    const float sdCut = sdSlope;
    // const float sdCut = sdSlope + sqrt(sdMuls * sdMuls + sdPVoff * sdPVoff);
    const float dPhiPos = innerMiniDoubletAnchorHit.deltaPhi(outerMiniDoubletAnchorHit);
    if (not (std::abs(dPhiPos) <= sdCut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #2 in " << __FUNCTION__ << std::endl;
            std::cout <<  " dPhiPos: " << dPhiPos <<  " sdCut: " << sdCut <<  std::endl;
        }
        return false;
    }

    // Cut #3: dPhi compatibility
    // const float dPhiChange = innerMiniDoubletAnchorHit.deltaPhiChange(outerMiniDoubletAnchorHit); // NOTE When using the full r3 coordinate (this was turned off in slava's code)
    const float dzFrac = dz / innerMiniDoubletAnchorHitZ;
    const float dPhiChange = dPhiPos / dzFrac * (1.f + dzFrac);
    if (not (std::abs(dPhiChange) <= sdCut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #3 in " << __FUNCTION__ << std::endl;
            std::cout <<  " dPhiChange: " << dPhiChange <<  " sdCut: " << sdCut <<  std::endl;
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

