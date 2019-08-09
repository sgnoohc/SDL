#ifndef Segment_h
#define Segment_h

#include "MiniDoublet.h"
#include "TiltedGeometry.h"
#include "Module.h"
#include "Algo.h"

namespace SDL
{
    class Module;
    class MiniDoublet;
}

namespace SDL
{
    class Segment
    {

        // Segment is two mini-doublets
        // So physically it will look like the following:
        //
        // Below, the pair of x's are one mini-doublet and the pair of y's are another mini-doublet
        //
        // The x's are outer mini-doublet
        // The y's are inner mini-doublet
        //
        //    --------x--------
        //    ---------x-------  <-- outer lower module
        //
        //    ---------y-------
        //    --------y--------  <-- inner lower module
        //
        // Module naming is given above
        //

        private:

            // Inner MiniDoublet (inner means one closer to the beam position, i.e. lower "layer")
            MiniDoublet* innerMiniDoubletPtr_;

            // Outer MiniDoublet (outer means one further away from the beam position, i.e. upper "layer")
            MiniDoublet* outerMiniDoubletPtr_;

            // Bits to flag whether this segment passes some algorithm
            int passAlgo_;

            // Some reco'ed quantities
            float rt_;
            float dphichange_;

        public:
            Segment();
            Segment(const Segment&);
            Segment(MiniDoublet* innerMiniDoubletPtr, MiniDoublet* outerMiniDoubletPtr);
            ~Segment();

            MiniDoublet* innerMiniDoubletPtr() const;
            MiniDoublet* outerMiniDoubletPtr() const;
            const int& getPassAlgo() const;
            const float& getRt() const;
            const float& getDeltaPhiChange() const;

            void setRt(float);
            void setDeltaPhiChange(float);

            // return whether it passed the algorithm
            bool passesSegmentAlgo(SGAlgo algo) const;

            // The function to run segment algorithm on a segment candidate
            void runSegmentAlgo(SGAlgo algo, SDL::LogLevel logLevel=SDL::Log_Nothing);

            // The following algorithm does nothing and accept everything
            void runSegmentAllCombAlgo();

            // The default algorithms
            void runSegmentDefaultAlgo(SDL::LogLevel logLevel);
            void runSegmentDefaultAlgoBarrel(SDL::LogLevel logLevel);
            void runSegmentDefaultAlgoEndcap(SDL::LogLevel logLevel);

            bool isIdxMatched(const Segment&) const;

            // The function to actually determine whether a pair of mini-doublets is a reco-ed segment or not
            static bool isMiniDoubletPairASegment(const MiniDoublet& innerMiniDoublet, const MiniDoublet& outerMiniDoublet, SGAlgo algo, SDL::LogLevel logLevel=SDL::Log_Nothing);
            static bool isMiniDoubletPairASegmentCandidateBarrel(const MiniDoublet& innerMiniDoublet, const MiniDoublet& outerMiniDoublet, SGAlgo algo, SDL::LogLevel logLevel=SDL::Log_Nothing);
            static bool isMiniDoubletPairASegmentCandidateEndcap(const MiniDoublet& innerMiniDoublet, const MiniDoublet& outerMiniDoublet, SGAlgo algo, SDL::LogLevel logLevel=SDL::Log_Nothing);
            static bool isMiniDoubletPairAngleCompatibleEndcap(const MiniDoublet& innerMiniDoublet, const MiniDoublet& outerMiniDoublet, SGAlgo algo, SDL::LogLevel logLevel=SDL::Log_Nothing);

            // cout printing
            friend std::ostream& operator<<(std::ostream& out, const Segment& md);
            friend std::ostream& operator<<(std::ostream& out, const Segment* md);

    };

}

#endif
