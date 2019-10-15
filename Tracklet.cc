#include "Tracklet.h"

SDL::Tracklet::Tracklet()
{
}

SDL::Tracklet::~Tracklet()
{
}

SDL::Tracklet::Tracklet(const Tracklet& tl) :
    innerSegmentPtr_(tl.innerSegmentPtr()),
    outerSegmentPtr_(tl.outerSegmentPtr()),
    passAlgo_(tl.getPassAlgo()),
    passBitsDefaultAlgo_(tl.getPassBitsDefaultAlgo()),
    deltaBeta_(tl.getDeltaBeta()),
    deltaBetaCut_(tl.getDeltaBetaCut()),
    betaIn_(tl.getBetaIn()),
    betaInCut_(tl.getBetaInCut()),
    betaOut_(tl.getBetaOut()),
    betaOutCut_(tl.getBetaOutCut()),
    recovars_(tl.getRecoVars())
{
}

SDL::Tracklet::Tracklet(SDL::Segment* innerSegmentPtr, SDL::Segment* outerSegmentPtr) :
    innerSegmentPtr_(innerSegmentPtr),
    outerSegmentPtr_(outerSegmentPtr),
    passAlgo_(0),
    passBitsDefaultAlgo_(0),
    deltaBeta_(0),
    deltaBetaCut_(0),
    betaIn_(0),
    betaInCut_(0),
    betaOut_(0),
    betaOutCut_(0)
{
}

SDL::Segment* SDL::Tracklet::innerSegmentPtr() const
{
    return innerSegmentPtr_;
}

SDL::Segment* SDL::Tracklet::outerSegmentPtr() const
{
    return outerSegmentPtr_;
}

const int& SDL::Tracklet::getPassAlgo() const
{
    return passAlgo_;
}

const int& SDL::Tracklet::getPassBitsDefaultAlgo() const
{
    return passBitsDefaultAlgo_;
}

const float& SDL::Tracklet::getDeltaBeta() const
{
    return deltaBeta_;
}

void SDL::Tracklet::setDeltaBeta(float deltaBeta)
{
    deltaBeta_ = deltaBeta;
}

const float& SDL::Tracklet::getDeltaBetaCut() const
{
    return deltaBetaCut_;
}

void SDL::Tracklet::setDeltaBetaCut(float deltaBetaCut)
{
    deltaBetaCut_ = deltaBetaCut;
}

const float& SDL::Tracklet::getBetaIn() const
{
    return betaIn_;
}

void SDL::Tracklet::setBetaIn(float betaIn)
{
    betaIn_ = betaIn;
}

const float& SDL::Tracklet::getBetaInCut() const
{
    return betaInCut_;
}

void SDL::Tracklet::setBetaInCut(float betaInCut)
{
    betaInCut_ = betaInCut;
}

const float& SDL::Tracklet::getBetaOut() const
{
    return betaOut_;
}

void SDL::Tracklet::setBetaOut(float betaOut)
{
    betaOut_ = betaOut;
}

const float& SDL::Tracklet::getBetaOutCut() const
{
    return betaOutCut_;
}

void SDL::Tracklet::setBetaOutCut(float betaOutCut)
{
    betaOutCut_ = betaOutCut;
}

const std::map<std::string, float>& SDL::Tracklet::getRecoVars() const
{
    return recovars_;
}

const float& SDL::Tracklet::getRecoVar(std::string key) const
{
    return recovars_.at(key);
}

void SDL::Tracklet::setRecoVars(std::string key, float var)
{
    recovars_[key] = var;
}

bool SDL::Tracklet::passesTrackletAlgo(SDL::TLAlgo algo) const
{
    // Each algorithm is an enum shift it by its value and check against the flag
    return passAlgo_ & (1 << algo);
}

void SDL::Tracklet::runTrackletAlgo(SDL::TLAlgo algo, SDL::LogLevel logLevel)
{
    if (algo == SDL::AllComb_TLAlgo)
    {
        runTrackletAllCombAlgo();
    }
    else if (algo == SDL::Default_TLAlgo)
    {
        runTrackletDefaultAlgo(logLevel);
    }
    else
    {
        SDL::cout << "Warning: Unrecognized segment algorithm!" << algo << std::endl;
        return;
    }
}

void SDL::Tracklet::runTrackletAllCombAlgo()
{
    passAlgo_ |= (1 << SDL::AllComb_TLAlgo);
}

void SDL::Tracklet::runTrackletDefaultAlgo(SDL::LogLevel logLevel)
{
    // Retreived the lower module object
    const Module& innerSgInnerMDLowerHitModule = innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->getModule();
    const Module& outerSgInnerMDLowerHitModule = outerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->getModule();
    const Module& innerSgOuterMDLowerHitModule = innerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr()->getModule();
    const Module& outerSgOuterMDLowerHitModule = outerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr()->getModule();

    if (innerSgInnerMDLowerHitModule.subdet() == SDL::Module::Barrel
            and outerSgInnerMDLowerHitModule.subdet() == SDL::Module::Barrel
            and innerSgOuterMDLowerHitModule.subdet() == SDL::Module::Barrel
            and outerSgOuterMDLowerHitModule.subdet() == SDL::Module::Barrel
            )
    {
        runTrackletDefaultAlgoBarrelBarrelBarrelBarrel(logLevel);
    }
    else if (innerSgInnerMDLowerHitModule.subdet() == SDL::Module::Barrel
            and outerSgInnerMDLowerHitModule.subdet() == SDL::Module::Endcap
            and innerSgOuterMDLowerHitModule.subdet() == SDL::Module::Barrel
            and outerSgOuterMDLowerHitModule.subdet() == SDL::Module::Endcap
            )
    {
        runTrackletDefaultAlgoBarrelBarrelEndcapEndcap(logLevel);
    }
}

