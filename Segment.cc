#include "Segment.h"

SDL::Segment::Segment()
{
}

SDL::Segment::~Segment()
{
}

SDL::Segment::Segment(const Segment& sg) :
    innerMiniDoubletPtr_(sg.innerMiniDoubletPtr()),
    outerMiniDoubletPtr_(sg.outerMiniDoubletPtr()),
    passAlgo_(sg.getPassAlgo()),
    passBitsDefaultAlgo_(sg.getPassBitsDefaultAlgo()),
    rtOut_(sg.getRtOut()),
    rtIn_(sg.getRtIn()),
    dphichange_(sg.getDeltaPhiChange()),
    zOut_(sg.getZOut()),
    zIn_ (sg.getZIn()),
    zLo_(sg.getZLo()),
    zHi_(sg.getZHi()),
    rtLo_(sg.getRtLo()),
    rtHi_(sg.getRtHi()),
    recovars_(sg.getRecoVars())
{
}

SDL::Segment::Segment(SDL::MiniDoublet* innerMiniDoubletPtr, SDL::MiniDoublet* outerMiniDoubletPtr) :
    innerMiniDoubletPtr_(innerMiniDoubletPtr),
    outerMiniDoubletPtr_(outerMiniDoubletPtr),
    passAlgo_(0),
    passBitsDefaultAlgo_(0),
    rtOut_(0),
    rtIn_(0),
    dphichange_(0),
    zOut_(0),
    zIn_(0),
    zLo_(0),
    zHi_(0),
    rtLo_(0),
    rtHi_(0)
{
}

SDL::MiniDoublet* SDL::Segment::innerMiniDoubletPtr() const
{
    return innerMiniDoubletPtr_;
}

SDL::MiniDoublet* SDL::Segment::outerMiniDoubletPtr() const
{
    return outerMiniDoubletPtr_;
}

const int& SDL::Segment::getPassAlgo() const
{
    return passAlgo_;
}

const int& SDL::Segment::getPassBitsDefaultAlgo() const
{
    return passBitsDefaultAlgo_;
}

const float& SDL::Segment::getRtOut() const
{
    return rtOut_;
}

const float& SDL::Segment::getRtIn() const
{
    return rtIn_;
}

const float& SDL::Segment::getDeltaPhiChange() const
{
    return dphichange_;
}

const float& SDL::Segment::getZOut() const
{
    return zOut_;
}

const float& SDL::Segment::getZIn() const
{
    return zIn_;
}

const float& SDL::Segment::getZLo() const
{
    return zLo_;
}

const float& SDL::Segment::getZHi() const
{
    return zHi_;
}

const float& SDL::Segment::getRtLo() const
{
    return rtLo_;
}

const float& SDL::Segment::getRtHi() const
{
    return rtHi_;
}

const std::map<std::string, float>& SDL::Segment::getRecoVars() const
{
    return recovars_;
}

const float& SDL::Segment::getRecoVar(std::string key) const
{
    return recovars_.at(key);
}

void SDL::Segment::setRtOut(float rt)
{
    rtOut_ = rt;
}

void SDL::Segment::setRtLo(float rt)
{
    rtLo_ = rt;
}

void SDL::Segment::setRtHi(float rt)
{
    rtHi_ = rt;
}

void SDL::Segment::setRtIn(float rt)
{
    rtIn_ = rt;
}

void SDL::Segment::setDeltaPhiChange(float dphichange)
{
    dphichange_ = dphichange;
}

void SDL::Segment::setZOut(float zOut)
{
    zOut_ = zOut;
}

void SDL::Segment::setZIn(float zIn)
{
    zIn_ = zIn;
}

void SDL::Segment::setZLo(float zLo)
{
    zLo_ = zLo;
}

void SDL::Segment::setZHi(float zHi)
{
    zHi_ = zHi;
}

void SDL::Segment::setRecoVars(std::string key, float var)
{
    recovars_[key] = var;
}

bool SDL::Segment::passesSegmentAlgo(SDL::SGAlgo algo) const
{
    // Each algorithm is an enum shift it by its value and check against the flag
    return passAlgo_ & (1 << algo);
}

void SDL::Segment::runSegmentAlgo(SDL::SGAlgo algo, SDL::LogLevel logLevel)
{
    if (algo == SDL::AllComb_SGAlgo)
    {
        runSegmentAllCombAlgo();
    }
    else if (algo == SDL::Default_SGAlgo)
    {
        runSegmentDefaultAlgo(logLevel);
    }
    else
    {
        SDL::cout << "Warning: Unrecognized segment algorithm!" << algo << std::endl;
        return;
    }
}

void SDL::Segment::runSegmentAllCombAlgo()
{
    passAlgo_ |= (1 << SDL::AllComb_SGAlgo);
}

void SDL::Segment::runSegmentDefaultAlgo(SDL::LogLevel logLevel)
{
    // Retreived the lower module object
    const Module& innerLowerModule = innerMiniDoubletPtr()->lowerHitPtr()->getModule();
    const Module& outerLowerModule = outerMiniDoubletPtr()->lowerHitPtr()->getModule();

    //FIXME:Change the whole thing to a check in outer module alone if this trick works!

    if (innerLowerModule.subdet() == SDL::Module::Barrel)
    {
        if(outerLowerModule.subdet() == SDL::Module::Barrel)
        {
            //Needs a name change to BarrelBarrel later
            runSegmentDefaultAlgoBarrel(logLevel);
        }
        else
        {
            runSegmentDefaultAlgoEndcap(logLevel);
        }
    }
    else
    {
        if(outerLowerModule.subdet() == SDL::Module::Endcap)
        {
            runSegmentDefaultAlgoEndcap(logLevel);
        }
        else
        {
            runSegmentDefaultAlgoBarrel(logLevel);
        }
    }
}

