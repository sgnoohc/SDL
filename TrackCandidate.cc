#include "TrackCandidate.h"

SDL::TrackCandidate::TrackCandidate()
{
}

SDL::TrackCandidate::~TrackCandidate()
{
}

SDL::TrackCandidate::TrackCandidate(const TrackCandidate& tl) :
    innerTrackletPtr_(tl.innerTrackletPtr()),
    outerTrackletPtr_(tl.outerTrackletPtr()),
    passAlgo_(tl.getPassAlgo()),
    passBitsDefaultAlgo_(tl.getPassBitsDefaultAlgo()),
    recovars_(tl.getRecoVars())
{
}

SDL::TrackCandidate::TrackCandidate(SDL::Tracklet* innerTrackletPtr, SDL::Tracklet* outerTrackletPtr) :
    innerTrackletPtr_(innerTrackletPtr),
    outerTrackletPtr_(outerTrackletPtr),
    passAlgo_(0),
    passBitsDefaultAlgo_(0)
{
}

SDL::Tracklet* SDL::TrackCandidate::innerTrackletPtr() const
{
    return innerTrackletPtr_;
}

SDL::Tracklet* SDL::TrackCandidate::outerTrackletPtr() const
{
    return outerTrackletPtr_;
}

const int& SDL::TrackCandidate::getPassAlgo() const
{
    return passAlgo_;
}

const int& SDL::TrackCandidate::getPassBitsDefaultAlgo() const
{
    return passBitsDefaultAlgo_;
}

const std::map<std::string, float>& SDL::TrackCandidate::getRecoVars() const
{
    return recovars_;
}

const float& SDL::TrackCandidate::getRecoVar(std::string key) const
{
    return recovars_.at(key);
}

void SDL::TrackCandidate::setRecoVars(std::string key, float var)
{
    recovars_[key] = var;
}

bool SDL::TrackCandidate::passesTrackCandidateAlgo(SDL::TCAlgo algo) const
{
    // Each algorithm is an enum shift it by its value and check against the flag
    return passAlgo_ & (1 << algo);
}

void SDL::TrackCandidate::runTrackCandidateAlgo(SDL::TCAlgo algo, SDL::LogLevel logLevel)
{
    if (algo == SDL::AllComb_TCAlgo)
    {
        runTrackCandidateAllCombAlgo();
    }
    else if (algo == SDL::Default_TCAlgo)
    {
        runTrackCandidateDefaultAlgo(logLevel);
    }
    else
    {
        SDL::cout << "Warning: Unrecognized track candidate algorithm!" << algo << std::endl;
        return;
    }
}

void SDL::TrackCandidate::runTrackCandidateAllCombAlgo()
{
    passAlgo_ |= (1 << SDL::AllComb_TCAlgo);
}

