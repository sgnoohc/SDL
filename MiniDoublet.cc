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

float SDL::MiniDoublet::dPhiThreshold(const SDL::Hit& lowerHit, const SDL::Module& module)
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
    std::array<float, 6> miniRminMeanBarrel {21.8, 34.6, 49.6, 67.4, 87.6, 106.8}; // TODO: Update this with newest geometry
    std::array<float, 5> miniRminMeanEndcap {131.4, 156.2, 185.6, 220.3, 261.5};// use z for endcaps // TODO: Update this with newest geometry

    // =================================================================
    // Computing some components that make up the cut threshold
    // =================================================================
    float rt = lowerHit.rt();
    unsigned int iL = module.layer() - 1;
    const float miniSlope = std::asin(std::min(rt * k2Rinv1GeVf / ptCut, sinAlphaMax));
    const float rLayNominal = module.subdet() == SDL::Module::Barrel ? miniRminMeanBarrel[iL] : miniRminMeanEndcap[iL];
    const float miniPVoff = 0.1 / rLayNominal;
    const float miniMuls = module.subdet() == SDL::Module::Barrel ? miniMulsPtScaleBarrel[iL] * 3.f / ptCut : miniMulsPtScaleEndcap[iL] * 3.f / ptCut;
    const bool isTilted = false;
    const bool tiltedOT123 = false;
    // const float pixelPSZpitch = 0.15;
    const float miniTilt = isTilted && tiltedOT123 ? /*0.5f * pixelPSZpitch * drdz / sqrt(1.f + drdz * drdz) / miniDelta[iL]*/ 0 : 0; // TODO: Tilt is properly not accounted for
    // Compute luminous region requirement for endcap
    const float deltaZLum = 15.f;
    const float miniLum = deltaZLum / std::abs(lowerHit.z());

    // =================================================================
    // Return the threshold value
    // =================================================================
    // Following condition is met if the module is central and flatly lying
    if (module.subdet() == SDL::Module::Barrel and module.side() == SDL::Module::Center)
    {
        return miniSlope + sqrt(pow(miniMuls, 2) + pow(miniPVoff, 2));
    }
    // Following condition is met if the module is central and tilted
    else if (module.subdet() == SDL::Module::Barrel and module.side() != SDL::Module::Center)
    {
        return miniSlope + sqrt(pow(miniMuls, 2) + pow(miniPVoff, 2) + pow(miniTilt * miniSlope, 2));
    }
    // If not barrel, it is Endcap
    else 
    {
        return miniSlope + sqrt(pow(miniMuls, 2) + pow(miniPVoff, 2) + pow(miniLum, 2));
    }

}

