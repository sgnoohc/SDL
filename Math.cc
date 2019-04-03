#include "Math.h"

float SDL::Math::Phi_mpi_pi(float x)
{
    if (isnan(x))
    {
        std::cout << "SDL::Math::Phi_0_2pi() function called with NaN" << std::endl;
        return x;
    }

    while (x >= M_PI)
        x -= 2. * M_PI;

    while (x < -M_PI)
        x += 2. * M_PI;

    return x;

}

float SDL::Math::ATan2(float y, float x)
{
    if (x != 0) return  atan2(y, x);
    if (y == 0) return  0;
    if (y >  0) return  M_PI / 2;
    else        return -M_PI / 2;
}
