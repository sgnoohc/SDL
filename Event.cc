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

void SDL::Event::createMiniDoublets(MDAlgo algo)
{
    // Loop over lower modules
    for (auto& lowerModulePtr : getLowerModulePtrs())
    {

        // Create mini doublets
        createMiniDoubletsFromLowerModule(lowerModulePtr->detId(), algo);

    }
}

void SDL::Event::createMiniDoubletsFromLowerModule(unsigned int detId, SDL::MDAlgo algo)
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
                if (algo == SDL::AllComb_MDAlgo)
                {
                    addMiniDoubletToLowerModule(SDL::MiniDoublet(lowerHitPtr, upperHitPtr), lowerModule.detId());
                    continue;
                }
                else if (algo == SDL::Default_MDAlgo)
                {

                    // The dphi change going from lower Hit to upper Hit
                    float fabsdphi = fabs(lowerHit.deltaPhiChange(upperHit));

                    float rt = lowerHit.rt();
                    unsigned int iL = lowerModule.layer() - 1;
                    float miniCut = MiniDoublet::dPhiThresholdBarrel(rt, iL);

                    if (fabsdphi < miniCut)
                    {
                        addMiniDoubletToLowerModule(SDL::MiniDoublet(lowerHitPtr, upperHitPtr), lowerModule.detId());
                    }
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