void SDL::Segment::runSegmentDefaultAlgoBarrel(SDL::LogLevel logLevel)
{

    const MiniDoublet& innerMiniDoublet = (*innerMiniDoubletPtr());
    const MiniDoublet& outerMiniDoublet = (*outerMiniDoubletPtr());

    const Module& innerLowerModule = innerMiniDoublet.lowerHitPtr()->getModule();
    const Module& outerLowerModule = outerMiniDoublet.lowerHitPtr()->getModule();

    // Constants
    const float kRinv1GeVf = (2.99792458e-3 * 3.8);
    const float k2Rinv1GeVf = kRinv1GeVf / 2.;
    const float ptCut = PTCUT;
    const float sinAlphaMax = 0.95;
    const float deltaZLum = 15.f;
    std::array<float, 6> miniMulsPtScaleBarrel {0.0052, 0.0038, 0.0034, 0.0034, 0.0032, 0.0034};
    // std::array<float, 5> miniMulsPtScaleEndcap {0.006, 0.006, 0.006, 0.006, 0.006}; //inter/extra-polated from L11 and L13 both roughly 0.006 [larger R have smaller value by ~50%]
    const float sdMuls = miniMulsPtScaleBarrel[innerLowerModule.layer()] * 3.f / ptCut * 2.f;//will need a better guess than x2?

    // Get the relevant anchor hits
    const Hit& innerMiniDoubletAnchorHit = (innerLowerModule.moduleType() == SDL::Module::PS) ? ( (innerLowerModule.moduleLayerType() == SDL::Module::Pixel) ? *innerMiniDoublet.lowerHitPtr() : *innerMiniDoublet.upperHitPtr()): *innerMiniDoublet.lowerHitPtr();
    const Hit& outerMiniDoubletAnchorHit = (outerLowerModule.moduleType() == SDL::Module::PS) ? ( (outerLowerModule.moduleLayerType() == SDL::Module::Pixel) ? *outerMiniDoublet.lowerHitPtr() : *outerMiniDoublet.upperHitPtr()): *outerMiniDoublet.lowerHitPtr();

    // MiniDoublet information
    float innerMiniDoubletAnchorHitRt = innerMiniDoubletAnchorHit.rt();
    float outerMiniDoubletAnchorHitRt = outerMiniDoubletAnchorHit.rt();
    float innerMiniDoubletAnchorHitZ = innerMiniDoubletAnchorHit.z();
    float outerMiniDoubletAnchorHitZ = outerMiniDoubletAnchorHit.z();

    // Reco value set
    setRtOut(outerMiniDoubletAnchorHitRt);
    setRtIn(innerMiniDoubletAnchorHitRt);

    const float sdSlope = std::asin(std::min(outerMiniDoubletAnchorHitRt * k2Rinv1GeVf / ptCut, sinAlphaMax));
    const float sdPVoff = 0.1f / outerMiniDoubletAnchorHitRt;
    const float dzDrtScale = std::tan(sdSlope) / sdSlope; //FIXME: need approximate value
    const float pixelPSZpitch = 0.15;
    const float strip2SZpitch = 5.0;

    const float zGeom = (innerLowerModule.layer() <= 2) ? (2.f * pixelPSZpitch) : (2.f * strip2SZpitch); //twice the macro-pixel or strip size

    float zLo = innerMiniDoubletAnchorHitZ + (innerMiniDoubletAnchorHitZ - deltaZLum) * (outerMiniDoubletAnchorHitRt / innerMiniDoubletAnchorHitRt - 1.f) * (innerMiniDoubletAnchorHitZ > 0.f ? 1.f : dzDrtScale) - zGeom; //slope-correction only on outer end
    float zHi = innerMiniDoubletAnchorHitZ + (innerMiniDoubletAnchorHitZ + deltaZLum) * (outerMiniDoubletAnchorHitRt / innerMiniDoubletAnchorHitRt - 1.f) * (innerMiniDoubletAnchorHitZ < 0.f ? 1.f : dzDrtScale) + zGeom;

    setZOut(outerMiniDoubletAnchorHitZ);
    setZLo(zLo);
    setZHi(zHi);
    setZIn(innerMiniDoubletAnchorHitZ);

    // Reset passBitsDefaultAlgo_;
    passBitsDefaultAlgo_ = 0;

    // Cut #1: Z compatibility
    if (not (outerMiniDoubletAnchorHitZ >= zLo and outerMiniDoubletAnchorHitZ <= zHi))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #1 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " zLo: " << zLo <<  " outerMiniDoubletAnchorHitZ: " << outerMiniDoubletAnchorHitZ <<  " zHi: " << zHi <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_SGAlgo);
        return;
    }


