#include "MathUtil.h"

float SDL::MathUtil::Phi_mpi_pi(float x)
{
    if (isnan(x))
    {
        std::cout << "SDL::MathUtil::Phi_0_2pi() function called with NaN" << std::endl;
        return x;
    }

    while (x >= M_PI)
        x -= 2. * M_PI;

    while (x < -M_PI)
        x += 2. * M_PI;

    return x;

}

float SDL::MathUtil::ATan2(float y, float x)
{
    if (x != 0) return  atan2(y, x);
    if (y == 0) return  0;
    if (y >  0) return  M_PI / 2;
    else        return -M_PI / 2;
}

float SDL::MathUtil::ptEstimateFromDeltaPhiChangeAndRt(float dPhiChange, float rt)
{
    return rt * 2.99792458e-3 * 3.8 / 2. / std::sin(dPhiChange);
}

float SDL::MathUtil::dphiEstimateFromPtAndRt(float pt, float rt)
{
    return std::asin(rt / (2 * pt / (2.99792458e-3 * 3.8)));
}
