#ifndef StudySegmentOccupancy_h
#define StudySegmentOccupancy_h

#include "SDL/Event.h"

#include "Study.h"

#include <vector>
#include <tuple>

#include "TString.h"
#include "trktree.h"
#include "constants.h"

#include "AnalysisConfig.h"

class StudySegmentOccupancy : public Study
{

public:
    enum StudySegmentOccupancyMode
    {
        kStudyAll = 1,
    };

    const char* studyname;
    StudySegmentOccupancyMode mode;
    const char* modename;
    std::array<int, NLAYERS> n_in_lower_modules_by_layer;
    std::array<int, NLAYERS> n_in_upper_modules_by_layer;
    std::array<int, NLAYERS> n_in_both_modules_by_layer;

    StudySegmentOccupancy(const char* studyName, StudySegmentOccupancy::StudySegmentOccupancyMode mode_);
    virtual void bookStudy();
    virtual void doStudy(SDL::Event& recoevent, std::vector<std::tuple<unsigned int, SDL::Event*>> simtrkevents);

};

#endif
