#ifndef MathUtil_h
#define MathUtil_h

#include <iostream>
#include <cmath>

#include "Hit.cuh"

namespace SDL
{
    class Hit;
}

namespace SDL
{
    // functions for math related operations
    namespace MathUtil
    {

        __device__ __host__ float Phi_mpi_pi(float phi);
        __device__ __host__ float ATan2(float y, float x);
        float ptEstimateFromDeltaPhiChangeAndRt(float dphiChange, float rt);
        float ptEstimateFromRadius(float radius);
        float dphiEstimateFromPtAndRt(float pt, float rt);
        SDL::Hit getCenterFromThreePoints(SDL::Hit& hit1, SDL::Hit& hit2, SDL::Hit& hit3);
        float angleCorr(float dr, float pt, float angle);

    }
}

#endif