void SDL::Tracklet::runTrackletDefaultAlgoBarrelBarrelBarrelBarrel(SDL::LogLevel logLevel)
{
    const float deltaZLum = 15.f;
    const float pixelPSZpitch = 0.15;
    const float strip2SZpitch = 5.0;
    const float zGeom =
        ((innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->getModule()).moduleType() == SDL::Module::PS ? pixelPSZpitch : strip2SZpitch)
        +
        ((outerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->getModule()).moduleType() == SDL::Module::PS ? pixelPSZpitch : strip2SZpitch);
    const float rtIn = innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->rt();
    const float rtOut = outerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->rt();
    const float zIn = innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->z();
    const float zOut = outerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->z();
    const float rtOut_o_rtIn = rtOut / rtIn;
    const float kRinv1GeVf = (2.99792458e-3 * 3.8);
    const float k2Rinv1GeVf = kRinv1GeVf / 2.;
    const float ptCut = PTCUT;
    const float sinAlphaMax = 0.95;
    const float sdlSlope = std::asin(std::min(rtOut * k2Rinv1GeVf / ptCut, sinAlphaMax));
    const float dzDrtScale = tan(sdlSlope) / sdlSlope;//FIXME: need approximate value
    const float zLo = zIn + (zIn - deltaZLum) * (rtOut_o_rtIn - 1.f) * (zIn > 0.f ? 1.f : dzDrtScale) - zGeom; //slope-correction only on outer end

    // Reset passBitsDefaultAlgo_;
    passBitsDefaultAlgo_ = 0;

    // Cut #1: Z compatibility
    if (not (zOut >= zLo))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #1 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " zLo: " << zLo <<  " zOut: " << zOut <<  std::endl;
            SDL::cout <<  " zIn: " << zIn <<  std::endl;
            SDL::cout <<  " deltaZLum: " << deltaZLum <<  std::endl;
            SDL::cout <<  " rtOut_o_rtIn: " << rtOut_o_rtIn <<  std::endl;
            SDL::cout <<  " dzDrtScale: " << dzDrtScale <<  std::endl;
            SDL::cout <<  " zGeom: " << zGeom <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }

    const float zHi = zIn + (zIn + deltaZLum) * (rtOut_o_rtIn - 1.f) * (zIn < 0.f ? 1.f : dzDrtScale) + zGeom;

    // Cut #2: Z compatibility
    if (not (zOut >= zLo and zOut <= zHi))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #2 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " zLo: " << zLo <<  " zOut: " << zOut <<  " zHi: " << zHi <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }
    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << TrackletSelection::deltaZ);

    const float drOutIn = (rtOut - rtIn);

    const float rtInvIn = 1. / rtIn;
    // const float ptIn = sdIn.p3.Pt(); // For Pixel Seeds
    const float rIn = sqrt(zIn*zIn + rtIn*rtIn);
    const float drtSDIn = innerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr()->rt() - innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->rt();
    const float dzSDIn = innerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr()->z() - innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->z();
    const float dr3SDIn = innerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr()->r3() - innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->r3();

    const float coshEta = dr3SDIn / drtSDIn; //direction estimate
    float dzErr = zGeom * zGeom * 2.f; //both sides contribute to direction uncertainty
    //FIXME (later) more realistic accounting of material effects is needed
    const float sdlThetaMulsF = 0.015f * sqrt(0.1f + 0.2 * (rtOut - rtIn) / 50.f) * sqrt(rIn / rtIn);
    const float sdlMuls = sdlThetaMulsF * 3.f / ptCut * 4.f; //will need a better guess than x4?
    dzErr += sdlMuls * sdlMuls * drOutIn * drOutIn / 3.f * coshEta * coshEta; //sloppy
    dzErr = sqrt(dzErr);
    const float dzMean = dzSDIn / drtSDIn * drOutIn;
    const float zWindow = dzErr / drtSDIn * drOutIn + zGeom; //FIXME for ptCut lower than ~0.8 need to add curv path correction
    const float zLoPointed = zIn + dzMean * (zIn > 0.f ? 1.f : dzDrtScale) - zWindow;
    const float zHiPointed = zIn + dzMean * (zIn < 0.f ? 1.f : dzDrtScale) + zWindow;

    // Cut #3: Pointed Z
    if (not (zOut >= zLoPointed and zOut <= zHiPointed))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #3 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " zLoPointed: " << zLoPointed <<  " zOut: " << zOut <<  " zHiPointed: " << zHiPointed <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }
    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << TrackletSelection::deltaZPointed);

    const float sdlPVoff = 0.1f / rtOut;
    const float sdlCut = sdlSlope + sqrt(sdlMuls * sdlMuls + sdlPVoff*sdlPVoff);
    const MiniDoublet& sdIn_mdOut = (*innerSegmentPtr()->outerMiniDoubletPtr());
    const MiniDoublet& sdOut_mdOut = (*outerSegmentPtr()->outerMiniDoubletPtr());
    const Hit& sdIn_mdOut_hit = (*innerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr());
    const Hit& sdOut_mdOut_hit = (*outerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr());

    // Cut #4: deltaPhiPos can be tighter
    float deltaPhiPos = sdIn_mdOut_hit.deltaPhi(sdOut_mdOut_hit);
    if (not (std::abs(deltaPhiPos) <= sdlCut) )
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #4 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " deltaPhiPos: " << deltaPhiPos <<  " sdlCut: " << sdlCut <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }
    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << TrackletSelection::deltaPhiPos);

    const Hit& sdIn_r3 = (*innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr());
    const Hit& sdOut_r3 = (*outerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr());
    Hit midR3;
    midR3 += sdIn_r3;
    midR3 += sdOut_r3;
    midR3 /= 2;
    const float dPhi = midR3.deltaPhi(sdOut_r3 - sdIn_r3);

    // Cut #5: deltaPhiChange
    if (not (std::abs(dPhi) <= sdlCut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #5 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " dPhi: " << dPhi <<  " sdlCut: " << sdlCut <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }
    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << TrackletSelection::slope);

    const Hit& sdOut_mdRef_hit = (*outerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr());
    const float sdIn_alpha = innerSegmentPtr()->getDeltaPhiChange();
    const float sdOut_alpha = innerSegmentPtr()->getDeltaPhiChange();
    const float sdOut_alphaOut = sdOut_mdOut_hit.deltaPhi(sdOut_mdOut_hit  - sdOut_mdRef_hit);
    const Hit& sdOut_mdOut_r3 = (*outerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr());
    const Hit dr3 = sdOut_mdOut_r3 - sdIn_r3;
    float betaIn  = sdIn_alpha - sdIn_r3.deltaPhi(dr3);
    float betaOut = -sdOut_alphaOut + sdOut_mdOut_r3.deltaPhi(dr3);
    float betaInRHmin = betaIn;
    float betaInRHmax = betaIn;
    float betaOutRHmin = betaOut;
    float betaOutRHmax = betaOut;

    setRecoVars("sdIn_alpha", sdIn_alpha);
    setRecoVars("sdOut_alphaOut", sdOut_alphaOut);
    setRecoVars("rawBetaInCorrection", sdIn_r3.deltaPhi(dr3));
    setRecoVars("rawBetaOutCorrection", sdOut_mdOut_r3.deltaPhi(dr3));
    setRecoVars("rawBetaIn", betaIn);
    setRecoVars("rawBetaOut", betaOut);

    const Hit& sdIn_mdRef_hit = (*innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr());

    const float sdIn_dr = (sdIn_mdOut_hit - sdIn_mdRef_hit).rt();
    const float sdIn_d = sdIn_mdOut_hit.rt() - sdIn_mdRef_hit.rt();

    const float dr = dr3.rt();
    //beta upper cuts: 2-strip difference for direction resolution
    const float corrF = 1.f;
    bool pass_betaIn_cut = false;//pixel seeds were already selected
    const float betaIn_cut = (-sdIn_dr * corrF + dr) * k2Rinv1GeVf / ptCut + (0.02f / sdIn_d);
    pass_betaIn_cut = std::abs(betaInRHmin) < betaIn_cut;

    setBetaIn(betaInRHmin);
    setBetaInCut(betaIn_cut);

    // Cut #6: first beta cut
    if (not (pass_betaIn_cut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #6 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " betaInRHmin: " << betaInRHmin <<  " betaIn_cut: " << betaIn_cut <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }
    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << TrackletSelection::dAlphaIn);

    //now the actual segment linking magic
    float betaAv = 0.5f * (betaIn + betaOut);
    //pt/k2Rinv1GeVf/2. = R
    //R*sin(betaAv) = pt/k2Rinv1GeVf/2*sin(betaAv) = dr/2 => pt = dr*k2Rinv1GeVf/sin(betaAv);
    float pt_beta = dr * k2Rinv1GeVf / sin(betaAv);

    const float pt_betaMax = 7.0f;

    int lIn = 5;
    int lOut = 5;

    int betacormode = 0;

    const float sdOut_dr = (sdOut_mdOut_hit - sdOut_mdRef_hit).rt();
    const float sdOut_d = sdOut_mdOut_hit.rt() - sdOut_mdRef_hit.rt();
    const float diffDr = std::abs(sdIn_dr - sdOut_dr) / std::abs(sdIn_dr + sdOut_dr);
    if (true //do it for all//diffDr > 0.05 //only if segment length is different significantly
            && betaIn * betaOut > 0.f
            && (std::abs(pt_beta) < 4.f * pt_betaMax
                || (lIn >= 11 && std::abs(pt_beta) < 8.f * pt_betaMax)))   //and the pt_beta is well-defined; less strict for endcap-endcap
    {
        betacormode = 1;
        const float betaInUpd  = betaIn + copysign(std::asin(std::min(sdIn_dr * k2Rinv1GeVf / std::abs(pt_beta), sinAlphaMax)), betaIn); //FIXME: need a faster version
        const float betaOutUpd = betaOut + copysign(std::asin(std::min(sdOut_dr * k2Rinv1GeVf / std::abs(pt_beta), sinAlphaMax)), betaOut); //FIXME: need a faster version
        betaAv = 0.5f * (betaInUpd + betaOutUpd);
        pt_beta = dr * k2Rinv1GeVf / sin(betaAv); //get a better pt estimate
        betaIn  += copysign(std::asin(std::min(sdIn_dr * k2Rinv1GeVf / std::abs(pt_beta), sinAlphaMax)), betaIn); //FIXME: need a faster version
        betaOut += copysign(std::asin(std::min(sdOut_dr * k2Rinv1GeVf / std::abs(pt_beta), sinAlphaMax)), betaOut); //FIXME: need a faster version
        //update the av and pt
        betaAv = 0.5f * (betaIn + betaOut);
        pt_beta = dr * k2Rinv1GeVf / sin(betaAv); //get a better pt estimate
    }
    else if (lIn < 11 && std::abs(betaOut) < 0.2 * std::abs(betaIn) && std::abs(pt_beta) < 12.f * pt_betaMax)   //use betaIn sign as ref
    {
        betacormode = 2;
        const float pt_betaIn = dr * k2Rinv1GeVf / sin(betaIn);
        const float betaInUpd  = betaIn + copysign(std::asin(std::min(sdIn_dr * k2Rinv1GeVf / std::abs(pt_betaIn), sinAlphaMax)), betaIn); //FIXME: need a faster version
        const float betaOutUpd = betaOut + copysign(std::asin(std::min(sdOut_dr * k2Rinv1GeVf / std::abs(pt_betaIn), sinAlphaMax)), betaIn); //FIXME: need a faster version
        betaAv = (std::abs(betaOut) > 0.2f * std::abs(betaIn)) ? (0.5f * (betaInUpd + betaOutUpd)) : betaInUpd;
        pt_beta = dr * k2Rinv1GeVf / sin(betaAv); //get a better pt estimate
        betaIn  += copysign(std::asin(std::min(sdIn_dr * k2Rinv1GeVf / std::abs(pt_beta), sinAlphaMax)), betaIn); //FIXME: need a faster version
        betaOut += copysign(std::asin(std::min(sdOut_dr * k2Rinv1GeVf / std::abs(pt_beta), sinAlphaMax)), betaIn); //FIXME: need a faster version
        //update the av and pt
        betaAv = 0.5f * (betaIn + betaOut);
        pt_beta = dr * k2Rinv1GeVf / sin(betaAv); //get a better pt estimate
    }

    //rescale the ranges proportionally
    const float betaInMMSF = (std::abs(betaInRHmin + betaInRHmax) > 0) ? (2.f * betaIn / std::abs(betaInRHmin + betaInRHmax)) : 0.; //mean value of min,max is the old betaIn
    const float betaOutMMSF = (std::abs(betaOutRHmin + betaOutRHmax) > 0) ? (2.f * betaOut / std::abs(betaOutRHmin + betaOutRHmax)) : 0.;
    betaInRHmin *= betaInMMSF;
    betaInRHmax *= betaInMMSF;
    betaOutRHmin *= betaOutMMSF;
    betaOutRHmax *= betaOutMMSF;

    const float dBetaMuls = sdlThetaMulsF * 4.f / std::min(std::abs(pt_beta), pt_betaMax); //need to confirm the range-out value of 7 GeV

    //regularize to alpha of pt_betaMax .. review may want to add resolution
    const float sdIn_rt = sdIn_mdRef_hit.rt();
    const float sdOut_rt = sdOut_mdRef_hit.rt();
    const float sdIn_z = sdIn_mdRef_hit.z();
    const float sdOut_z = sdOut_mdRef_hit.z();
    const float alphaInAbsReg = std::max(std::abs(sdIn_alpha), std::asin(std::min(sdIn_rt * k2Rinv1GeVf / 3.0f, sinAlphaMax)));
    const float alphaOutAbsReg = std::max(std::abs(sdOut_alpha), std::asin(std::min(sdOut_rt * k2Rinv1GeVf / 3.0f, sinAlphaMax)));
    const float dBetaInLum = lIn < 11 ? 0.0f : std::abs(alphaInAbsReg*deltaZLum / sdIn_z);
    const float dBetaOutLum = lOut < 11 ? 0.0f : std::abs(alphaOutAbsReg*deltaZLum / sdOut_z);
    const float dBetaLum2 = (dBetaInLum + dBetaOutLum) * (dBetaInLum + dBetaOutLum);

    const float sinDPhi = std::sin(dPhi);
    // const float dBetaRIn2 = std::pow((sdIn.mdRef.rtRHout - sdIn.mdRef.rtRHin) * sinDPhi / dr, 2); //TODO-RH: Ask Slava about this rtRHout? rtRHin?
    // const float dBetaROut2 = std::pow((sdOut.mdOut.rtRHout - sdOut.mdOut.rtRHin) * sinDPhi / dr, 2); //TODO-RH
    const float dBetaRIn2 = 0; // TODO-RH
    const float dBetaROut2 = 0; // TODO-RH

    const float betaOut_cut = std::asin(std::min(dr*k2Rinv1GeVf / ptCut, sinAlphaMax)) //FIXME: need faster version
        + (0.02f / sdOut_d) + sqrt(dBetaLum2 + dBetaMuls*dBetaMuls);
    // const float betaOut_cut = std::min(0.01, std::asin(std::min(dr*k2Rinv1GeVf / ptCut, sinAlphaMax)) + (0.02f / sdOut_d) + sqrt(dBetaLum2 + dBetaMuls*dBetaMuls));

    setBetaOut(betaOut);
    setBetaOutCut(betaOut_cut);

    // Cut #7: The real beta cut
    if (not (std::abs(betaOut) < betaOut_cut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #7 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " betaOut: " << betaOut <<  " betaOut_cut: " << betaOut_cut <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }
    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << TrackletSelection::dAlphaOut);

    float pt_betaIn = dr * k2Rinv1GeVf / sin(betaIn);
    const float pt_betaOut = dr * k2Rinv1GeVf / sin(betaOut);
    const float dBetaRes = 0.02f / std::min(sdOut_d, sdIn_d);
    const float dBetaCut2 = (dBetaRes*dBetaRes * 2.0f + dBetaMuls * dBetaMuls + dBetaLum2 + dBetaRIn2 + dBetaROut2
            + 0.25 * std::pow(std::abs(betaInRHmin - betaInRHmax) + std::abs(betaOutRHmin - betaOutRHmax), 2));
    const float dBeta = betaIn - betaOut;
    // const float dZeta = sdIn.zeta - sdOut.zeta;

    const float innerSgInnerMdDetId = (innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->getModule()).detId();
    const float outerSgInnerMdDetId = (outerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->getModule()).detId();
    const float innerSgOuterMdDetId = (innerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr()->getModule()).detId();
    const float outerSgOuterMdDetId = (outerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr()->getModule()).detId();

    setRecoVars("sinAlphaMax", sinAlphaMax);
    setRecoVars("betaIn", betaIn);
    setRecoVars("betaInRHmax", betaInRHmax);
    setRecoVars("betaInRHmin", betaInRHmin);
    setRecoVars("betaOut", betaOut);
    setRecoVars("betaOutRHmax", betaOutRHmax);
    setRecoVars("betaOutRHmin", betaOutRHmin);
    setRecoVars("dBeta", dBeta);
    setRecoVars("dBetaCut2", dBetaCut2);
    setRecoVars("dBetaLum2", dBetaLum2);
    setRecoVars("dBetaMuls", dBetaMuls);
    setRecoVars("dBetaRIn2", dBetaRIn2);
    setRecoVars("dBetaROut2", dBetaROut2);
    setRecoVars("dBetaRes", dBetaRes);
    setRecoVars("deltaZLum", deltaZLum);
    setRecoVars("dr", dr);
    setRecoVars("dzDrtScale", dzDrtScale);
    setRecoVars("innerSgInnerMdDetId", innerSgInnerMdDetId);
    setRecoVars("innerSgOuterMdDetId", innerSgOuterMdDetId);
    setRecoVars("k2Rinv1GeVf", k2Rinv1GeVf);
    setRecoVars("kRinv1GeVf", kRinv1GeVf);
    setRecoVars("outerSgInnerMdDetId", outerSgInnerMdDetId);
    setRecoVars("outerSgOuterMdDetId", outerSgOuterMdDetId);
    setRecoVars("pixelPSZpitch", pixelPSZpitch);
    setRecoVars("ptCut", ptCut);
    setRecoVars("pt_betaIn", pt_betaIn);
    setRecoVars("pt_betaOut", pt_betaOut);
    setRecoVars("rtIn", rtIn);
    setRecoVars("rtOut", rtOut);
    setRecoVars("rtOut_o_rtIn", rtOut_o_rtIn);
    setRecoVars("sdIn_d", sdIn_d);
    setRecoVars("sdOut_d", sdOut_d);
    setRecoVars("sdlSlope", sdlSlope);
    setRecoVars("strip2SZpitch", strip2SZpitch);
    setRecoVars("zGeom", zGeom);
    setRecoVars("zIn", zIn);
    setRecoVars("zLo", zLo);
    setRecoVars("zOut", zOut);
    setRecoVars("betacormode", betacormode);

    setDeltaBeta(dBeta);
    setDeltaBetaCut(std::sqrt(dBetaCut2));

    if (not (dBeta * dBeta <= dBetaCut2))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #8 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " dBeta*dBeta: " << dBeta*dBeta <<  " dBetaCut2: " << dBetaCut2 <<  std::endl;
            SDL::cout <<  " dBetaRes: " << dBetaRes <<  " dBetaMuls: " << dBetaMuls <<  " dBetaLum2: " << dBetaLum2 <<  std::endl;
            SDL::cout <<  " dBetaRIn2: " << dBetaRIn2 <<  " dBetaROut2: " << dBetaROut2 <<  std::endl;
            SDL::cout <<  " betaInRHmin: " << betaInRHmin <<  " betaInRHmax: " << betaInRHmax <<  std::endl;
            SDL::cout <<  " betaOutRHmin: " << betaOutRHmin <<  " betaOutRHmax: " << betaOutRHmax <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        // passAlgo_ |= (1 << SDL::Default_TLAlgo);
        return;
    }
    else if (logLevel >= SDL::Log_Debug3)
    {
        SDL::cout << "Passed Cut #8 in " << __FUNCTION__ << std::endl;
        SDL::cout <<  " dBeta*dBeta: " << dBeta*dBeta <<  " dBetaCut2: " << dBetaCut2 <<  std::endl;
        SDL::cout <<  " dBetaRes: " << dBetaRes <<  " dBetaMuls: " << dBetaMuls <<  " dBetaLum2: " << dBetaLum2 <<  std::endl;
        SDL::cout <<  " dBetaRIn2: " << dBetaRIn2 <<  " dBetaROut2: " << dBetaROut2 <<  std::endl;
        SDL::cout <<  " betaInRHmin: " << betaInRHmin <<  " betaInRHmax: " << betaInRHmax <<  std::endl;
        SDL::cout <<  " betaOutRHmin: " << betaOutRHmin <<  " betaOutRHmax: " << betaOutRHmax <<  std::endl;
    }
    // Flag the pass bit
    passBitsDefaultAlgo_ |= (1 << TrackletSelection::dBeta);

    passAlgo_ |= (1 << SDL::Default_TLAlgo);
    return;
}

void SDL::Tracklet::runTrackletDefaultAlgoBarrelBarrelEndcapEndcap(SDL::LogLevel logLevel)
{

    const float deltaZLum = 15.f;
    const float pixelPSZpitch = 0.15;
    const float strip2SZpitch = 5.0;
    const float zGeom =
        ((innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->getModule()).moduleType() == SDL::Module::PS ? pixelPSZpitch : strip2SZpitch)
        +
        ((outerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->getModule()).moduleType() == SDL::Module::PS ? pixelPSZpitch : strip2SZpitch);
    const float rtIn = innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->rt();
    const float rtOut = outerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->rt();
    const float zIn = innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->z();
    const float zOut = outerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->z();
    const float rtOut_o_rtIn = rtOut / rtIn;
    const float kRinv1GeVf = (2.99792458e-3 * 3.8);
    const float k2Rinv1GeVf = kRinv1GeVf / 2.;
    const float ptCut = PTCUT;
    const float sinAlphaMax = 0.95;
    const float sdlSlope = std::asin(std::min(rtOut * k2Rinv1GeVf / ptCut, sinAlphaMax));
    const float dzDrtScale = tan(sdlSlope) / sdlSlope;//FIXME: need approximate value
    const float zLo = zIn + (zIn - deltaZLum) * (rtOut_o_rtIn - 1.f) * (zIn > 0.f ? 1.f : dzDrtScale) - zGeom; //slope-correction only on outer end

    // Cut #0: Preliminary
    // (Only here in endcap case)
    if (not (zIn * zOut > 0))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #0 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " zIn: " << zIn <<  std::endl;
            SDL::cout <<  " zOut: " << zOut <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }


    const float dLum = std::copysign(deltaZLum, zIn);
    const bool isOutSgInnerMDPS = (outerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->getModule()).moduleType() == SDL::Module::PS;
    const float rtGeom1 = isOutSgInnerMDPS ? pixelPSZpitch : strip2SZpitch;//FIXME: make this chosen by configuration for lay11,12 full PS
    const float zGeom1 = std::copysign(zGeom, zIn); //used in B-E region
    const float rtLo = rtIn * (1.f + (zOut - zIn - zGeom1) / (zIn + zGeom1 + dLum) / dzDrtScale) - rtGeom1; //slope correction only on the lower end

    // Cut #1: rt condition
    if (not (rtOut >= rtLo))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #1 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " rtOut: " << rtOut <<  std::endl;
            SDL::cout <<  " rtLo: " << rtLo <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }

    float zInForHi = zIn - zGeom1 - dLum;
    if (zInForHi * zIn < 0)
        zInForHi = std::copysign(0.1f, zIn);
    const float rtHi = rtIn * (1.f + (zOut - zIn + zGeom1) / zInForHi) + rtGeom1;

    // Cut #2: rt condition
    if (not (rtOut >= rtLo and rtOut <= rtHi))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #2 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " rtOut: " << rtOut <<  std::endl;
            SDL::cout <<  " rtLo: " << rtLo <<  std::endl;
            SDL::cout <<  " rtHi: " << rtHi <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }

    const float rIn = sqrt(zIn*zIn + rtIn*rtIn);
    const float drtSDIn = innerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr()->rt() - innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->rt();
    const float dzSDIn = innerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr()->z() - innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->z();
    const float dr3SDIn = innerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr()->r3() - innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr()->r3();
    const float coshEta = dr3SDIn / drtSDIn; //direction estimate
    const float dzOutInAbs = std::abs(zOut - zIn);
    const float multDzDr = dzOutInAbs * coshEta / (coshEta * coshEta - 1.f);
    const float zGeom1_another = pixelPSZpitch; // TODO-Q Why only one?
    const float kZ = (zOut - zIn) / dzSDIn;
    float drtErr = zGeom1_another * zGeom1_another * drtSDIn * drtSDIn / dzSDIn / dzSDIn * (1.f - 2.f * kZ + 2.f * kZ * kZ); //Notes:122316
    const float sdlThetaMulsF = 0.015f * sqrt(0.1f + 0.2 * (rtOut - rtIn) / 50.f) * sqrt(rIn / rtIn);
    const float sdlMuls = sdlThetaMulsF * 3.f / ptCut * 4.f; //will need a better guess than x4?
    drtErr += sdlMuls * sdlMuls * multDzDr * multDzDr / 3.f * coshEta * coshEta; //sloppy: relative muls is 1/3 of total muls
    drtErr = sqrt(drtErr);
    const float drtMean = drtSDIn * dzOutInAbs / std::abs(dzSDIn); //
    const float rtWindow = drtErr + rtGeom1;
    const float rtLo_another = rtIn + drtMean / dzDrtScale - rtWindow;
    const float rtHi_another = rtIn + drtMean + rtWindow;

    // Cut #3: rt-z pointed
    if (not (kZ >= 0 and rtOut >= rtLo and rtOut <= rtHi))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #3 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " kZ: " << kZ <<  std::endl;
            SDL::cout <<  " rtOut: " << rtOut <<  std::endl;
            SDL::cout <<  " rtLo: " << rtLo <<  std::endl;
            SDL::cout <<  " rtHi: " << rtHi <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }

    const float sdlPVoff = 0.1f / rtOut;
    const float sdlCut = sdlSlope + sqrt(sdlMuls * sdlMuls + sdlPVoff*sdlPVoff);
    const MiniDoublet& sdIn_mdOut = (*innerSegmentPtr()->outerMiniDoubletPtr());
    const MiniDoublet& sdOut_mdOut = (*outerSegmentPtr()->outerMiniDoubletPtr());
    const Hit& sdIn_mdOut_hit = (*innerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr());
    const Hit& sdOut_mdOut_hit = (*outerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr());

    // Cut #4: deltaPhiPos can be tighter
    float deltaPhiPos = sdIn_mdOut_hit.deltaPhi(sdOut_mdOut_hit);
    if (not (std::abs(deltaPhiPos) <= sdlCut) )
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #4 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " deltaPhiPos: " << deltaPhiPos <<  " sdlCut: " << sdlCut <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }

    const Hit& sdIn_r3 = (*innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr());
    const Hit& sdOut_r3 = (*outerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr());
    Hit midR3;
    midR3 += sdIn_r3;
    midR3 += sdOut_r3;
    midR3 /= 2;
    const float dPhi = midR3.deltaPhi(sdOut_r3 - sdIn_r3);

    // Cut #5: deltaPhiChange
    if (not (std::abs(dPhi) <= sdlCut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #5 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " dPhi: " << dPhi <<  " sdlCut: " << sdlCut <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }

    const Hit& sdOut_mdRef_hit = (*outerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr());
    const float sdIn_alpha = innerSegmentPtr()->getDeltaPhiChange();
    const float sdOut_alpha = innerSegmentPtr()->getDeltaPhiChange();
    const float sdOut_alphaOut = sdOut_mdOut_hit.deltaPhi(sdOut_mdOut_hit  - sdOut_mdRef_hit);
    const Hit& sdOut_mdOut_r3 = (*outerSegmentPtr()->outerMiniDoubletPtr()->anchorHitPtr());
    const Hit dr3 = sdOut_mdOut_r3 - sdIn_r3;
    float betaIn  = sdIn_alpha - sdIn_r3.deltaPhi(dr3);
    float betaOut = -sdOut_alphaOut + sdOut_mdOut_r3.deltaPhi(dr3);
    float betaInRHmin = betaIn;
    float betaInRHmax = betaIn;
    float betaOutRHmin = betaOut;// - sdOut.alphaOutRHmin + sdOut.alphaOut; // TODO-RH: the alphaOutRHmin/ max are the sliding
    float betaOutRHmax = betaOut;// - sdOut.alphaOutRHmax + sdOut.alphaOut; // TODO-RH: the alphaOutRHmin/ max are the sliding
    if (std::abs(betaOutRHmin) > std::abs(betaOutRHmax)) std::swap(betaOutRHmax, betaOutRHmin);

    const Hit& sdIn_mdRef_hit = (*innerSegmentPtr()->innerMiniDoubletPtr()->anchorHitPtr());

    const float sdIn_dr = (sdIn_mdOut_hit - sdIn_mdRef_hit).rt();
    const float sdIn_d = sdIn_mdOut_hit.rt() - sdIn_mdRef_hit.rt();

    const float dr = dr3.rt();
    //beta upper cuts: 2-strip difference for direction resolution
    const float corrF = 1.f;
    bool pass_betaIn_cut = false;//pixel seeds were already selected
    const float betaIn_cut = (-sdIn_dr * corrF + dr) * k2Rinv1GeVf / ptCut + (0.02f / sdIn_d);
    pass_betaIn_cut = std::abs(betaInRHmin) < betaIn_cut;

    // Cut #6: first beta cut
    if (not (pass_betaIn_cut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #6 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " betaInRHmin: " << betaInRHmin <<  " betaIn_cut: " << betaIn_cut <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }

    //now the actual segment linking magic
    float betaAv = 0.5f * (betaIn + betaOut);
    //pt/k2Rinv1GeVf/2. = R
    //R*sin(betaAv) = pt/k2Rinv1GeVf/2*sin(betaAv) = dr/2 => pt = dr*k2Rinv1GeVf/sin(betaAv);
    float pt_beta = dr * k2Rinv1GeVf / sin(betaAv);

    const float pt_betaMax = 7.0f;

    int lIn = 5;
    int lOut = 11;

    const float sdOut_dr = (sdOut_mdOut_hit - sdOut_mdRef_hit).rt();
    const float sdOut_d = sdOut_mdOut_hit.rt() - sdOut_mdRef_hit.rt();
    const float diffDr = std::abs(sdIn_dr - sdOut_dr) / std::abs(sdIn_dr + sdOut_dr);
    if (true //do it for all//diffDr > 0.05 //only if segment length is different significantly
            && betaIn * betaOut > 0.f
            && (std::abs(pt_beta) < 4.f * pt_betaMax
                || (lIn >= 11 && std::abs(pt_beta) < 8.f * pt_betaMax)))   //and the pt_beta is well-defined; less strict for endcap-endcap
    {
        const float betaInUpd  = betaIn + copysign(std::asin(std::min(sdIn_dr * k2Rinv1GeVf / std::abs(pt_beta), sinAlphaMax)), betaIn); //FIXME: need a faster version
        const float betaOutUpd = betaOut + copysign(std::asin(std::min(sdOut_dr * k2Rinv1GeVf / std::abs(pt_beta), sinAlphaMax)), betaOut); //FIXME: need a faster version
        betaAv = 0.5f * (betaInUpd + betaOutUpd);
        pt_beta = dr * k2Rinv1GeVf / sin(betaAv); //get a better pt estimate
        betaIn  += copysign(std::asin(std::min(sdIn_dr * k2Rinv1GeVf / std::abs(pt_beta), sinAlphaMax)), betaIn); //FIXME: need a faster version
        betaOut += copysign(std::asin(std::min(sdOut_dr * k2Rinv1GeVf / std::abs(pt_beta), sinAlphaMax)), betaOut); //FIXME: need a faster version
        //update the av and pt
        betaAv = 0.5f * (betaIn + betaOut);
        pt_beta = dr * k2Rinv1GeVf / sin(betaAv); //get a better pt estimate
    }
    else if (lIn < 11 && std::abs(betaOut) < 0.2 * std::abs(betaIn) && std::abs(pt_beta) < 12.f * pt_betaMax)   //use betaIn sign as ref
    {
        const float pt_betaIn = dr * k2Rinv1GeVf / sin(betaIn);
        const float betaInUpd  = betaIn + copysign(std::asin(std::min(sdIn_dr * k2Rinv1GeVf / std::abs(pt_betaIn), sinAlphaMax)), betaIn); //FIXME: need a faster version
        const float betaOutUpd = betaOut + copysign(std::asin(std::min(sdOut_dr * k2Rinv1GeVf / std::abs(pt_betaIn), sinAlphaMax)), betaIn); //FIXME: need a faster version
        betaAv = (std::abs(betaOut) > 0.2f * std::abs(betaIn)) ? (0.5f * (betaInUpd + betaOutUpd)) : betaInUpd;
        pt_beta = dr * k2Rinv1GeVf / sin(betaAv); //get a better pt estimate
        betaIn  += copysign(std::asin(std::min(sdIn_dr * k2Rinv1GeVf / std::abs(pt_beta), sinAlphaMax)), betaIn); //FIXME: need a faster version
        betaOut += copysign(std::asin(std::min(sdOut_dr * k2Rinv1GeVf / std::abs(pt_beta), sinAlphaMax)), betaIn); //FIXME: need a faster version
        //update the av and pt
        betaAv = 0.5f * (betaIn + betaOut);
        pt_beta = dr * k2Rinv1GeVf / sin(betaAv); //get a better pt estimate
    }

    //rescale the ranges proportionally
    const float betaInMMSF = (std::abs(betaInRHmin + betaInRHmax) > 0) ? 2.f * betaIn / std::abs(betaInRHmin + betaInRHmax) : 0.; //TODO-RH: the terneary operator should no be necessary once RHmin/max is propagatedmean value of min,max is the old betaIn
    const float betaOutMMSF = (std::abs(betaOutRHmin + betaOutRHmax) > 0) ? 2.f * betaOut / std::abs(betaOutRHmin + betaOutRHmax) : 0.;
    betaInRHmin *= betaInMMSF;
    betaInRHmax *= betaInMMSF;
    betaOutRHmin *= betaOutMMSF;
    betaOutRHmax *= betaOutMMSF;

    const float dBetaMuls = sdlThetaMulsF * 4.f / std::min(std::abs(pt_beta), pt_betaMax); //need to confirm the range-out value of 7 GeV

    //regularize to alpha of pt_betaMax .. review may want to add resolution
    const float sdIn_rt = sdIn_mdRef_hit.rt();
    const float sdOut_rt = sdOut_mdRef_hit.rt();
    const float sdIn_z = sdIn_mdRef_hit.z();
    const float sdOut_z = sdOut_mdRef_hit.z();
    const float alphaInAbsReg = std::max(std::abs(sdIn_alpha), std::asin(std::min(sdIn_rt * k2Rinv1GeVf / 3.0f, sinAlphaMax)));
    const float alphaOutAbsReg = std::max(std::abs(sdOut_alpha), std::asin(std::min(sdOut_rt * k2Rinv1GeVf / 3.0f, sinAlphaMax)));
    const float dBetaInLum = lIn < 11 ? 0.0f : std::abs(alphaInAbsReg*deltaZLum / sdIn_z);
    const float dBetaOutLum = lOut < 11 ? 0.0f : std::abs(alphaOutAbsReg*deltaZLum / sdOut_z);
    const float dBetaLum2 = (dBetaInLum + dBetaOutLum) * (dBetaInLum + dBetaOutLum);

    const float sinDPhi = std::sin(dPhi);
    // const float dBetaRIn2 = std::pow((sdIn.mdRef.rtRHout - sdIn.mdRef.rtRHin) * sinDPhi / dr, 2); //TODO-RH: Ask Slava about this rtRHout? rtRHin?
    // const float dBetaROut2 = std::pow((sdOut.mdOut.rtRHout - sdOut.mdOut.rtRHin) * sinDPhi / dr, 2); //TODO-RH: Ask Slava about this rtRHout? rtRHin?
    // const float dBetaRIn2 = std::pow((sdIn_rt * 0.01) * sinDPhi / dr, 2); //TODO-RH-MOCKUP: Sliding induces 1% error
    // const float dBetaROut2 = std::pow((sdOut_rt * 0.4) * sinDPhi / dr, 2); //TODO-RH-MOCKUP: Sliding induces 1% error
    const float dBetaRIn2 = 0; // TODO-RH
    const float dBetaROut2 = 0; // TODO-RH

    const float betaOut_cut = std::asin(std::min(dr*k2Rinv1GeVf / ptCut, sinAlphaMax)) //FIXME: need faster version
        + (0.02f / sdOut_d) + sqrt(dBetaLum2 + dBetaMuls*dBetaMuls);

    // Cut #7: The real beta cut
    if (not (std::abs(betaOut) < betaOut_cut))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #7 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " betaOut: " << betaOut <<  " betaOut_cut: " << betaOut_cut <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }

    float pt_betaIn = dr * k2Rinv1GeVf / sin(betaIn);
    const float pt_betaOut = dr * k2Rinv1GeVf / sin(betaOut);
    const float dBetaRes = 0.02f / std::min(sdOut_d, sdIn_d);
    const float dBetaCut2 = (dBetaRes*dBetaRes * 2.0f + dBetaMuls * dBetaMuls + dBetaLum2 + dBetaRIn2 + dBetaROut2
            + 0.25 * std::pow(std::abs(betaInRHmin - betaInRHmax) + std::abs(betaOutRHmin - betaOutRHmax), 2));
    const float dBeta = betaIn - betaOut;
    // const float dZeta = sdIn.zeta - sdOut.zeta;

    setDeltaBeta(dBeta);

    if (not (dBeta * dBeta <= dBetaCut2))
    {
        if (logLevel >= SDL::Log_Debug3)
        {
            SDL::cout << "Failed Cut #8 in " << __FUNCTION__ << std::endl;
            SDL::cout <<  " dBeta*dBeta: " << dBeta*dBeta <<  " dBetaCut2: " << dBetaCut2 <<  std::endl;
            SDL::cout <<  " dBetaRes: " << dBetaRes <<  " dBetaMuls: " << dBetaMuls <<  " dBetaLum2: " << dBetaLum2 <<  std::endl;
            SDL::cout <<  " dBetaRIn2: " << dBetaRIn2 <<  " dBetaROut2: " << dBetaROut2 <<  std::endl;
            SDL::cout <<  " betaInRHmin: " << betaInRHmin <<  " betaInRHmax: " << betaInRHmax <<  std::endl;
            SDL::cout <<  " betaOutRHmin: " << betaOutRHmin <<  " betaOutRHmax: " << betaOutRHmax <<  std::endl;
        }
        passAlgo_ &= (0 << SDL::Default_TLAlgo);
        return;
    }
    else if (logLevel >= SDL::Log_Debug3)
    {
        SDL::cout << "Passed Cut #8 in " << __FUNCTION__ << std::endl;
        SDL::cout <<  " dBeta*dBeta: " << dBeta*dBeta <<  " dBetaCut2: " << dBetaCut2 <<  std::endl;
        SDL::cout <<  " dBetaRes: " << dBetaRes <<  " dBetaMuls: " << dBetaMuls <<  " dBetaLum2: " << dBetaLum2 <<  std::endl;
        SDL::cout <<  " dBetaRIn2: " << dBetaRIn2 <<  " dBetaROut2: " << dBetaROut2 <<  std::endl;
        SDL::cout <<  " betaInRHmin: " << betaInRHmin <<  " betaInRHmax: " << betaInRHmax <<  std::endl;
        SDL::cout <<  " betaOutRHmin: " << betaOutRHmin <<  " betaOutRHmax: " << betaOutRHmax <<  std::endl;
    }


    passAlgo_ |= (1 << SDL::Default_TLAlgo);
    return;
}

void SDL::Tracklet::runTrackletDefaultAlgoBarrelBarrel(SDL::LogLevel logLevel)
{
    return;
}

void SDL::Tracklet::runTrackletDefaultAlgoBarrelEndcap(SDL::LogLevel logLevel)
{
    return;
}

void SDL::Tracklet::runTrackletDefaultAlgoEndcapEndcap(SDL::LogLevel logLevel)
{
    return;
}

bool SDL::Tracklet::hasCommonSegment(const Tracklet& outer_tl) const
{
    if (outerSegmentPtr()->isIdxMatched(*(outer_tl.innerSegmentPtr())))
        return true;
    return false;
}

bool SDL::Tracklet::isIdxMatched(const Tracklet& md) const
{
    if (not innerSegmentPtr_->isIdxMatched(*(md.innerSegmentPtr())))
        return false;
    if (not outerSegmentPtr_->isIdxMatched(*(md.outerSegmentPtr())))
        return false;
    return true;
}

bool SDL::Tracklet::isAnchorHitIdxMatched(const Tracklet& md) const
{
    if (not innerSegmentPtr_->isAnchorHitIdxMatched(*(md.innerSegmentPtr())))
        return false;
    if (not outerSegmentPtr_->isAnchorHitIdxMatched(*(md.outerSegmentPtr())))
        return false;
    return true;
}

bool SDL::Tracklet::isSegmentPairATracklet(const Segment& innerSegment, const Segment& outerSegment, TLAlgo algo, SDL::LogLevel logLevel)
{
    // If the algorithm is "do all combination" (e.g. used for efficiency calculation)
    if (algo == SDL::AllComb_TLAlgo)
    {
        return true;
    }
    else if (algo == SDL::Default_TLAlgo)
    {
        return false;
    }
    else
    {
        SDL::cout << "Warning: Unrecognized segment algorithm!" << algo << std::endl;
        return false;
    }
}

bool SDL::Tracklet::isSegmentPairATrackletBarrel(const Segment& innerSegment, const Segment& outerSegment, TLAlgo algo, SDL::LogLevel logLevel)
{
    return false;
}

bool SDL::Tracklet::isSegmentPairATrackletEndcap(const Segment& innerSegment, const Segment& outerSegment, TLAlgo algo, SDL::LogLevel logLevel)
{
    return false;
}

namespace SDL
{
    std::ostream& operator<<(std::ostream& out, const Tracklet& tl)
    {
        out << "Tracklet" << std::endl;
        out << "------------------------------" << std::endl;
        {
            IndentingOStreambuf indent(out);
            out << "Inner Segment" << std::endl;
            out << "------------------------------" << std::endl;
            {
                IndentingOStreambuf indent(out);
                out << tl.innerSegmentPtr_ << std::endl;
            }
            out << "Outer Segment" << std::endl;
            out << "------------------------------" << std::endl;
            {
                IndentingOStreambuf indent(out);
                out << tl.outerSegmentPtr_;
            }
        }
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const Tracklet* tl)
    {
        out << *tl;
        return out;
    }
}