bool SDL::MiniDoublet::isMiniDoubletPair(const SDL::Hit& lowerHit, const SDL::Hit& upperHit, const SDL::Module& lowerModule, SDL::MDAlgo algo, SDL::LogLevel logLevel)
{
    // If the algorithm is "do all combination" (e.g. used for efficiency calculation)
    if (algo == SDL::AllComb_MDAlgo)
    {
        return true;
    }
    // If the algorithm is default
    else if (algo == SDL::Default_MDAlgo)
    {

        // There are several cuts applied to possible hit pairs, and if the hit pairs passes the cut, it is considered as mini-doublet.
        // The logic is split into two parts, when it's for barrel and endcap.
        // Internally, if it is in barrel, there is some subtle difference depending on whether it was tilted or not tilted.
        // The difference is encoded in the "SDL::MiniDoublet::dPhiThreshold()" function

        // If barrel, apply cuts for barrel mini-doublets
        if (lowerModule.subdet() == SDL::Module::Barrel)
        {

            // Cut #1: The dz difference
            // Ref to original code: https://github.com/slava77/cms-tkph2-ntuple/blob/184d2325147e6930030d3d1f780136bc2dd29ce6/doubletAnalysis.C#L3067
            const float dzCut = 10.f; // Could be tighter for PS modules
            float dz = std::abs(lowerHit.z() - upperHit.z());
            if (not (dz < dzCut)) // If cut fails continue
            {
                if (logLevel >= SDL::Log_Debug3)
                {
                    SDL::cout << lowerModule << std::endl;
                    SDL::cout << "Debug: " << __FUNCTION__ << "()" << std::endl;
                    SDL::cout << "upperHit: " << upperHit << std::endl;
                    SDL::cout << "lowerHit: " << lowerHit << std::endl;
                    SDL::cout << "dz : " << dz << std::endl;
                    SDL::cout << "dzCut : " << dzCut << std::endl;
                }
                return false;
            }
            else
            {
                if (logLevel >= SDL::Log_Debug3)
                {
                    SDL::cout << lowerModule << std::endl;
                    SDL::cout << "Debug: " << __FUNCTION__ << "()" << std::endl;
                    SDL::cout << "upperHit: " << upperHit << std::endl;
                    SDL::cout << "lowerHit: " << lowerHit << std::endl;
                    SDL::cout << "dz : " << dz << std::endl;
                    SDL::cout << "dzCut : " << dzCut << std::endl;
                }
            }

            // Calculate the cut thresholds for the selection
            float miniCut = MiniDoublet::dPhiThreshold(lowerHit, lowerModule);

            // Cut #2: dphi difference
            // Ref to original code: https://github.com/slava77/cms-tkph2-ntuple/blob/184d2325147e6930030d3d1f780136bc2dd29ce6/doubletAnalysis.C#L3085
            float fabsdPhi = std::abs(lowerHit.deltaPhi(upperHit));
            if (not (fabsdPhi < miniCut)) // If cut fails continue
            {
                if (logLevel >= SDL::Log_Debug3)
                {
                    SDL::cout << lowerModule << std::endl;
                    SDL::cout << "Debug: " << __FUNCTION__ << "()" << std::endl;
                    SDL::cout << "upperHit: " << upperHit << std::endl;
                    SDL::cout << "lowerHit: " << lowerHit << std::endl;
                    SDL::cout << "fabsdPhi : " << fabsdPhi << std::endl;
                    SDL::cout << "miniCut : " << miniCut << std::endl;
                }
                return false;
            }
            else
            {
                if (logLevel >= SDL::Log_Debug3)
                {
                    SDL::cout << lowerModule << std::endl;
                    SDL::cout << "Debug: " << __FUNCTION__ << "()" << std::endl;
                    SDL::cout << "upperHit: " << upperHit << std::endl;
                    SDL::cout << "lowerHit: " << lowerHit << std::endl;
                    SDL::cout << "fabsdPhi : " << fabsdPhi << std::endl;
                    SDL::cout << "miniCut : " << miniCut << std::endl;
                }
            }

            // Cut #3: The dphi change going from lower Hit to upper Hit
            // Ref to original code: https://github.com/slava77/cms-tkph2-ntuple/blob/184d2325147e6930030d3d1f780136bc2dd29ce6/doubletAnalysis.C#L3076
            float fabsdPhiChange = std::abs(lowerHit.deltaPhiChange(upperHit));
            if (not (fabsdPhiChange < miniCut)) // If cut fails continue
            {
                if (logLevel >= SDL::Log_Debug3)
                {
                    SDL::cout << lowerModule << std::endl;
                    SDL::cout << "Debug: " << __FUNCTION__ << "()" << std::endl;
                    SDL::cout << "upperHit: " << upperHit << std::endl;
                    SDL::cout << "lowerHit: " << lowerHit << std::endl;
                    SDL::cout << "fabsdPhiChange : " << fabsdPhiChange << std::endl;
                    SDL::cout << "miniCut : " << miniCut << std::endl;
                }
                return false;
            }
            else
            {
                if (logLevel >= SDL::Log_Debug3)
                {
                    SDL::cout << lowerModule << std::endl;
                    SDL::cout << "Debug: " << __FUNCTION__ << "()" << std::endl;
                    SDL::cout << "upperHit: " << upperHit << std::endl;
                    SDL::cout << "lowerHit: " << lowerHit << std::endl;
                    SDL::cout << "fabsdPhiChange : " << fabsdPhiChange << std::endl;
                    SDL::cout << "miniCut : " << miniCut << std::endl;
                }
            }

            // If all cut passed this pair is good, and make and add the mini-doublet
            return true;

        }
        // If endcap, apply cuts for endcap mini-doublets
        else // If not barrel it is endcap
        {
            // Cut #1 : dz cut. The dz difference can't be larger than 1cm. (max separation is 4mm for modules in the endcap)
            // Ref to original code: https://github.com/slava77/cms-tkph2-ntuple/blob/184d2325147e6930030d3d1f780136bc2dd29ce6/doubletAnalysis.C#L3093
            const float dzCut = 1.f;
            float dz = std::abs(lowerHit.z() - upperHit.z());
            if (not (dz < dzCut)) // If cut fails continue
            {
                if (logLevel >= SDL::Log_Debug3)
                {
                    SDL::cout << lowerModule << std::endl;
                    SDL::cout << "Debug: " << __FUNCTION__ << "()" << std::endl;
                    SDL::cout << "upperHit: " << upperHit << std::endl;
                    SDL::cout << "lowerHit: " << lowerHit << std::endl;
                    SDL::cout << "dz : " << dz << std::endl;
                    SDL::cout << "dzCut : " << dzCut << std::endl;
                }
                return false;
            }
            else
            {
                if (logLevel >= SDL::Log_Debug3)
                {
                    SDL::cout << lowerModule << std::endl;
                    SDL::cout << "Debug: " << __FUNCTION__ << "()" << std::endl;
                    SDL::cout << "upperHit: " << upperHit << std::endl;
                    SDL::cout << "lowerHit: " << lowerHit << std::endl;
                    SDL::cout << "dz : " << dz << std::endl;
                    SDL::cout << "dzCut : " << dzCut << std::endl;
                }
            }

            // Cut #2 : drt cut. The dz difference can't be larger than 1cm. (max separation is 4mm for modules in the endcap)
            // Ref to original code: https://github.com/slava77/cms-tkph2-ntuple/blob/184d2325147e6930030d3d1f780136bc2dd29ce6/doubletAnalysis.C#L3100
            const float drtCut = 10.f; // i.e. should be smaller than the module length. Could be tighter if PS modules
            float drt = std::abs(lowerHit.rt() - upperHit.rt());
            if (not (drt < drtCut)) // If cut fails continue
            {
                if (logLevel >= SDL::Log_Debug3)
                {
                    SDL::cout << lowerModule << std::endl;
                    SDL::cout << "Debug: " << __FUNCTION__ << "()" << std::endl;
                    SDL::cout << "upperHit: " << upperHit << std::endl;
                    SDL::cout << "lowerHit: " << lowerHit << std::endl;
                    SDL::cout << "drt : " << drt << std::endl;
                    SDL::cout << "drtCut : " << drtCut << std::endl;
                }
                return false;
            }
            else
            {
                if (logLevel >= SDL::Log_Debug3)
                {
                    SDL::cout << lowerModule << std::endl;
                    SDL::cout << "Debug: " << __FUNCTION__ << "()" << std::endl;
                    SDL::cout << "upperHit: " << upperHit << std::endl;
                    SDL::cout << "lowerHit: " << lowerHit << std::endl;
                    SDL::cout << "drt : " << drt << std::endl;
                    SDL::cout << "drtCut : " << drtCut << std::endl;
                }
            }

            // Calculate the cut thresholds for the selection
            float miniCut = MiniDoublet::dPhiThreshold(lowerHit, lowerModule);

            // Cut #3: dphi difference
            // Ref to original code: https://github.com/slava77/cms-tkph2-ntuple/blob/184d2325147e6930030d3d1f780136bc2dd29ce6/doubletAnalysis.C#L3111
            // ----
            // Slava, 6:17 PM
            // here for the code to work you would need to slide (line extrapolate) the lower or the upper  hit along the strip direction to the radius of the other 
            // you'll get it almost right by assuming radial strips and just add the d_rt*(cosPhi, sinPhi)
            // ----
            // The algorithm assumed that the radial position is ~close according to Slava.
            // However, for PS modules, it is not the case.
            // So we'd have to move the hits to be in same position as the other.
            // We'll move the pixel along the radial direction (assuming the radial direction is more or less same as the strip direction)
            // TODO figure out which one is pixel hit (SDL::Module can answer this easily)
            SDL::Hit upperHitMod(upperHit);
            float xl = upperHitMod.x();
            float yl = upperHitMod.y();
            float d = 0; // TODO
            float xn = xl - d;
            float yn = yl - d;
            upperHitMod.setXYZ(xn, yn, upperHit.z());
            float fabsdPhi = std::abs(lowerHit.deltaPhi(upperHitMod));
            if (not (fabsdPhi < miniCut)) // If cut fails continue
            {
                if (logLevel >= SDL::Log_Debug3)
                {
                    SDL::cout << lowerModule << std::endl;
                    SDL::cout << "Debug: " << __FUNCTION__ << "()" << std::endl;
                    SDL::cout << "upperHit: " << upperHit << std::endl;
                    SDL::cout << "lowerHit: " << lowerHit << std::endl;
                    SDL::cout << "fabsdPhi : " << fabsdPhi << std::endl;
                    SDL::cout << "miniCut : " << miniCut << std::endl;
                }
                return false;
            }
            else
            {
                if (logLevel >= SDL::Log_Debug3)
                {
                    SDL::cout << lowerModule << std::endl;
                    SDL::cout << "Debug: " << __FUNCTION__ << "()" << std::endl;
                    SDL::cout << "upperHit: " << upperHit << std::endl;
                    SDL::cout << "lowerHit: " << lowerHit << std::endl;
                    SDL::cout << "fabsdPhi : " << fabsdPhi << std::endl;
                    SDL::cout << "miniCut : " << miniCut << std::endl;
                }
            }

            // Cut #3: Another cut on the dphi after some modification
            // Ref to original code: https://github.com/slava77/cms-tkph2-ntuple/blob/184d2325147e6930030d3d1f780136bc2dd29ce6/doubletAnalysis.C#L3119-L3124
            float dzFrac = dz / fabs(lowerHit.z());
            float fabsdPhiMod = fabsdPhi / dzFrac * (1.f + dzFrac);
            if (not (fabsdPhiMod < miniCut)) // If cut fails continue
            {
                if (logLevel >= SDL::Log_Debug2)
                {
                    SDL::cout << lowerModule << std::endl;
                    SDL::cout << "Debug: " << __FUNCTION__ << "()" << std::endl;
                    SDL::cout << "upperHit: " << upperHit << std::endl;
                    SDL::cout << "lowerHit: " << lowerHit << std::endl;
                    SDL::cout << "dz : " << dz << std::endl;
                    SDL::cout << "dzFrac : " << dzFrac << std::endl;
                    SDL::cout << "fabsdPhi : " << fabsdPhi << std::endl;
                    SDL::cout << "fabsdPhiMod : " << fabsdPhiMod << std::endl;
                    SDL::cout << "xl: " << xl <<  " yl: " << yl <<  std::endl;
                    SDL::cout << "xn: " << xn <<  " yn: " << yn <<  std::endl;
                    SDL::cout << "miniCut : " << miniCut << std::endl;
                }
                return false;
            }
            else
            {
                if (logLevel >= SDL::Log_Debug2)
                {
                    SDL::cout << lowerModule << std::endl;
                    SDL::cout << "Debug: " << __FUNCTION__ << "()" << std::endl;
                    SDL::cout << "upperHit: " << upperHit << std::endl;
                    SDL::cout << "lowerHit: " << lowerHit << std::endl;
                    SDL::cout << "dz : " << dz << std::endl;
                    SDL::cout << "dzFrac : " << dzFrac << std::endl;
                    SDL::cout << "fabsdPhi : " << fabsdPhi << std::endl;
                    SDL::cout << "fabsdPhiMod : " << fabsdPhiMod << std::endl;
                    SDL::cout << "xl: " << xl <<  " yl: " << yl <<  std::endl;
                    SDL::cout << "xn: " << xn <<  " yn: " << yn <<  std::endl;
                    SDL::cout << "miniCut : " << miniCut << std::endl;
                }
            }

            // If all cut passed this pair is good, and make and add the mini-doublet
            return true;
        }
    }
    else
    {
        SDL::cout << "Warning: Unrecognized mini-doublet algorithm!" << algo << std::endl;
        return false;
    }
}
