#include "Module.h"

SDL::Module::Module()
{
    setDetId(0);
}

SDL::Module::Module(unsigned int detId)
{
    setDetId(detId);
}

SDL::Module::Module(const Module& module)
{
    setDetId(module.detId());
}

SDL::Module::~Module()
{
}

const unsigned short& SDL::Module::subdet() const
{
    return subdet_;
}

const unsigned short& SDL::Module::side() const
{
    return side_;
}

const unsigned short& SDL::Module::layer() const
{
    return layer_;
}

const unsigned short& SDL::Module::rod() const
{
    return rod_;
}

const unsigned short& SDL::Module::ring() const
{
    return ring_;
}

const unsigned short& SDL::Module::module() const
{
    return module_;
}

const unsigned short& SDL::Module::isLower() const
{
    return isLower_;
}

const unsigned int& SDL::Module::detId() const
{
    return detId_;
}

const bool& SDL::Module::isInverted() const
{
    return isInverted_;
}

const std::vector<SDL::Hit*>& SDL::Module::hits() const
{
    return hits_;
}

void SDL::Module::setDetId(unsigned int detId)
{
    detId_ = detId;
    setDerivedQuantities();
}

void SDL::Module::setDerivedQuantities()
{
    subdet_ = parseSubdet(detId_);
    side_ = parseSide(detId_);
    layer_ = parseLayer(detId_);
    rod_ = parseRod(detId_);
    ring_ = parseRing(detId_);
    module_ = parseModule(detId_);
    isLower_ = parseIsLower(detId_);
}

void SDL::Module::addHit(SDL::Hit* hit)
{
    hits_.push_back(hit);
}

unsigned short SDL::Module::parseSubdet(unsigned int detId)
{
    return (detId & (7 << 25)) >> 25;
}

unsigned short SDL::Module::parseSide(unsigned int detId)
{
    if (parseSubdet(detId) == 4)
    {
        return (detId & (3 << 23)) >> 23;
    }
    else if (parseSubdet(detId) == 5)
    {
        return 3;
    }
    else
    {
        return 0;
    }
}

unsigned short SDL::Module::parseLayer(unsigned int detId)
{
    if (parseSubdet(detId) == 4)
    {
        return (detId & (7 << 18)) >> 18;
    }
    else if (parseSubdet(detId) == 5)
    {
        return (detId & (7 << 20)) >> 20;
    }
    else
    {
        return 0;
    }
}

unsigned short SDL::Module::parseRod(unsigned int detId)
{
    if (parseSubdet(detId) == 4)
    {
        return 0;
    }
    else if (parseSubdet(detId) == 5)
    {
        return (detId & (127 << 10)) >> 10;
    }
    else
    {
        return 0;
    }
}

unsigned short SDL::Module::parseRing(unsigned int detId)
{
    if (parseSubdet(detId) == 4)
    {
        return (detId & (15 << 12)) >> 12;
    }
    else if (parseSubdet(detId) == 5)
    {
        return 0;
    }
    else
    {
        return 0;
    }

}

unsigned short SDL::Module::parseModule(unsigned int detId)
{
    return (detId & (127 << 2)) >> 2;
}

unsigned short SDL::Module::parseIsLower(unsigned int detId)
{
    return (detId & 1);
}

namespace SDL
{
    std::ostream& operator<<(std::ostream& out, const Module& module)
    {
        out << "==============================" << std::endl;
        out << "Module(detId=" << module.detId();
        out << ", subdet=" << module.subdet_;
        out << ", side=" << module.side_;
        out << ", layer=" << module.layer_;
        out << ", rod=" << module.rod_;
        out << ", ring=" << module.ring_;
        out << ", module=" << module.module_;
        out << ", isLower=" << module.isLower_;
        out << ")" << std::endl;
        out << "==============================" << std::endl;
        for (auto& hit : module.hits())
            out << *hit << std::endl;
        out << "" << std::endl;

        return out;
    }
}