/*    if(zHi - zLo >= 50 && innerLowerModule.layer() < 2)
    {
      std::cout<<"zLo="<<zLo<<" "<<"zHi="<<zHi<<" "<<"inner hit="<<innerMiniDoubletAnchorHitZ<<" "<<"inner rt="<<innerMiniDoubletAnchorHitRt<<" outer rt="<<outerMiniDoubletAnchorHitRt<<" deltaZ lum="<<deltaZLum<<" "<<"dzdrtscale="<<dzDrtScale<<" inner layer="<<innerLowerModule.layer()<<" outer layer="<<outerLowerModule.layer()<<" inner module type="<<innerLowerModule.subdet()<<" outer module type="<<outerLowerModule.subdet();
     if(outerLowerModule.subdet() == SDL::Module::Endcap)
     {
         std::cout<<" outer layer ring="<<outerLowerModule.ring();
     }
     else
       std::cout<<" barrel to barrel is strange"<<std::endl;
     std::cout<<std::endl;
     
    }*/

    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << SegmentSelection::deltaZ);

    //TODO:Replace this with Minidoublet::getdeltaphi
    const float sdCut = sdSlope + sqrt(sdMuls * sdMuls + sdPVoff * sdPVoff);
    const float deltaPhi = innerMiniDoubletAnchorHit.deltaPhi(outerMiniDoubletAnchorHit);

    setRecoVars("sdCut", sdCut);
    setRecoVars("sdSlope", sdSlope);
    setRecoVars("sdMuls", sdMuls);
    setRecoVars("sdPVoff", sdPVoff);
    setRecoVars("deltaPhi", deltaPhi);

    // Cut #2: phi differences between the two minidoublets
    if (not (std::abs(deltaPhi) <= sdCut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #2 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " deltaPhi: " << deltaPhi <<  " sdCut: " << sdCut <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_SGAlgo);
        return;
    }

    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << SegmentSelection::deltaPhiPos);

    //TODO:Replace this with MiniDoublet::getdeltaphi
    setDeltaPhiChange(innerMiniDoubletAnchorHit.deltaPhiChange(outerMiniDoubletAnchorHit));

    // Cut #3: phi change between the two minidoublets
    if (not (std::abs(getDeltaPhiChange()) <= sdCut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #3 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " deltaPhiChange: " << getDeltaPhiChange() <<  " sdCut: " << sdCut <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_SGAlgo);
        return;
    }

    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << SegmentSelection::slope);

    float segmentDr = outerMiniDoubletAnchorHit.rt() - innerMiniDoubletAnchorHit.rt();
    float sdZ = innerMiniDoubletAnchorHit.z();
    float sdRt = innerMiniDoubletAnchorHit.rt();

    const float dAlpha_Bfield = std::asin(std::min(segmentDr * k2Rinv1GeVf / ptCut, sinAlphaMax));

    std::array<float, 6> miniDeltaBarrel {0.26, 0.16, 0.16, 0.18, 0.18, 0.18};
    std::array<float, 5> miniDeltaEndcap {0.4 , 0.4 , 0.4 , 0.4 , 0.4};
    std::array<float, 6> miniDeltaTilted {0.26, 0.26, 0.26, 0.4, 0.4, 0.4}; // Used only for tilted modules (i.e. first 3 numbers only matter)

    const bool isInnerTilted = innerLowerModule.subdet() == SDL::Module::Barrel and innerLowerModule.side() != SDL::Module::Center;
    const bool isOuterTilted = outerLowerModule.subdet() == SDL::Module::Barrel and outerLowerModule.side() != SDL::Module::Center;
    const unsigned int innerdetid = (innerLowerModule.moduleLayerType() == SDL::Module::Pixel) ?  innerLowerModule.partnerDetId() : innerLowerModule.detId();
    const unsigned int outerdetid = (outerLowerModule.moduleLayerType() == SDL::Module::Pixel) ?  outerLowerModule.partnerDetId() : outerLowerModule.detId();
    const float drdzinner = tiltedGeometry.getDrDz(innerdetid);
    const float drdzouter = tiltedGeometry.getDrDz(outerdetid);
    const float innerminiTilt = isInnerTilted ? (0.5f * pixelPSZpitch * drdzinner / sqrt(1.f + drdzinner * drdzinner) / miniDeltaTilted[innerLowerModule.layer()-1]) : 0;
    const float outerminiTilt = isOuterTilted ? (0.5f * pixelPSZpitch * drdzouter / sqrt(1.f + drdzouter * drdzouter) / miniDeltaTilted[outerLowerModule.layer()-1]) : 0;

    float miniDelta = (innerLowerModule.subdet() == SDL::Module::Barrel) ? (isInnerTilted ? miniDeltaTilted[innerLowerModule.layer()-1] : miniDeltaBarrel[innerLowerModule.layer()-1]) : (miniDeltaEndcap[innerLowerModule.layer()-1]);
    float sdLumForInnerMini = (SDL::MiniDoublet::useBarrelLogic(innerLowerModule)) ?  (innerminiTilt * dAlpha_Bfield) :  (15.f / innerMiniDoubletAnchorHitZ);
    float sdLumForOuterMini = (SDL::MiniDoublet::useBarrelLogic(outerLowerModule)) ?  (outerminiTilt * dAlpha_Bfield) :  (15.f / outerMiniDoubletAnchorHitZ);

    // float dAlpha_res = 0.04f / miniDelta * (innerLowerModule.subdet() == SDL::Module::Barrel ? 1.0f : std::abs(sdZ / sdRt)); //4-strip difference
    float dAlpha_res = 0.04f / miniDelta * (SDL::MiniDoublet::useBarrelLogic(innerLowerModule) ? 1.0f : std::abs(sdZ / sdRt)); //4-strip difference
    float dAlpha_compat_inner_vs_sg = dAlpha_Bfield + sqrt(dAlpha_res * dAlpha_res + sdMuls * sdMuls/* + sdLumForInnerMini * sdLumForInnerMini*/);
    if(innerLowerModule.subdet() == SDL::Module::Endcap)
        dAlpha_compat_inner_vs_sg += sdLumForInnerMini * sdLumForInnerMini;

    float dAlpha_compat_outer_vs_sg = dAlpha_Bfield + sqrt(dAlpha_res * dAlpha_res + sdMuls * sdMuls/* + sdLumForOuterMini * sdLumForOuterMini*/);
    // float dAlpha_compat_inner_vs_outer = dAlpha_Bfield + sqrt(dAlpha_res * dAlpha_res + sdMuls * sdMuls + sdLumForInnerMini * sdLumForInnerMini + sdLumForOuterMini * sdLumForOuterMini);
    if(innerLowerModule.subdet() == SDL::Module::Endcap)
        dAlpha_compat_outer_vs_sg += sdLumForOuterMini * sdLumForOuterMini;


    float dAlpha_compat_inner_vs_outer = dAlpha_Bfield + sqrt(dAlpha_res * dAlpha_res + sdMuls * sdMuls);

    float inner_md_alpha = innerMiniDoublet.getDeltaPhiChange();
    float outer_md_alpha = outerMiniDoublet.getDeltaPhiChange();
    float sg_alpha = getDeltaPhiChange();

    std::unordered_map<std::string,float> dAlphaCutValues = dAlphaThreshold(innerMiniDoublet,outerMiniDoublet);
    dAlpha_compat_inner_vs_sg = dAlphaCutValues["dAlphaInnerMDSegment"];
    dAlpha_compat_outer_vs_sg = dAlphaCutValues["dAlphaOuterMDSegment"];
    dAlpha_compat_inner_vs_outer = dAlphaCutValues["dAlphaInnerMDOuterMD"];


    // Cut #4: angle compatibility between mini-doublet and segment
    float dAlpha_inner_md_sg = inner_md_alpha - sg_alpha;
    if (not (std::abs(dAlpha_inner_md_sg) < dAlpha_compat_inner_vs_sg))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #4 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " dAlpha_inner_md_sg: " << dAlpha_inner_md_sg <<  " dAlpha_compat_inner_vs_sg: " << dAlpha_compat_inner_vs_sg <<  std::endl;
            SDL::cout <<  " dAlpha_Bfield: " << dAlpha_Bfield <<  " dAlpha_res: " << dAlpha_res <<  " sdMuls: " << sdMuls <<  " sdLumForInnerMini: " << sdLumForInnerMini <<  std::endl;
            SDL::cout <<  " inner_md_alpha: " << inner_md_alpha <<  " sg_alpha: " << sg_alpha <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_SGAlgo);
        return;
    }

    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << SegmentSelection::alphaRef);

    // Cut #5: angle compatibility between mini-doublet and segment
    float dAlpha_outer_md_sg = outer_md_alpha - sg_alpha;
    if (not (std::abs(dAlpha_outer_md_sg) < dAlpha_compat_outer_vs_sg))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #5 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " dAlpha_outer_md_sg: " << dAlpha_outer_md_sg <<  " dAlpha_compat_outer_vs_sg: " << dAlpha_compat_outer_vs_sg <<  std::endl;
            SDL::cout <<  " dAlpha_Bfield: " << dAlpha_Bfield <<  " dAlpha_res: " << dAlpha_res <<  " sdMuls: " << sdMuls <<  " sdLumForOuterMini: " << sdLumForOuterMini <<  std::endl;
            SDL::cout <<  " outer_md_alpha: " << outer_md_alpha <<  " sg_alpha: " << sg_alpha <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_SGAlgo);
        return;
    }

    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << SegmentSelection::alphaOut);

    // Cut #6: angle compatibility between mini-doublet mini-doublets
    float dAlpha_outer_md_inner_md = outer_md_alpha - inner_md_alpha;
    if (not (std::abs(dAlpha_outer_md_inner_md) < dAlpha_compat_inner_vs_outer))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #6 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " dAlpha_outer_md_inner_md: " << dAlpha_outer_md_inner_md <<  " dAlpha_compat_inner_vs_outer: " << dAlpha_compat_inner_vs_outer <<  std::endl;
            SDL::cout <<  " dAlpha_Bfield: " << dAlpha_Bfield <<  " dAlpha_res: " << dAlpha_res <<  " sdMuls: " << sdMuls <<  " sdLumForInnerMini: " << sdLumForInnerMini <<  " sdLumForOuterMini: " << sdLumForOuterMini <<  std::endl;
            SDL::cout <<  " outer_md_alpha: " << outer_md_alpha <<  " inner_md_alpha: " << inner_md_alpha <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_SGAlgo);
        return;
    }

    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << SegmentSelection::alphaRefOut);

    passAlgo_ |= (1 << SDL::Default_SGAlgo);
    return;
}

