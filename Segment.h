#ifndef Segment_h
#define Segment_h

#include "MiniDoublet.h"
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
        // So physically it will look like
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

        public:
            Segment();
            Segment(const Segment&);
            Segment(MiniDoublet* innerMiniDoubletPtr, MiniDoublet* outerMiniDoubletPtr);
            ~Segment();

            MiniDoublet* innerMiniDoubletPtr() const;
            MiniDoublet* outerMiniDoubletPtr() const;

            // The function to actually determine whether a pair of mini-doublets is a reco-ed segment or not
            static bool isMiniDoubletPairASegment(const MiniDoublet& innerMiniDoublet, const MiniDoublet& outerMiniDoublet, const Module& innerLowerModule, const Module& innerOuterModule, SGAlgo algo, SDL::LogLevel logLevel=SDL::Log_Nothing);

            // cout printing
            friend std::ostream& operator<<(std::ostream& out, const Segment& md);
            friend std::ostream& operator<<(std::ostream& out, const Segment* md);

    };

}

#endif
