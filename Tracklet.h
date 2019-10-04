#ifndef Tracklet_h
#define Tracklet_h

#include <iomanip>

#include "Module.h"
#include "Algo.h"
#include "Segment.h"
#include "PrintUtil.h"

namespace SDL
{
    class Module;
    class Tracklet;
    class Segment;
}

namespace SDL
{
    class Tracklet
    {

        // Tracklet is two segments
        // So physically it will look like the following:
        //
        // Below, the pair of x's are one segment and the pair of y's are another segment
        //
        // The x's are outer segment
        // The y's are inner segment
        //
        //    --------x--------  <-
        //    ---------x-------   |
        //                        | outer segment
        //    ----------x------   |
        //    -----------x-----  <-
        //
        //
        //
        //
        //    -----------y-----  <-
        //    ----------y------   |
        //                        | inner segment
        //    ---------y-------   |
        //    --------y--------  <-
        //
        // Module naming is given above
        //

        private:

            // Inner Segment (inner means one closer to the beam position, i.e. lower "layer")
            Segment* innerSegmentPtr_;

            // Outer Segment (outer means one further away from the beam position, i.e. upper "layer")
            Segment* outerSegmentPtr_;

            // Bits to flag whether this tracklet passes some algorithm
            int passAlgo_;

        public:
            enum TrackletSelection
            {
                deltaZ = 0,
                deltaZPointed = 1,
                deltaPhiPos = 2,
                slope=3,
                dAlphaIn=4,
                dAlphaOut=5,
                dBeta=6,
                nCut=7
            };

        private:
            // Bits to flag whether this tracklet passes which cut of default algorithm
            int passBitsDefaultAlgo_;

            // Some reco'ed quantities
            float betaIn_;
            float betaInCut_;
            float betaOut_;
            float betaOutCut_;
            float deltaBeta_;
            float deltaBetaCut_;

            std::map<std::string, float> recovars_;

        public:
            Tracklet();
            Tracklet(const Tracklet&);
            Tracklet(Segment* innerSegmentPtr, Segment* outerSegmentPtr);
            ~Tracklet();

            Segment* innerSegmentPtr() const;
            Segment* outerSegmentPtr() const;
            const int& getPassAlgo() const;
            const int& getPassBitsDefaultAlgo() const;
            const float& getDeltaBeta() const;
            const float& getDeltaBetaCut() const;
            const float& getBetaIn() const;
            const float& getBetaInCut() const;
            const float& getBetaOut() const;
            const float& getBetaOutCut() const;
            const std::map<std::string, float>& getRecoVars() const;
            const float& getRecoVar(std::string) const;

            void setDeltaBeta(float);
            void setDeltaBetaCut(float);
            void setBetaIn(float);
            void setBetaInCut(float);
            void setBetaOut(float);
            void setBetaOutCut(float);
            void setRecoVars(std::string, float);

            // return whether it passed the algorithm
            bool passesTrackletAlgo(TLAlgo algo) const;

            // The function to run segment algorithm on a segment candidate
            void runTrackletAlgo(TLAlgo algo, SDL::LogLevel logLevel=SDL::Log_Nothing);

            // The following algorithm does nothing and accept everything
            void runTrackletAllCombAlgo();

            // The default algorithms
            void runTrackletDefaultAlgo(SDL::LogLevel logLevel);
            void runTrackletDefaultAlgoBarrelBarrelBarrelBarrel(SDL::LogLevel logLevel);
            void runTrackletDefaultAlgoBarrelBarrelEndcapEndcap(SDL::LogLevel logLevel);
            void runTrackletDefaultAlgoBarrelBarrel(SDL::LogLevel logLevel);
            void runTrackletDefaultAlgoBarrelEndcap(SDL::LogLevel logLevel);
            void runTrackletDefaultAlgoEndcapEndcap(SDL::LogLevel logLevel);

            bool isIdxMatched(const Tracklet&) const;
            bool isAnchorHitIdxMatched(const Tracklet&) const;

            // The function to actually determine whether a pair of mini-doublets is a reco-ed segment or not
            static bool isSegmentPairATracklet(const Segment& innerSegment, const Segment& outerSegment, TLAlgo algo, SDL::LogLevel logLevel=SDL::Log_Nothing);
            static bool isSegmentPairATrackletBarrel(const Segment& innerSegment, const Segment& outerSegment, TLAlgo algo, SDL::LogLevel logLevel=SDL::Log_Nothing);
            static bool isSegmentPairATrackletEndcap(const Segment& innerSegment, const Segment& outerSegment, TLAlgo algo, SDL::LogLevel logLevel=SDL::Log_Nothing);

            // cout printing
            friend std::ostream& operator<<(std::ostream& out, const Tracklet& tl);
            friend std::ostream& operator<<(std::ostream& out, const Tracklet* tl);

    };

}

#endif