void SDL::Segment::runSegmentDefaultAlgoEndcap(SDL::LogLevel logLevel)
{
    const MiniDoublet& innerMiniDoublet = (*innerMiniDoubletPtr());
    const MiniDoublet& outerMiniDoublet = (*outerMiniDoubletPtr());

    const Module& innerLowerModule = innerMiniDoublet.lowerHitPtr()->getModule();
    const Module& outerLowerModule = outerMiniDoublet.lowerHitPtr()->getModule();

    // Constants
    const float kRinv1GeVf = (2.99792458e-3 * 3.8);
    const float k2Rinv1GeVf = kRinv1GeVf / 2.;
    const float ptCut = PTCUT;
    const float sinAlphaMax = 0.95;
    const float deltaZLum = 15.f;
    std::array<float, 6> miniMulsPtScaleBarrel {0.0052, 0.0038, 0.0034, 0.0034, 0.0032, 0.0034};
    std::array<float, 5> miniMulsPtScaleEndcap {0.006, 0.006, 0.006, 0.006, 0.006}; //inter/extra-polated from L11 and L13 both roughly 0.006 [larger R have smaller value by ~50%]
    float sdMuls;
    if(innerLowerModule.subdet() == SDL::Module::Endcap)
    {
        sdMuls = miniMulsPtScaleEndcap[innerLowerModule.layer()-1] * 3.f / ptCut;//will need a better guess than x2?
    }
    else
    {
        sdMuls = miniMulsPtScaleBarrel[innerLowerModule.layer()-1] * 3.f / ptCut;
    }

    // Get the relevant anchor hits
    const Hit& innerMiniDoubletAnchorHit = (innerLowerModule.moduleType() == SDL::Module::PS) ? ( (innerLowerModule.moduleLayerType() == SDL::Module::Pixel) ? *innerMiniDoublet.lowerHitPtr() : *innerMiniDoublet.upperHitPtr()): *innerMiniDoublet.lowerHitPtr();
    const Hit& outerMiniDoubletAnchorHit = (outerLowerModule.moduleType() == SDL::Module::PS) ? ( (outerLowerModule.moduleLayerType() == SDL::Module::Pixel) ? *outerMiniDoublet.lowerHitPtr() : *outerMiniDoublet.upperHitPtr()): *outerMiniDoublet.lowerHitPtr();

    // MiniDoublet information
    float innerMiniDoubletAnchorHitRt = innerMiniDoubletAnchorHit.rt();
    float outerMiniDoubletAnchorHitRt = outerMiniDoubletAnchorHit.rt();
    float innerMiniDoubletAnchorHitZ = innerMiniDoubletAnchorHit.z();
    float outerMiniDoubletAnchorHitZ = outerMiniDoubletAnchorHit.z();

    // Reco value set
    setRtOut(outerMiniDoubletAnchorHitRt);
    setRtIn(innerMiniDoubletAnchorHitRt);


    const float sdSlope = std::asin(std::min(outerMiniDoubletAnchorHitRt * k2Rinv1GeVf / ptCut, sinAlphaMax));
    // const float sdPVoff = 0.1f / outerMiniDoubletAnchorHitRt;
    // const float dzDrtScale = std::tan(sdSlope) / sdSlope; //FIXME: need approximate value
    const float pixelPSZpitch = 0.15;
    const float strip2SZpitch = 5.0;
    const float disks2SMinRadius = 60.f;

    // const float zGeom = innerLowerModule.layer() <= 2 ? 2.f * pixelPSZpitch : 2.f * strip2SZpitch; //twice the macro-pixel or strip size

    const float rtGeom = ((innerMiniDoubletAnchorHitRt < disks2SMinRadius && outerMiniDoubletAnchorHitRt < disks2SMinRadius) ? (2.f * pixelPSZpitch)
            : ((innerMiniDoubletAnchorHitRt < disks2SMinRadius || outerMiniDoubletAnchorHitRt < disks2SMinRadius) ? (pixelPSZpitch + strip2SZpitch)
            : (2.f * strip2SZpitch)));

    // float zLo = innerMiniDoubletAnchorHitZ + (innerMiniDoubletAnchorHitZ - deltaZLum) * (outerMiniDoubletAnchorHitRt / innerMiniDoubletAnchorHitRt - 1.f) * (innerMiniDoubletAnchorHitZ > 0.f ? 1.f : dzDrtScale) - zGeom; //slope-correction only on outer end
    // float zHi = innerMiniDoubletAnchorHitZ + (innerMiniDoubletAnchorHitZ + deltaZLum) * (outerMiniDoubletAnchorHitRt / innerMiniDoubletAnchorHitRt - 1.f) * (innerMiniDoubletAnchorHitZ < 0.f ? 1.f : dzDrtScale) + zGeom;

    // Cut #0: preliminary cut (if the combo is between negative and positive don't even bother...)
    if (innerMiniDoubletAnchorHitZ * outerMiniDoubletAnchorHitZ < 0)
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #0 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " innerMiniDoubletAnchorHitZ: " << innerMiniDoubletAnchorHitZ <<  " outerMiniDoubletAnchorHitZ: " << outerMiniDoubletAnchorHitZ <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_SGAlgo);
        return; //do not even accumulate stats for wrong side combinations
    }

    const float dz = outerMiniDoubletAnchorHitZ - innerMiniDoubletAnchorHitZ;

    // Cut #1: Z compatibility
    const float dLum = std::copysign(deltaZLum, innerMiniDoubletAnchorHitZ);
    const float drtDzScale = sdSlope / std::tan(sdSlope); //FIXME: need approximate value
    float rtLo = std::max(innerMiniDoubletAnchorHitRt * (1.f + dz / (innerMiniDoubletAnchorHitZ + dLum) * drtDzScale) - rtGeom, innerMiniDoubletAnchorHitRt - 0.5f * rtGeom); //rt should increase
    float rtHi = innerMiniDoubletAnchorHitRt * (outerMiniDoubletAnchorHitZ - dLum) / (innerMiniDoubletAnchorHitZ - dLum) + rtGeom; //dLum for luminous; rGeom for measurement size; no tanTheta_loc(pt) correction


    setRtLo(rtLo);
    setRtHi(rtHi);

    if (not (outerMiniDoubletAnchorHitRt >= rtLo and outerMiniDoubletAnchorHitRt <= rtHi))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #1 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " rtLo: " << rtLo <<  " outerMiniDoubletAnchorHitRt: " << outerMiniDoubletAnchorHitRt <<  " rtHi: " << rtHi <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_SGAlgo);
        return;
    }

    // Cut #2: dPhi compatibility
    // const float sdLum = deltaZLum / std::abs(innerMiniDoubletAnchorHitZ);
    // const float sdCut = sdSlope + sqrt(sdMuls * sdMuls + sdPVoff * sdPVoff + sdLum * sdLum);
    const float sdCut = sdSlope;
    // const float sdCut = sdSlope + sqrt(sdMuls * sdMuls + sdPVoff * sdPVoff);
    const float dPhiPos = innerMiniDoubletAnchorHit.deltaPhi(outerMiniDoubletAnchorHit);
    setRecoVars("sdCut",sdCut);
    setRecoVars("sdSlope",sdSlope);
    setRecoVars("deltaPhi",dPhiPos);
    if (not (std::abs(dPhiPos) <= sdCut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #2 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " dPhiPos: " << dPhiPos <<  " sdCut: " << sdCut <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_SGAlgo);
        return;
    }

    // Cut #3: dPhi compatibility
    // const float dPhiChange = innerMiniDoubletAnchorHit.deltaPhiChange(outerMiniDoubletAnchorHit); // NOTE When using the full r3 coordinate (this was turned off in slava's code)
    const float dzFrac = dz / innerMiniDoubletAnchorHitZ;
    // const float dPhiChange = dPhiPos / dzFrac * (1.f + dzFrac);
    setDeltaPhiChange(dPhiPos / dzFrac * (1.f + dzFrac));
    if (not (std::abs(getDeltaPhiChange()) <= sdCut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #3 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " dPhiChange: " << getDeltaPhiChange() <<  " sdCut: " << sdCut <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_SGAlgo);
        return;
    }

    float segmentDr = outerMiniDoubletAnchorHit.rt() - innerMiniDoubletAnchorHit.rt();
    float sdZ = innerMiniDoubletAnchorHit.z();
    float sdRt = innerMiniDoubletAnchorHit.rt();
    // float sdZ_outer = outerMiniDoubletAnchorHit.z();
    // float sdRt_outer = outerMiniDoubletAnchorHit.rt();

    const float dAlpha_Bfield = std::asin(std::min(segmentDr * k2Rinv1GeVf / ptCut, sinAlphaMax));

    // std::array<float, 6> miniDeltaBarrel {0.26, 0.16, 0.16, 0.18, 0.18, 0.18};
    // std::array<float, 5> miniDeltaEndcap {0.4 , 0.4 , 0.4 , 0.4 , 0.4};
    
    std::array<float, 6> miniDeltaBarrel {0.26, 0.16, 0.16, 0.18, 0.18, 0.18};
    std::array<float, 5> miniDeltaEndcap {0.4 , 0.4 , 0.4 , 0.4 , 0.4};
    std::array<float, 6> miniDeltaTilted {0.26, 0.26, 0.26, 0.4, 0.4, 0.4}; // Used only for tilted modules (i.e. first 3 numbers only matter)

    const bool isInnerTilted = innerLowerModule.subdet() == SDL::Module::Barrel and innerLowerModule.side() != SDL::Module::Center;
    const bool isOuterTilted = outerLowerModule.subdet() == SDL::Module::Barrel and outerLowerModule.side() != SDL::Module::Center;
    const unsigned int innerdetid = (innerLowerModule.subdet() == SDL::Module::Barrel and innerLowerModule.moduleLayerType() == SDL::Module::Pixel) ?  innerLowerModule.partnerDetId() : innerLowerModule.detId();
    const unsigned int outerdetid = (outerLowerModule.subdet() == SDL::Module::Barrel and outerLowerModule.moduleLayerType() == SDL::Module::Pixel) ?  outerLowerModule.partnerDetId() : outerLowerModule.detId();
    const float drdzinner = tiltedGeometry.getDrDz(innerdetid);
    const float drdzouter = tiltedGeometry.getDrDz(outerdetid);
    const float innerminiTilt = isInnerTilted ? (0.5f * pixelPSZpitch * drdzinner / sqrt(1.f + drdzinner * drdzinner) / miniDeltaTilted[innerLowerModule.layer()-1]) : 0;
    const float outerminiTilt = isOuterTilted ? (0.5f * pixelPSZpitch * drdzouter / sqrt(1.f + drdzouter * drdzouter) / miniDeltaTilted[outerLowerModule.layer()-1]) : 0;

    float miniDelta;
    if(innerLowerModule.subdet() == SDL::Module::Barrel)
    {
        if(isInnerTilted)
        {
            if(SDL::MiniDoublet::useBarrelLogic(innerLowerModule))
            {
                miniDelta = miniDeltaTilted[innerLowerModule.layer()-1];
            }
            else
            {
                miniDelta = miniDeltaEndcap[innerLowerModule.layer()-1];
            }
        }
        else
        {
            miniDelta = miniDeltaBarrel[innerLowerModule.layer()-1];
        }
    }
    else
    {
        miniDelta = miniDeltaEndcap[innerLowerModule.layer()-1];
    }

    //float miniDelta = (innerLowerModule.subdet() == SDL::Module::Barrel) ? (isInnerTilted ? miniDeltaTilted[innerLowerModule.layer()-1] : miniDeltaBarrel[innerLowerModule.layer()-1]) : (miniDeltaEndcap[innerLowerModule.layer()-1]);

    //The above variable values don't matter for endcap->endcap
    float sdLumForInnerMini = (SDL::MiniDoublet::useBarrelLogic(innerLowerModule)) ?  (innerminiTilt * dAlpha_Bfield) :  (15.f / innerMiniDoubletAnchorHitZ);
    float sdLumForOuterMini = (SDL::MiniDoublet::useBarrelLogic(outerLowerModule)) ?  (outerminiTilt * dAlpha_Bfield) :  (15.f / outerMiniDoubletAnchorHitZ);


    //float miniDelta = 0.4f;
    //float sdLumForInnerMini = 15.f / innerMiniDoubletAnchorHitZ;
    //float sdLumForOuterMini = 15.f / outerMiniDoubletAnchorHitZ;

    // float dAlpha_res = 0.04f / miniDelta * (innerLowerModule.subdet() == SDL::Module::Barrel ? 1.0f : std::abs(sdZ / sdRt)); //4-strip difference
//    float dAlpha_res = 0.04f / miniDelta * std::abs(sdZ / (sdRt/*-10.f*/)); //4-strip difference
    float dAlpha_res = 0.04f / miniDelta * (SDL::MiniDoublet::useBarrelLogic(innerLowerModule) ? 1.0f : std::abs(sdZ / sdRt)); //4-strip difference 
    float dAlpha_uncRt = 15.0f / sdRt * dAlpha_res;

    float dAlpha_compat_inner_vs_sg = dAlpha_Bfield + sqrt(dAlpha_res * dAlpha_res + sdMuls * sdMuls);// + sdLumForInnerMini * sdLumForInnerMini);
    if(innerLowerModule.subdet() == SDL::Module::Endcap)
        dAlpha_compat_inner_vs_sg += sdLumForInnerMini * sdLumForInnerMini;

    float dAlpha_compat_outer_vs_sg = dAlpha_Bfield + sqrt(dAlpha_res * dAlpha_res + sdMuls * sdMuls);// + sdLumForOuterMini * sdLumForOuterMini);
    if(innerLowerModule.subdet() == SDL::Module::Endcap)
        dAlpha_compat_outer_vs_sg += sdLumForOuterMini * sdLumForOuterMini;

    float dAlpha_compat_inner_vs_outer = dAlpha_Bfield + sqrt(dAlpha_res * dAlpha_res + sdMuls * sdMuls);
    // float dAlpha_compat_inner_vs_outer = dAlpha_Bfield + sqrt(dAlpha_res * dAlpha_res + sdMuls * sdMuls + sdLumForInnerMini * sdLumForInnerMini + sdLumForOuterMini * sdLumForOuterMini);
    // float dAlpha_compat_inner_vs_outer = dAlpha_Bfield + sqrt(dAlpha_res * dAlpha_res + dAlpha_uncRt * dAlpha_uncRt + sdMuls * sdMuls);

    float inner_md_alpha = innerMiniDoublet.getDeltaPhiChange();
    float outer_md_alpha = outerMiniDoublet.getDeltaPhiChange();
    float sg_alpha = getDeltaPhiChange();

    //Using the new functions
    std::unordered_map<std::string,float> dAlphaCutValues = dAlphaThreshold(innerMiniDoublet,outerMiniDoublet);
    dAlpha_compat_inner_vs_sg = dAlphaCutValues["dAlphaInnerMDSegment"];
    dAlpha_compat_outer_vs_sg = dAlphaCutValues["dAlphaOuterMDSegment"];
    dAlpha_compat_inner_vs_outer = dAlphaCutValues["dAlphaInnerMDOuterMD"];

    // Cut #4: angle compatibility between mini-doublet and segment
    float dAlpha_inner_md_sg = inner_md_alpha - sg_alpha;
    if (not (std::abs(dAlpha_inner_md_sg) < dAlpha_compat_inner_vs_sg))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #4 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " dAlpha_inner_md_sg: " << dAlpha_inner_md_sg <<  " dAlpha_compat_inner_vs_sg: " << dAlpha_compat_inner_vs_sg <<  std::endl;
            SDL::cout <<  " inner_md_alpha: " << inner_md_alpha <<  " sg_alpha: " << sg_alpha <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_SGAlgo);
        return;
    }

    // Cut #5: angle compatibility between mini-doublet and segment
    float dAlpha_outer_md_sg = outer_md_alpha - sg_alpha;
    if (not (std::abs(dAlpha_outer_md_sg) < dAlpha_compat_outer_vs_sg))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #5 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " dAlpha_outer_md_sg: " << dAlpha_outer_md_sg <<  " dAlpha_compat_outer_vs_sg: " << dAlpha_compat_outer_vs_sg <<  std::endl;
            SDL::cout <<  " outer_md_alpha: " << outer_md_alpha <<  " sg_alpha: " << sg_alpha <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_SGAlgo);
        return;
    }

    // Cut #6: angle compatibility between mini-doublet mini-doublets
    float dAlpha_outer_md_inner_md = outer_md_alpha - inner_md_alpha;
    if (not (std::abs(dAlpha_outer_md_inner_md) < dAlpha_compat_inner_vs_outer))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #6 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " dAlpha_outer_md_inner_md: " << dAlpha_outer_md_inner_md <<  " dAlpha_compat_inner_vs_outer: " << dAlpha_compat_inner_vs_outer <<  std::endl;
            SDL::cout <<  " dAlpha_Bfield: " << dAlpha_Bfield <<  " dAlpha_res: " << dAlpha_res <<  " sdMuls: " << sdMuls <<  " dAlpha_uncRt: " << dAlpha_uncRt <<  " sdLumForInnerMini: " << sdLumForInnerMini <<  " sdLumForOuterMini: " << sdLumForOuterMini <<  std::endl;
            SDL::cout <<  " sdZ: " << sdZ <<  " sdRt: " << sdRt <<  " miniDelta: " << miniDelta <<  std::endl;
            SDL::cout <<  " segmentDr: " << segmentDr <<  " k2Rinv1GeVf: " << k2Rinv1GeVf <<  " ptCut: " << ptCut <<  " sinAlphaMax: " << sinAlphaMax <<  std::endl;
            SDL::cout <<  " outer_md_alpha: " << outer_md_alpha <<  " inner_md_alpha: " << inner_md_alpha <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_SGAlgo);
        return;
    }

    passAlgo_ |= (1 << SDL::Default_SGAlgo);
    return;
}

