#ifndef Hit_h
#define Hit_h

#include <iostream>
#include <cmath>
#include <vector>

#include "MathUtil.cuh"
#include "PrintUtil.h"

namespace SDL
{
    class Module;
}

namespace SDL
{

    class Hit
    {
        private:
            const Module* modulePtr_;

            float x_;
            float y_;
            float z_;
            float r3_; // 3d distance from origin
            float rt_; // transverse distance
            float phi_;
            float eta_;
            int idx_; // unique index to the hit index in the ntuple

            Hit* hit_high_edge_;
            Hit* hit_low_edge_;

            __device__ __host__ void setDerivedQuantities();

        public:

            // cnstr/destr
            Hit();
            __device__ __host__ Hit(float x, float y, float z, int idx=-1);
            __device__ __host__ Hit(const Hit&);
            __device__ __host__ ~Hit();

            // modifying class content
            __device__ __host__ void setX(float x);
            __device__ __host__ void setY(float y);
            __device__ __host__ void setZ(float z);
            __device__ __host__ void setXYZ(float x, float y, float z);
            __device__ __host__ void setIdx(int idx);
            __device__ __host__ void setModule(const Module*);

            // Set the boundary hits where the hits are shifted
            void setHitHighEdgePtr(Hit* hit);
            void setHitLowEdgePtr(Hit* hit);

            // accessor functions
            __host__ __device__ const float& x() const;
            __host__ __device__ const float& y() const;
            __host__ __device__ const float& z() const;
            __host__ __device__ const float& rt() const;
            __host__ __device__ const float& r3() const;
            __host__ __device__ const float& phi() const;
            __host__ __device__ const float& eta() const;
            __host__ __device__ const int& idx() const;
            __host__ __device__ const Module& getModule() const;

            // Set the boundary hits where the hits are shifted
            const Hit* getHitHighEdgePtr() const;
            const Hit* getHitLowEdgePtr() const;

            // variable computation between two hits
            __host__ __device__ float deltaPhi(const Hit&) const;
            __host__ __device__ float deltaPhiChange(const Hit&) const;
            bool isIdxMatched(const Hit&) const;

            // operator overloading
            bool operator !=(const Hit&) const;
            bool operator ==(const Hit&) const;
            __host__ __device__ Hit  operator - (const Hit&) const;
            __host__ __device__ Hit& operator = (const Hit&);
            __host__ __device__ Hit& operator -=(const Hit&);
            __host__ __device__ Hit& operator +=(const Hit&);
            __host__ __device__ Hit& operator /=(const float&);

            // cout printing
            friend std::ostream& operator<<(std::ostream& out, const Hit& hit);
            friend std::ostream& operator<<(std::ostream& out, const Hit* hit);

    };

}

#endif