void SDL::TrackCandidate::runTrackCandidateDefaultAlgo(SDL::LogLevel logLevel)
{

    passAlgo_ &= (0 << SDL::Default_TCAlgo);

    // SDL::Segment* innerOuterSegment = innerTrackletPtr()->outerSegmentPtr();
    // SDL::Segment* outerInnerSegment = outerTrackletPtr()->innerSegmentPtr();

    // std::cout <<  " innerOuterSegment: " << innerOuterSegment <<  std::endl;
    // std::cout <<  " outerInnerSegment: " << outerInnerSegment <<  std::endl;
    // std::cout <<  " innerTrackletPtr()->hasCommonSegment(*(outerTrackletPtr())): " << innerTrackletPtr()->hasCommonSegment(*(outerTrackletPtr())) <<  std::endl;

    // passAlgo_ |= (1 << SDL::Default_TCAlgo);
    // return;

    if (not (innerTrackletPtr()->hasCommonSegment(*(outerTrackletPtr()))))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #1 in " << __FUNCTION__ << std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TCAlgo);
        // passAlgo_ |= (1 << SDL::Default_TCAlgo);
        return;
    }
    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << TrackCandidateSelection::commonSegment);

    SDL::Segment* innerInnerSegment = innerTrackletPtr()->innerSegmentPtr();
    SDL::Segment* innerOuterSegment = innerTrackletPtr()->outerSegmentPtr();
    SDL::Segment* outerInnerSegment = outerTrackletPtr()->innerSegmentPtr();
    SDL::Segment* outerOuterSegment = outerTrackletPtr()->outerSegmentPtr();

    SDL::Hit& innerA = (*innerInnerSegment->innerMiniDoubletPtr()->anchorHitPtr());
    SDL::Hit& innerB = (*innerInnerSegment->outerMiniDoubletPtr()->anchorHitPtr());
    SDL::Hit& innerC = (*innerOuterSegment->innerMiniDoubletPtr()->anchorHitPtr());
    SDL::Hit& outerA = (*outerInnerSegment->outerMiniDoubletPtr()->anchorHitPtr());
    SDL::Hit& outerB = (*outerOuterSegment->innerMiniDoubletPtr()->anchorHitPtr());
    SDL::Hit& outerC = (*outerOuterSegment->outerMiniDoubletPtr()->anchorHitPtr());

    SDL::Hit innerPoint = SDL::MathUtil::getCenterFromThreePoints(innerA, innerB, innerC);
    SDL::Hit outerPoint = SDL::MathUtil::getCenterFromThreePoints(outerA, outerB, outerC);

    float innerRadius = sqrt(pow(innerA.x() - innerPoint.x(), 2) + pow(innerA.y() - innerPoint.y(), 2));
    float outerRadius = sqrt(pow(outerA.x() - outerPoint.x(), 2) + pow(outerA.y() - outerPoint.y(), 2));

    float dR = (innerRadius - outerRadius) / innerRadius;
    setRecoVars("dR", dR);
    setRecoVars("innerR", innerRadius);

    float upperthresh =  0.6 / 15000. * innerRadius + 0.2;
    float lowerthresh = -1.4 / 4000. * innerRadius - 0.1;

    if (not (dR > lowerthresh and dR < upperthresh))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #2 in " << __FUNCTION__ << std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TCAlgo);
        // passAlgo_ |= (1 << SDL::Default_TCAlgo);
        return;
    }
    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << TrackCandidateSelection::ptConsistency);

    passAlgo_ |= (1 << SDL::Default_TCAlgo);
}

bool SDL::TrackCandidate::isIdxMatched(const TrackCandidate& tc) const
{
    if (not innerTrackletPtr()->isIdxMatched(*(tc.innerTrackletPtr())))
        return false;
    if (not outerTrackletPtr()->isIdxMatched(*(tc.outerTrackletPtr())))
        return false;
    return true;
}

bool SDL::TrackCandidate::isAnchorHitIdxMatched(const TrackCandidate& tc) const
{
    if (not innerTrackletPtr()->isAnchorHitIdxMatched(*(tc.innerTrackletPtr())))
        return false;
    if (not outerTrackletPtr()->isAnchorHitIdxMatched(*(tc.outerTrackletPtr())))
        return false;
    return true;
}

bool SDL::TrackCandidate::isTrackletPairATrackCandidate(const Tracklet& innerTracklet, const Tracklet& outerTracklet, TCAlgo algo, SDL::LogLevel logLevel)
{
    // If the algorithm is "do all combination" (e.g. used for efficiency calculation)
    if (algo == SDL::AllComb_TCAlgo)
    {
        return true;
    }
    else if (algo == SDL::Default_TCAlgo)
    {
        return false;
    }
    else
    {
        SDL::cout << "Warning: Unrecognized track candidate algorithm!" << algo << std::endl;
        return false;
    }
}

namespace SDL
{
    std::ostream& operator<<(std::ostream& out, const TrackCandidate& tc)
    {
        out << "TrackCandidate" << std::endl;
        out << "------------------------------" << std::endl;
        {
            IndentingOStreambuf indent(out);
            out << "Inner Tracklet" << std::endl;
            out << "------------------------------" << std::endl;
            {
                IndentingOStreambuf indent(out);
                out << tc.innerTrackletPtr_ << std::endl;
            }
            out << "Outer Tracklet" << std::endl;
            out << "------------------------------" << std::endl;
            {
                IndentingOStreambuf indent(out);
                out << tc.outerTrackletPtr_;
            }
        }
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const TrackCandidate* tc)
    {
        out << *tc;
        return out;
    }
}