bool SDL::Segment::hasCommonMiniDoublet(const Segment& outer_sg) const
{
    if (not outerMiniDoubletPtr()->isIdxMatched(*(outer_sg.innerMiniDoubletPtr())))
        return false;
    return true;
}

bool SDL::Segment::isIdxMatched(const Segment& sg) const
{
    if (not innerMiniDoubletPtr_->isIdxMatched(*(sg.innerMiniDoubletPtr())))
        return false;
    if (not outerMiniDoubletPtr_->isIdxMatched(*(sg.outerMiniDoubletPtr())))
        return false;
    return true;
}

bool SDL::Segment::isAnchorHitIdxMatched(const Segment& sg) const
{
    if (not innerMiniDoubletPtr_->isAnchorHitIdxMatched(*(sg.innerMiniDoubletPtr())))
        return false;
    if (not outerMiniDoubletPtr_->isAnchorHitIdxMatched(*(sg.outerMiniDoubletPtr())))
        return false;
    return true;
}

std::unordered_map<std::string,float> SDL::Segment::dAlphaThreshold(const SDL::MiniDoublet &innerMiniDoublet, const SDL::MiniDoublet &outerMiniDoublet)
{

    std::unordered_map<std::string,float> dAlphaValues;
    const Module& innerLowerModule = innerMiniDoublet.lowerHitPtr()->getModule();
    const Module& outerLowerModule = outerMiniDoublet.lowerHitPtr()->getModule();
    const float kRinv1GeVf = (2.99792458e-3 * 3.8);
    const float k2Rinv1GeVf = kRinv1GeVf / 2.;
    float ptCut = PTCUT;
    float sinAlphaMax = 0.95;

    std::array<float, 6> miniMulsPtScaleBarrel {0.0052, 0.0038, 0.0034, 0.0034, 0.0032, 0.0034};
    std::array<float, 5> miniMulsPtScaleEndcap {0.006, 0.006, 0.006, 0.006, 0.006}; //inter/extra-polated from L11 and L13 both roughly 0.006 [larger R have smaller value by ~50%]

    std::array<float, 6> miniDeltaBarrel {0.26, 0.16, 0.16, 0.18, 0.18, 0.18};
    std::array<float, 5> miniDeltaEndcap {0.4 , 0.4 , 0.4 , 0.4 , 0.4};
    std::array<float, 6> miniDeltaTilted {0.26, 0.26, 0.26, 0.4, 0.4, 0.4}; // Used only for tilted modules (i.e. first 3 numbers only matter)
    
    float sdMuls = (innerLowerModule.subdet() == SDL::Module::Barrel) ? miniMulsPtScaleBarrel[innerLowerModule.layer()-1] * 3.f/ptCut : miniMulsPtScaleEndcap[innerLowerModule.layer()-1] * 3.f/ptCut;

    //BField dAlpha
    const Hit& innerMiniDoubletAnchorHit = (innerLowerModule.moduleType() == SDL::Module::PS) ? ( (innerLowerModule.moduleLayerType() == SDL::Module::Pixel) ? *innerMiniDoublet.lowerHitPtr() : *innerMiniDoublet.upperHitPtr()): *innerMiniDoublet.lowerHitPtr();
    const Hit& outerMiniDoubletAnchorHit = (outerLowerModule.moduleType() == SDL::Module::PS) ? ( (outerLowerModule.moduleLayerType() == SDL::Module::Pixel) ? *outerMiniDoublet.lowerHitPtr() : *outerMiniDoublet.upperHitPtr()): *outerMiniDoublet.lowerHitPtr();

    float innerMiniDoubletAnchorHitRt = innerMiniDoubletAnchorHit.rt();
    float outerMiniDoubletAnchorHitRt = outerMiniDoubletAnchorHit.rt();
    float innerMiniDoubletAnchorHitZ = innerMiniDoubletAnchorHit.z();
    float outerMiniDoubletAnchorHitZ = outerMiniDoubletAnchorHit.z();


    float segmentDr = outerMiniDoubletAnchorHitRt - innerMiniDoubletAnchorHitRt; 
    const float dAlpha_Bfield = std::asin(std::min(segmentDr * k2Rinv1GeVf/ptCut, sinAlphaMax));
    const float pixelPSZpitch = 0.15;

    const bool isInnerTilted = innerLowerModule.subdet() == SDL::Module::Barrel and innerLowerModule.side() != SDL::Module::Center;
    const bool isOuterTilted = outerLowerModule.subdet() == SDL::Module::Barrel and outerLowerModule.side() != SDL::Module::Center;
    const unsigned int innerdetid = (innerLowerModule.moduleLayerType() == SDL::Module::Pixel) ?  innerLowerModule.partnerDetId() : innerLowerModule.detId();
    const unsigned int outerdetid = (outerLowerModule.moduleLayerType() == SDL::Module::Pixel) ?  outerLowerModule.partnerDetId() : outerLowerModule.detId();
    const float drdzinner = tiltedGeometry.getDrDz(innerdetid);
    const float drdzouter = tiltedGeometry.getDrDz(outerdetid);
    const float innerminiTilt = isInnerTilted ? (0.5f * pixelPSZpitch * drdzinner / sqrt(1.f + drdzinner * drdzinner) / miniDeltaTilted[innerLowerModule.layer()-1]) : 0;
    const float outerminiTilt = isOuterTilted ? (0.5f * pixelPSZpitch * drdzouter / sqrt(1.f + drdzouter * drdzouter) / miniDeltaTilted[outerLowerModule.layer()-1]) : 0;

    float miniDelta; 
    if(innerLowerModule.subdet() == SDL::Module::Barrel)
    {
        if(isInnerTilted)
        {
            if(SDL::MiniDoublet::useBarrelLogic(innerLowerModule))
            {
                miniDelta = miniDeltaTilted[innerLowerModule.layer()-1];
            }
            else
            {
                miniDelta = miniDeltaEndcap[innerLowerModule.layer()-1];
            }
        }
        else
        {
            miniDelta = miniDeltaBarrel[innerLowerModule.layer()-1];
        }
    }
    else
    {
        miniDelta = miniDeltaEndcap[innerLowerModule.layer()-1];
    }
 

    float sdLumForInnerMini = (SDL::MiniDoublet::useBarrelLogic(innerLowerModule)) ?  (innerminiTilt * dAlpha_Bfield) :  (15.f / innerMiniDoubletAnchorHitZ);
    float sdLumForOuterMini = (SDL::MiniDoublet::useBarrelLogic(outerLowerModule)) ?  (outerminiTilt * dAlpha_Bfield) :  (15.f / outerMiniDoubletAnchorHitZ);


    //Unique stuff for the segment dudes alone
    float dAlpha_res = 0.04f/miniDelta * (SDL::MiniDoublet::useBarrelLogic(innerLowerModule) ? 1.0f : std::abs(innerMiniDoubletAnchorHitZ/innerMiniDoubletAnchorHitRt));

    //TODO:Check if PVoff is required
    if(innerLowerModule.subdet() == SDL::Module::Barrel)
    {
        dAlphaValues["dAlphaInnerMDSegment"] = dAlpha_Bfield + sqrt(pow(dAlpha_res,2) + pow(sdMuls,2));
        dAlphaValues["dAlphaOuterMDSegment"]  = dAlpha_Bfield + sqrt(pow(dAlpha_res,2) + pow(sdMuls,2));
        dAlphaValues["dAlphaInnerMDOuterMD"] = dAlpha_Bfield + sqrt(pow(dAlpha_res,2) + pow(sdMuls,2));
 
    }
    else
    {
        dAlphaValues["dAlphaInnerMDSegment"] = dAlpha_Bfield + sqrt(pow(dAlpha_res,2) + pow(sdMuls,2) + pow(sdLumForInnerMini,2));
        dAlphaValues["dAlphaOuterMDSegment"] = dAlpha_Bfield + sqrt(pow(dAlpha_res,2) + pow(sdMuls,2) + pow(sdLumForOuterMini,2));
        dAlphaValues["dAlphaInnerMDOuterMD"] = dAlpha_Bfield + sqrt(pow(dAlpha_res,2) + pow(sdMuls,2));

    }

    return dAlphaValues;
}

