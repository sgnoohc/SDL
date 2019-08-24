#include "Layer.h"

const std::vector<std::pair<std::pair<int, SDL::Layer::SubDet>, std::pair<int, SDL::Layer::SubDet>>> SDL::Layer::tracklet_compatible_layer_pairs_ =
{
    // {{1, SDL::Layer::Barrel},{2, SDL::Layer::Barrel}},
    // {{2, SDL::Layer::Barrel},{3, SDL::Layer::Barrel}},
    // {{3, SDL::Layer::Barrel},{4, SDL::Layer::Barrel}},
    // {{4, SDL::Layer::Barrel},{5, SDL::Layer::Barrel}},
    // {{5, SDL::Layer::Barrel},{6, SDL::Layer::Barrel}},
    // {{1, SDL::Layer::Endcap},{2, SDL::Layer::Endcap}},
    // {{2, SDL::Layer::Endcap},{3, SDL::Layer::Endcap}},
    // {{3, SDL::Layer::Endcap},{4, SDL::Layer::Endcap}},
    // {{4, SDL::Layer::Endcap},{5, SDL::Layer::Endcap}},
    // {{1, SDL::Layer::Barrel},{1, SDL::Layer::Endcap}},
    // {{2, SDL::Layer::Barrel},{1, SDL::Layer::Endcap}},
    // {{3, SDL::Layer::Barrel},{1, SDL::Layer::Endcap}},
    // {{4, SDL::Layer::Barrel},{1, SDL::Layer::Endcap}},
    // {{5, SDL::Layer::Barrel},{1, SDL::Layer::Endcap}}
    {{1, SDL::Layer::Barrel},{3, SDL::Layer::Barrel}},
    {{1, SDL::Layer::Barrel},{4, SDL::Layer::Barrel}},
    {{1, SDL::Layer::Barrel},{5, SDL::Layer::Barrel}},
    {{2, SDL::Layer::Barrel},{4, SDL::Layer::Barrel}},
    {{2, SDL::Layer::Barrel},{5, SDL::Layer::Barrel}},
    {{3, SDL::Layer::Barrel},{5, SDL::Layer::Barrel}},
    {{1, SDL::Layer::Barrel},{1, SDL::Layer::Endcap}},
    {{2, SDL::Layer::Barrel},{1, SDL::Layer::Endcap}},
    {{3, SDL::Layer::Barrel},{1, SDL::Layer::Endcap}},
    {{4, SDL::Layer::Barrel},{1, SDL::Layer::Endcap}},
    {{5, SDL::Layer::Barrel},{1, SDL::Layer::Endcap}},
};

SDL::Layer::Layer()
{
}

SDL::Layer::Layer(int layerIdx, unsigned short subdet) : layer_idx_(layerIdx), subdet_(subdet)
{
}

SDL::Layer::~Layer()
{
}

const unsigned short& SDL::Layer::subdet() const
{
    return subdet_;
}

const int& SDL::Layer::layerIdx() const
{
    return layer_idx_;
}

const std::vector<SDL::Segment*>& SDL::Layer::getSegmentPtrs() const
{
    return segments_;
}

const std::vector<SDL::Tracklet*>& SDL::Layer::getTrackletPtrs() const
{
    return tracklets_;;
}

const std::vector<std::pair<std::pair<int, SDL::Layer::SubDet>, std::pair<int, SDL::Layer::SubDet>>>& SDL::Layer::getListOfTrackletCompatibleLayerPairs()
{
    return SDL::Layer::tracklet_compatible_layer_pairs_;
}

void SDL::Layer::setLayerIdx(int lidx)
{
    layer_idx_ = lidx;
}

void SDL::Layer::setSubDet(SDL::Layer::SubDet subdet)
{
    subdet_ = subdet;
}

void SDL::Layer::addSegment(SDL::Segment* sg)
{
    segments_.push_back(sg);
}

void SDL::Layer::addTracklet(SDL::Tracklet* tl)
{
    tracklets_.push_back(tl);
}

namespace SDL
{
    std::ostream& operator<<(std::ostream& out, const Layer& layer)
    {
        out << "==============================" << std::endl;
        out << "Layer(layerIdx=" << layer.layerIdx();
        out << ", subdet=" << layer.subdet();
        out << ")" << std::endl;
        out << "==============================" << std::endl;
        for (auto& segmentPtr : layer.segments_)
            out << segmentPtr << std::endl;
        for (auto& trackletPtr : layer.tracklets_)
            out << trackletPtr << std::endl;
        out << "" << std::endl;

        return out;
    }

    std::ostream& operator<<(std::ostream& out, const Layer* layer)
    {
        out << *layer;
        return out;
    }


}
