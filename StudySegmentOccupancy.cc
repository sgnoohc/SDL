#include "StudySegmentOccupancy.h"

StudySegmentOccupancy::StudySegmentOccupancy(const char* studyName, StudySegmentOccupancy::StudySegmentOccupancyMode mode_)
{

    studyname = studyName;
    mode = mode_;
    switch (mode)
    {
        case kStudyAll: modename = "all"; break;
        default: modename = "UNDEFINED"; break;
    }

}

void StudySegmentOccupancy::bookStudy()
{
    // Book Histograms
    const int nlayers = NLAYERS;
    for (int ii = 0; ii < nlayers; ++ii)
    {
        ana.histograms.addHistogram(TString::Format("n_sg_lower_by_layer%d", ii), 56, 0, 14000, [&, ii]() { return n_in_lower_modules_by_layer[ii]; } );
        ana.histograms.addHistogram(TString::Format("n_sg_upper_by_layer%d", ii), 56, 0, 14000, [&, ii]() { return n_in_upper_modules_by_layer[ii]; } );
        ana.histograms.addHistogram(TString::Format("n_sg_both_by_layer%d", ii),112, 0, 28000, [&, ii]() { return n_in_both_modules_by_layer[ii]; } );
    }

}

void StudySegmentOccupancy::doStudy(SDL::Event& event, std::vector<std::tuple<unsigned int, SDL::Event*>> simtrkevents)
{

    // Each do study is performed per event

    // First clear all the output variables that will be used to fill the histograms for this event
    for (int ii = 0; ii < NLAYERS; ++ii)
    {
        n_in_lower_modules_by_layer[ii] = 0;
        n_in_upper_modules_by_layer[ii] = 0;
        n_in_both_modules_by_layer[ii] = 0;
    }

    //***********************
    // Studying Hit Occupancy
    //***********************

    std::vector<SDL::Module*> moduleList = event.getModulePtrs();

    // Loop over tracklets in event
    for (auto& modulePtr : moduleList)
    {
        int subdet = modulePtr->subdet();
        if (subdet != 5)
            continue;

        // Get the nhit
        int nhit_in_module = modulePtr->getSegmentPtrs().size();

        // Get the layer index
        int ilayer = modulePtr->layer();

        // Is Lower
        int isLower = modulePtr->isLower();

        // Add to the counter
        if (isLower)
            n_in_lower_modules_by_layer[ilayer-1] += nhit_in_module;
        else
            n_in_upper_modules_by_layer[ilayer-1] += nhit_in_module;
        n_in_both_modules_by_layer[ilayer-1] += nhit_in_module;

    }

    // First clear all the output variables that will be used to fill the histograms for this event
    for (int ii = 0; ii < NLAYERS; ++ii)
    {
        // std::cout <<  " n_in_lower_modules_by_layer[ii]: " << n_in_lower_modules_by_layer[ii] <<  std::endl;
        // std::cout <<  " n_in_upper_modules_by_layer[ii]: " << n_in_upper_modules_by_layer[ii] <<  std::endl;
        // std::cout <<  " n_in_both_modules_by_layer[ii]: " << n_in_both_modules_by_layer[ii] <<  std::endl;
    }

}
