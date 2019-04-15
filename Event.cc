#include "Event.h"

SDL::Event::Event()
{
}

SDL::Event::~Event()
{
}

bool SDL::Event::hasModule(unsigned int detId)
{
    if (modulesMapByDetId_.find(detId) == modulesMapByDetId_.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

SDL::Module& SDL::Event::getModule(unsigned int detId)
{
    // using std::map::emplace
    std::pair<std::map<unsigned int, Module>::iterator, bool> emplace_result = modulesMapByDetId_.emplace(detId, detId);

    // Retreive the module
    auto& inserted_or_existing = (*(emplace_result.first)).second;

    // If new was inserted, then insert to modules_ pointer list
    if (emplace_result.second) // if true, new was inserted
    {

        // The pointer to be added
        Module* module_ptr = &((*(emplace_result.first)).second);

        // Add the module pointer to the list of modules
        modules_.push_back(module_ptr);

        // If the module is lower module then add to list of lower modules
        if (module_ptr->isLower())
            lower_modules_.push_back(module_ptr);
    }

    return inserted_or_existing;
}

const std::vector<SDL::Module*> SDL::Event::getModulePtrs() const
{
    return modules_;
}

const std::vector<SDL::Module*> SDL::Event::getLowerModulePtrs() const
{
    return lower_modules_;
}

void SDL::Event::addHitToModule(SDL::Hit hit, unsigned int detId)
{
    // Add to global list of hits, where it will hold the object's instance
    hits_.push_back(hit);

    // And get the module (if not exists, then create), and add the address to Module.hits_
    getModule(detId).addHit(&(hits_.back()));
}

void SDL::Event::addMiniDoubletToLowerModule(SDL::MiniDoublet md, unsigned int detId)
{
    // Add to global list of mini doublets, where it will hold the object's instance
    miniDoublets_.push_back(md);

    // And get the module (if not exists, then create), and add the address to Module.hits_
    getModule(detId).addMiniDoublet(&(miniDoublets_.back()));
}

void SDL::Event::createMiniDoublets()
{
    // Loop over lower modules
    for (auto& lowerModulePtr : getLowerModulePtrs())
    {

        // Create mini doublets
        createMiniDoubletsFromLowerModule(lowerModulePtr->detId());

    }
}

void SDL::Event::createMiniDoubletsFromLowerModule(unsigned int detId)
{
    // Get reference to the lower Module
    Module& lowerModule = getModule(detId);

    // Get reference to the upper Module
    Module& upperModule = getModule(lowerModule.partnerDetId());

    // Double nested loops
    // Loop over lower module hits
    for (auto& lowerHitPtr : lowerModule.getHitPtrs())
    {

        // Get reference to lower Hit
        SDL::Hit& lowerHit = *lowerHitPtr;

        // Loop over upper module hits
        for (auto& upperHitPtr : upperModule.getHitPtrs())
        {

            // Get reference to upper Hit
            SDL::Hit& upperHit = *upperHitPtr;

            if (lowerModule.subdet() == SDL::Module::Barrel and lowerModule.side() == SDL::Module::Center)
            {
                // The dphi change going from lower Hit to upper Hit
                float dphi = lowerHit.deltaPhiChange(upperHit);

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
                float rt = lowerHit.rt();
                unsigned int iL = lowerModule.layer() - 1;
                const float miniSlope = std::asin(std::min(rt * k2Rinv1GeVf / ptCut, sinAlphaMax));
                const float rLayNominal = miniRminMeanBarrel[iL];
                const float miniPVoff = 0.1 / rLayNominal;
                const float miniMuls = miniMulsPtScaleBarrel[iL] * 3.f / ptCut;
                const bool isTilted = false;
                const bool tiltedOT123 = false;
                const float pixelPSZpitch = 0.15;
                const float miniTilt = isTilted && tiltedOT123 ? /*0.5f * pixelPSZpitch * drdz / sqrt(1.f + drdz * drdz) / miniDelta[iL]*/ : 0;
                const float miniCut = miniSlope + sqrt(pow(miniMuls, 2) + pow(miniPVoff, 2) + pow(miniTilt * miniSlope, 2));

                if (dphi < miniCut)
                {
                    addMiniDoubletToLowerModule(SDL::MiniDoublet(lowerHitPtr, upperHitPtr), lowerModule.detId());
                }

            }

        }

    }

}

namespace SDL
{
    std::ostream& operator<<(std::ostream& out, const Event& event)
    {

        out << "" << std::endl;
        out << "==============" << std::endl;
        out << "Printing Event" << std::endl;
        out << "==============" << std::endl;
        out << "" << std::endl;

        for (auto& modulePtr : event.modules_)
        {
            out << modulePtr;
        }

        return out;
    }

    std::ostream& operator<<(std::ostream& out, const Event* event)
    {
        out << *event;
        return out;
    }

}
