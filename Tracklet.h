#ifndef Tracklet_h
#define Tracklet_h

#include "Module.h"
#include "Algo.h"
#include "Segment.h"

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

        public:
            Tracklet();
            Tracklet(const Tracklet&);
            Tracklet(Segment* innerSegmentPtr, Segment* outerSegmentPtr);
            ~Tracklet();

            Segment* innerSegmentPtr() const;
            Segment* outerSegmentPtr() const;

            bool isIdxMatched(const Tracklet&) const;

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