[[deprecated("SDL:: isMiniDoubletPairASegment() is deprecated")]]
bool SDL::Segment::isMiniDoubletPairASegment(const MiniDoublet& innerMiniDoublet, const MiniDoublet& outerMiniDoublet, SGAlgo algo, SDL::LogLevel logLevel)
{
    // If the algorithm is "do all combination" (e.g. used for efficiency calculation)
    if (algo == SDL::AllComb_SGAlgo)
    {
        return true;
    }
    else if (algo == SDL::Default_SGAlgo)
    {
        const Module& innerLowerModule = innerMiniDoublet.lowerHitPtr()->getModule();
        // const Module& outerLowerModule = outerMiniDoublet.lowerHitPtr()->getModule();
        // Port your favorite segment formation algorithm code here
        // Case 1: Barrel - Barrel
        // if (innerLowerModule.subdet() == SDL::Module::Barrel and outerLowerModule.subdet() == SDL::Module::Barrel)
        if (innerLowerModule.subdet() == SDL::Module::Barrel)
        {
            if (not isMiniDoubletPairASegmentCandidateBarrel(innerMiniDoublet, outerMiniDoublet, algo, logLevel))
                return false;
        }
        else // if (innerLowerModule.subdet() == SDL::Module::Endcap)
        {
            if (not isMiniDoubletPairASegmentCandidateEndcap(innerMiniDoublet, outerMiniDoublet, algo, logLevel))
                return false;
        }
        return true;
    }
    else
    {
        SDL::cout << "Warning: Unrecognized segment algorithm!" << algo << std::endl;
        return false;
    }
}

