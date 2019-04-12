#ifndef Hit_h
#define Hit_h

#include <math.h>
#include <iostream>
#include <cmath>

#include "Math.h"

namespace SDL
{

    class Hit
    {
        private:
            float x_;
            float y_;
            float z_;
            float r3_; // 3d distance from origin
            float rt_; // transverse distance
            float phi_;

            void setDerivedQuantities();

        public:

            // cnstr/destr
            Hit();
            Hit(float x, float y, float z);
            Hit(const Hit&);
            ~Hit();

            // modifying class content
            void setX(float x);
            void setY(float y);
            void setZ(float z);

            // accessor functions
            const float& x() const;
            const float& y() const;
            const float& z() const;
            const float& rt() const;
            const float& r3() const;
            const float& phi() const;

            // variable computation between two hits
            float deltaPhi(const Hit&) const;
            float deltaPhiChange(const Hit&) const;

            // operator overloading
            bool operator !=(const Hit&) const;
            bool operator ==(const Hit&) const;
            Hit  operator - (const Hit&) const;
            Hit& operator = (const Hit&);
            Hit& operator -=(const Hit&);
            Hit& operator +=(const Hit&);

            // cout printing
            friend std::ostream& operator<<(std::ostream& out, const Hit& hit);
            friend std::ostream& operator<<(std::ostream& out, const Hit* hit);

    };

}

#endif