bool SDL::Segment::isMiniDoubletPairASegmentCandidateBarrel(const MiniDoublet& innerMiniDoublet, const MiniDoublet& outerMiniDoublet, SGAlgo algo, SDL::LogLevel logLevel)
{

    const Module& innerLowerModule = innerMiniDoublet.lowerHitPtr()->getModule();
    const Module& outerLowerModule = outerMiniDoublet.lowerHitPtr()->getModule();

    // Constants
    const float kRinv1GeVf = (2.99792458e-3 * 3.8);
    const float k2Rinv1GeVf = kRinv1GeVf / 2.;
    const float ptCut = PTCUT;
    const float sinAlphaMax = 0.95;
    const float deltaZLum = 15.f;
    std::array<float, 6> miniMulsPtScaleBarrel {0.0052, 0.0038, 0.0034, 0.0034, 0.0032, 0.0034};
    // std::array<float, 5> miniMulsPtScaleEndcap {0.006, 0.006, 0.006, 0.006, 0.006}; //inter/extra-polated from L11 and L13 both roughly 0.006 [larger R have smaller value by ~50%]
    const float sdMuls = miniMulsPtScaleBarrel[innerLowerModule.layer()] * 3.f / ptCut * 2.f;//will need a better guess than x2?

    // Get the relevant anchor hits
    const Hit& innerMiniDoubletAnchorHit = innerLowerModule.moduleType() == SDL::Module::PS ? ( (innerLowerModule.moduleLayerType() == SDL::Module::Pixel) ? *innerMiniDoublet.lowerHitPtr() : *innerMiniDoublet.upperHitPtr()): *innerMiniDoublet.lowerHitPtr();
    const Hit& outerMiniDoubletAnchorHit = outerLowerModule.moduleType() == SDL::Module::PS ? ( (outerLowerModule.moduleLayerType() == SDL::Module::Pixel) ? *outerMiniDoublet.lowerHitPtr() : *outerMiniDoublet.upperHitPtr()): *outerMiniDoublet.lowerHitPtr();

    // MiniDoublet information
    float innerMiniDoubletAnchorHitRt = innerMiniDoubletAnchorHit.rt();
    float outerMiniDoubletAnchorHitRt = outerMiniDoubletAnchorHit.rt();
    float innerMiniDoubletAnchorHitZ = innerMiniDoubletAnchorHit.z();
    float outerMiniDoubletAnchorHitZ = outerMiniDoubletAnchorHit.z();

    const float sdSlope = std::asin(std::min(outerMiniDoubletAnchorHitRt * k2Rinv1GeVf / ptCut, sinAlphaMax));
    const float sdPVoff = 0.1f / outerMiniDoubletAnchorHitRt;
    const float dzDrtScale = std::tan(sdSlope) / sdSlope; //FIXME: need approximate value
    const float pixelPSZpitch = 0.15;
    const float strip2SZpitch = 5.0;

    const float zGeom = (innerLowerModule.layer() <= 2) ? (2.f * pixelPSZpitch) : (2.f * strip2SZpitch); //twice the macro-pixel or strip size

    float zLo = innerMiniDoubletAnchorHitZ + (innerMiniDoubletAnchorHitZ - deltaZLum) * (outerMiniDoubletAnchorHitRt / innerMiniDoubletAnchorHitRt - 1.f) * (innerMiniDoubletAnchorHitZ > 0.f ? 1.f : dzDrtScale) - zGeom; //slope-correction only on outer end
    float zHi = innerMiniDoubletAnchorHitZ + (innerMiniDoubletAnchorHitZ + deltaZLum) * (outerMiniDoubletAnchorHitRt / innerMiniDoubletAnchorHitRt - 1.f) * (innerMiniDoubletAnchorHitZ < 0.f ? 1.f : dzDrtScale) + zGeom;

    // Cut #1: Z compatibility
    if (not (outerMiniDoubletAnchorHitZ >= zLo and outerMiniDoubletAnchorHitZ <= zHi))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #1 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " zLo: " << zLo <<  " outerMiniDoubletAnchorHitZ: " << outerMiniDoubletAnchorHitZ <<  " zHi: " << zHi <<  std::endl;
        }
        return false;
    }

    const float sdCut = sdSlope + sqrt(sdMuls * sdMuls + sdPVoff * sdPVoff);
    const float deltaPhi = innerMiniDoubletAnchorHit.deltaPhi(outerMiniDoubletAnchorHit);

    // Cut #2: phi differences between the two minidoublets
    if (not (std::abs(deltaPhi) <= sdCut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #2 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " deltaPhi: " << deltaPhi <<  " sdCut: " << sdCut <<  std::endl;
        }
        return false;
    }

    float dPhiChange = innerMiniDoubletAnchorHit.deltaPhiChange(outerMiniDoubletAnchorHit);

    // Cut #3: phi change between the two minidoublets
    if (not (std::abs(dPhiChange) <= sdCut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #3 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " deltaPhiChange: " << dPhiChange <<  " sdCut: " << sdCut <<  std::endl;
        }
        return false;
    }

    return true;
}

bool SDL::Segment::isMiniDoubletPairASegmentCandidateEndcap(const MiniDoublet& innerMiniDoublet, const MiniDoublet& outerMiniDoublet, SGAlgo algo, SDL::LogLevel logLevel)
{

    const Module& innerLowerModule = innerMiniDoublet.lowerHitPtr()->getModule();
    const Module& outerLowerModule = outerMiniDoublet.lowerHitPtr()->getModule();

    // Constants
    const float kRinv1GeVf = (2.99792458e-3 * 3.8);
    const float k2Rinv1GeVf = kRinv1GeVf / 2.;
    const float ptCut = PTCUT;
    const float sinAlphaMax = 0.95;
    const float deltaZLum = 15.f;
    // std::array<float, 6> miniMulsPtScaleBarrel {0.0052, 0.0038, 0.0034, 0.0034, 0.0032, 0.0034};
    // std::array<float, 5> miniMulsPtScaleEndcap {0.006, 0.006, 0.006, 0.006, 0.006}; //inter/extra-polated from L11 and L13 both roughly 0.006 [larger R have smaller value by ~50%]
    // const float sdMuls = miniMulsPtScaleEndcap[innerLowerModule.layer()] * 3.f / ptCut * 2.f;//will need a better guess than x2?

    // Get the relevant anchor hits
    const Hit& innerMiniDoubletAnchorHit = innerLowerModule.moduleType() == SDL::Module::PS ? ( innerLowerModule.moduleLayerType() == SDL::Module::Pixel ? *innerMiniDoublet.lowerHitPtr() : *innerMiniDoublet.upperHitPtr()): *innerMiniDoublet.lowerHitPtr();
    const Hit& outerMiniDoubletAnchorHit = outerLowerModule.moduleType() == SDL::Module::PS ? ( outerLowerModule.moduleLayerType() == SDL::Module::Pixel ? *outerMiniDoublet.lowerHitPtr() : *outerMiniDoublet.upperHitPtr()): *outerMiniDoublet.lowerHitPtr();

    // MiniDoublet information
    float innerMiniDoubletAnchorHitRt = innerMiniDoubletAnchorHit.rt();
    float outerMiniDoubletAnchorHitRt = outerMiniDoubletAnchorHit.rt();
    float innerMiniDoubletAnchorHitZ = innerMiniDoubletAnchorHit.z();
    float outerMiniDoubletAnchorHitZ = outerMiniDoubletAnchorHit.z();

    const float sdSlope = std::asin(std::min(outerMiniDoubletAnchorHitRt * k2Rinv1GeVf / ptCut, sinAlphaMax));
    // const float sdPVoff = 0.1f / outerMiniDoubletAnchorHitRt;
    // const float dzDrtScale = std::tan(sdSlope) / sdSlope; //FIXME: need approximate value
    const float pixelPSZpitch = 0.15;
    const float strip2SZpitch = 5.0;
    const float disks2SMinRadius = 60.f;

    // const float zGeom = innerLowerModule.layer() <= 2 ? 2.f * pixelPSZpitch : 2.f * strip2SZpitch; //twice the macro-pixel or strip size

    const float rtGeom = ((innerMiniDoubletAnchorHitRt < disks2SMinRadius && outerMiniDoubletAnchorHitRt < disks2SMinRadius) ? (2.f * pixelPSZpitch)
            : ((innerMiniDoubletAnchorHitRt < disks2SMinRadius || outerMiniDoubletAnchorHitRt < disks2SMinRadius) ? (pixelPSZpitch + strip2SZpitch)
            : (2.f * strip2SZpitch)));

    // float zLo = innerMiniDoubletAnchorHitZ + (innerMiniDoubletAnchorHitZ - deltaZLum) * (outerMiniDoubletAnchorHitRt / innerMiniDoubletAnchorHitRt - 1.f) * (innerMiniDoubletAnchorHitZ > 0.f ? 1.f : dzDrtScale) - zGeom; //slope-correction only on outer end
    // float zHi = innerMiniDoubletAnchorHitZ + (innerMiniDoubletAnchorHitZ + deltaZLum) * (outerMiniDoubletAnchorHitRt / innerMiniDoubletAnchorHitRt - 1.f) * (innerMiniDoubletAnchorHitZ < 0.f ? 1.f : dzDrtScale) + zGeom;

    // Cut #0: preliminary cut (if the combo is between negative and positive don't even bother...)
    if (innerMiniDoubletAnchorHitZ * outerMiniDoubletAnchorHitZ < 0)
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #0 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " innerMiniDoubletAnchorHitZ: " << innerMiniDoubletAnchorHitZ <<  " outerMiniDoubletAnchorHitZ: " << outerMiniDoubletAnchorHitZ <<  std::endl;
        }
        return false; //do not even accumulate stats for wrong side combinations
    }

    const float dz = outerMiniDoubletAnchorHitZ - innerMiniDoubletAnchorHitZ;

    // Cut #1: Z compatibility
    const float dLum = std::copysign(deltaZLum, innerMiniDoubletAnchorHitZ);
    const float drtDzScale = sdSlope / std::tan(sdSlope); //FIXME: need approximate value
    float rtLo = std::max(innerMiniDoubletAnchorHitRt * (1.f + dz / (innerMiniDoubletAnchorHitZ + dLum) * drtDzScale) - rtGeom, innerMiniDoubletAnchorHitRt - 0.5f * rtGeom); //rt should increase
    float rtHi = innerMiniDoubletAnchorHitRt * (outerMiniDoubletAnchorHitZ - dLum) / (innerMiniDoubletAnchorHitZ - dLum) + rtGeom; //dLum for luminous; rGeom for measurement size; no tanTheta_loc(pt) correction
    if (not (outerMiniDoubletAnchorHitRt >= rtLo and outerMiniDoubletAnchorHitRt <= rtHi))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #1 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " rtLo: " << rtLo <<  " outerMiniDoubletAnchorHitRt: " << outerMiniDoubletAnchorHitRt <<  " rtHi: " << rtHi <<  std::endl;
        }
        return false;
    }

    // Cut #2: dPhi compatibility
    // const float sdLum = deltaZLum / std::abs(innerMiniDoubletAnchorHitZ);
    // const float sdCut = sdSlope + sqrt(sdMuls * sdMuls + sdPVoff * sdPVoff + sdLum * sdLum);
    const float sdCut = sdSlope;
    // const float sdCut = sdSlope + sqrt(sdMuls * sdMuls + sdPVoff * sdPVoff);
    const float dPhiPos = innerMiniDoubletAnchorHit.deltaPhi(outerMiniDoubletAnchorHit);
    if (not (std::abs(dPhiPos) <= sdCut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #2 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " dPhiPos: " << dPhiPos <<  " sdCut: " << sdCut <<  std::endl;
        }
        return false;
    }

    // Cut #3: dPhi compatibility
    // const float dPhiChange = innerMiniDoubletAnchorHit.deltaPhiChange(outerMiniDoubletAnchorHit); // NOTE When using the full r3 coordinate (this was turned off in slava's code)
    const float dzFrac = dz / innerMiniDoubletAnchorHitZ;
    const float dPhiChange = dPhiPos / dzFrac * (1.f + dzFrac);
    if (not (std::abs(dPhiChange) <= sdCut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #3 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " dPhiChange: " << dPhiChange <<  " sdCut: " << sdCut <<  std::endl;
        }
        return false;
    }

    return true;
}


namespace SDL
{
    std::ostream& operator<<(std::ostream& out, const Segment& sg)
    {
        out << "sg_dPhiChange " << sg.getDeltaPhiChange() << std::endl;
        out << "ptestimate " << SDL::MathUtil::ptEstimateFromDeltaPhiChangeAndRt(sg.getDeltaPhiChange(), sg.getRtOut()) << std::endl;
        out << std::endl;
        out << "Inner Mini-Doublet" << std::endl;
        out << "------------------------------" << std::endl;
        {
            IndentingOStreambuf indent(out);
            out << sg.innerMiniDoubletPtr_ << std::endl;
        }
        out << "Outer Mini-Doublet" << std::endl;
        out << "------------------------------" << std::endl;
        {
            IndentingOStreambuf indent(out);
            out << sg.outerMiniDoubletPtr_;
        }
        // out << "Inner MD Module " << std::endl;
        // out << sg.innerMiniDoubletPtr_->lowerHitPtr()->getModule();
        // out << "outer MD Module " << std::endl;
        // out << sg.outerMiniDoubletPtr_->lowerHitPtr()->getModule();
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const Segment* sg)
    {
        out << *sg;
        return out;
    }
}

