#include "StudySDLEfficiency.h"

StudySDLEfficiency::StudySDLEfficiency(
        const char* studyName,
        StudySDLEfficiency::StudySDLMiniDoubletEfficiencyMode md_mode_,
        StudySDLEfficiency::StudySDLSegmentEfficiencyMode sg_mode_,
        StudySDLEfficiency::StudySDLTrackletEfficiencyMode tl_mode_,
        StudySDLEfficiency::StudySDLTrackCandidateEfficiencyMode tc_mode_,
        std::vector<float> ptbounds)
{

    studyname = studyName;

    md_mode = md_mode_;
    sg_mode = sg_mode_;
    tl_mode = tl_mode_;
    tc_mode = tc_mode_;

    pt_boundaries = ptbounds;

    switch (md_mode)
    {
        case kStudySDLMDEffAll: md_modename = "all"; break;
        case kStudySDLMDEffBarrel: md_modename = "barrel"; break;
        case kStudySDLMDEffBarrelFlat: md_modename = "barrelflat"; break;
        case kStudySDLMDEffBarrelTilt: md_modename = "barreltilt"; break;
        case kStudySDLMDEffEndcap: md_modename = "endcap"; break;
        case kStudySDLMDEffEndcapPS: md_modename = "endcapPS"; break;
        case kStudySDLMDEffEndcap2S: md_modename = "endcap2S"; break;
        default: md_modename = "UNDEFINED"; break;
    }

    switch (sg_mode)
    {
        case kStudySDLSGEffAll: sg_modename = "all"; break;
        case kStudySDLSGEffBB: sg_modename = "bb"; break;
        default: sg_modename = "UNDEFINED"; break;
    }

    switch (tl_mode)
    {
        case kStudySDLTLEffAll: tl_modename = "all"; break;
        case kStudySDLTLEffBBBB: tl_modename = "bbbb"; break;
        default: tl_modename = "UNDEFINED"; break;
    }

    switch (tc_mode)
    {
        case kStudySDLTCEffAll: tc_modename = "all"; break;
        case kStudySDLTCEffBBBBBB: tc_modename = "bbbbbb"; break;
        default: tc_modename = "UNDEFINED"; break;
    }

}

void StudySDLEfficiency::bookStudy()
{
    // Book Histograms
    const int nlayers = NLAYERS;

    for (int ii = 0; ii < nlayers; ++ii)
    {
        ana.histograms.addVecHistogram(TString::Format("md_%s_matched_track_pt_by_layer%d", md_modename, ii), pt_boundaries, [&, ii]() { return md_matched_track_pt_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("md_%s_all_track_pt_by_layer%d", md_modename, ii), pt_boundaries, [&, ii]() { return md_all_track_pt_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("md_%s_matched_track_eta_by_layer%d", md_modename, ii), 50, -4, 4, [&, ii]() { return md_matched_track_eta_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("md_%s_all_track_eta_by_layer%d", md_modename, ii), 50, -4, 4, [&, ii]() { return md_all_track_eta_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("md_%s_matched_track_dxy_by_layer%d", md_modename, ii), 50, -10, 10, [&, ii]() { return md_matched_track_dxy_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("md_%s_all_track_dxy_by_layer%d", md_modename, ii), 50, -10, 10, [&, ii]() { return md_all_track_dxy_by_layer[ii]; } );
    }

    for (int ii = 0; ii < nlayers; ++ii)
    {
        ana.histograms.addVecHistogram(TString::Format("sg_%s_matched_track_pt_by_layer%d", sg_modename, ii), pt_boundaries, [&, ii]() { return sg_matched_track_pt_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("sg_%s_all_track_pt_by_layer%d", sg_modename, ii), pt_boundaries, [&, ii]() { return sg_all_track_pt_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("sg_%s_matched_track_eta_by_layer%d", sg_modename, ii), 50, -4, 4, [&, ii]() { return sg_matched_track_eta_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("sg_%s_all_track_eta_by_layer%d", sg_modename, ii), 50, -4, 4, [&, ii]() { return sg_all_track_eta_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("sg_%s_matched_track_dxy_by_layer%d", sg_modename, ii), 50, -10, 10, [&, ii]() { return sg_matched_track_dxy_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("sg_%s_all_track_dxy_by_layer%d", sg_modename, ii), 50, -10, 10, [&, ii]() { return sg_all_track_dxy_by_layer[ii]; } );
    }

    for (int ii = 0; ii < nlayers; ++ii)
    {
        ana.histograms.addVecHistogram(TString::Format("tl_%s_matched_track_pt_by_layer%d", tl_modename, ii), pt_boundaries, [&, ii]() { return tl_matched_track_pt_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("tl_%s_all_track_pt_by_layer%d", tl_modename, ii), pt_boundaries, [&, ii]() { return tl_all_track_pt_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("tl_%s_matched_track_eta_by_layer%d", tl_modename, ii), 50, -4, 4, [&, ii]() { return tl_matched_track_eta_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("tl_%s_all_track_eta_by_layer%d", tl_modename, ii), 50, -4, 4, [&, ii]() { return tl_all_track_eta_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("tl_%s_matched_track_dxy_by_layer%d", tl_modename, ii), 50, -10, 10, [&, ii]() { return tl_matched_track_dxy_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("tl_%s_all_track_dxy_by_layer%d", tl_modename, ii), 50, -10, 10, [&, ii]() { return tl_all_track_dxy_by_layer[ii]; } );
    }

    for (int ii = 0; ii < nlayers; ++ii)
    {
        ana.histograms.addVecHistogram(TString::Format("tc_%s_matched_track_pt_by_layer%d", tc_modename, ii), pt_boundaries, [&, ii]() { return tc_matched_track_pt_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("tc_%s_all_track_pt_by_layer%d", tc_modename, ii), pt_boundaries, [&, ii]() { return tc_all_track_pt_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("tc_%s_matched_track_eta_by_layer%d", tc_modename, ii), 50, -4, 4, [&, ii]() { return tc_matched_track_eta_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("tc_%s_all_track_eta_by_layer%d", tc_modename, ii), 50, -4, 4, [&, ii]() { return tc_all_track_eta_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("tc_%s_matched_track_dxy_by_layer%d", tc_modename, ii), 50, -10, 10, [&, ii]() { return tc_matched_track_dxy_by_layer[ii]; } );
        ana.histograms.addVecHistogram(TString::Format("tc_%s_all_track_dxy_by_layer%d", tc_modename, ii), 50, -10, 10, [&, ii]() { return tc_all_track_dxy_by_layer[ii]; } );
    }

}

void StudySDLEfficiency::doStudy(SDL::Event& event, std::vector<std::tuple<unsigned int, SDL::Event*>> simtrkevents)
{
    // Each do study is performed per event

    // First clear all the output variables that will be used to fill the histograms for this event
    for (int ii = 0; ii < NLAYERS; ++ii)
    {

        md_matched_track_pt_by_layer[ii].clear();
        md_all_track_pt_by_layer[ii].clear();
        md_matched_track_eta_by_layer[ii].clear();
        md_all_track_eta_by_layer[ii].clear();
        md_matched_track_dxy_by_layer[ii].clear();
        md_all_track_dxy_by_layer[ii].clear();

        sg_matched_track_pt_by_layer[ii].clear();
        sg_all_track_pt_by_layer[ii].clear();
        sg_matched_track_eta_by_layer[ii].clear();
        sg_all_track_eta_by_layer[ii].clear();
        sg_matched_track_dxy_by_layer[ii].clear();
        sg_all_track_dxy_by_layer[ii].clear();

        tl_matched_track_pt_by_layer[ii].clear();
        tl_all_track_pt_by_layer[ii].clear();
        tl_matched_track_eta_by_layer[ii].clear();
        tl_all_track_eta_by_layer[ii].clear();
        tl_matched_track_dxy_by_layer[ii].clear();
        tl_all_track_dxy_by_layer[ii].clear();

        tc_matched_track_pt_by_layer[ii].clear();
        tc_all_track_pt_by_layer[ii].clear();
        tc_matched_track_eta_by_layer[ii].clear();
        tc_all_track_eta_by_layer[ii].clear();
        tc_matched_track_dxy_by_layer[ii].clear();
        tc_all_track_dxy_by_layer[ii].clear();

    }

    //***********************
    // Efficiency calculation
    //***********************

    // Loop over track events
    for (auto& simtrkevent : simtrkevents)
    {

        // Unpack the tuple (sim_track_index, SDL::Event containing reco hits only matched to the given sim track)
        unsigned int& isimtrk = std::get<0>(simtrkevent);
        SDL::Event& trackevent = *(std::get<1>(simtrkevent));

        // Create list of booleans
        std::array<bool, 6> md_trackmatch_by_layer; md_trackmatch_by_layer[0] = false; md_trackmatch_by_layer[1] = false; md_trackmatch_by_layer[2] = false; md_trackmatch_by_layer[3] = false; md_trackmatch_by_layer[4] = false; md_trackmatch_by_layer[5] = false;
        std::array<bool, 6> sg_trackmatch_by_layer; sg_trackmatch_by_layer[0] = false; sg_trackmatch_by_layer[1] = false; sg_trackmatch_by_layer[2] = false; sg_trackmatch_by_layer[3] = false; sg_trackmatch_by_layer[4] = false; sg_trackmatch_by_layer[5] = false;
        std::array<bool, 6> tl_trackmatch_by_layer; tl_trackmatch_by_layer[0] = false; tl_trackmatch_by_layer[1] = false; tl_trackmatch_by_layer[2] = false; tl_trackmatch_by_layer[3] = false; tl_trackmatch_by_layer[4] = false; tl_trackmatch_by_layer[5] = false;
        std::array<bool, 6> tc_trackmatch_by_layer; tc_trackmatch_by_layer[0] = false; tc_trackmatch_by_layer[1] = false; tc_trackmatch_by_layer[2] = false; tc_trackmatch_by_layer[3] = false; tc_trackmatch_by_layer[4] = false; tc_trackmatch_by_layer[5] = false;

        // Parse pt and eta of this track
        float pt = std::min((double) trk.sim_pt()[isimtrk], 49.999);
        float eta = trk.sim_eta()[isimtrk];
        float dxy = trk.sim_pca_dxy()[isimtrk];

        //********************************
        // Mini-doublet/Segment efficiency
        //********************************

        // Loop over the lower modules that contains mini-doublets or segments for this track
        for (auto& lowerModulePtr_Track : trackevent.getLowerModulePtrs())
        {

            // Parse the layer index later to be used for indexing
            int layer_idx = lowerModulePtr_Track->layer() - 1;

            // If there is at least one mini-doublet it means this layer had found a mini-doublet for this sim track
            if (lowerModulePtr_Track->getMiniDoubletPtrs().size() > 0)
            {
                md_trackmatch_by_layer[layer_idx] = true;
            }

            // If there is at least one mini-doublet it means this layer had found a mini-doublet for this sim track
            if (lowerModulePtr_Track->getSegmentPtrs().size() > 0)
            {
                sg_trackmatch_by_layer[layer_idx] = true;
            }

        }

        // Loop over the layers that contains tracklets and/or track candidates
        for (auto& layerPtr_Track : trackevent.getLayerPtrs())
        {

            // Parse the layer index later to be used for indexing
            int layer_idx = layerPtr_Track->layerIdx() - 1;

            // If there is at least one mini-doublet it means this layer had found a mini-doublet for this sim track
            if (layerPtr_Track->getTrackletPtrs().size() > 0)
            {
                tl_trackmatch_by_layer[layer_idx] = true;
            }

            // If there is at least one mini-doublet it means this layer had found a mini-doublet for this sim track
            if (layerPtr_Track->getTrackCandidatePtrs().size() > 0)
            {
                tc_trackmatch_by_layer[layer_idx] = true;
            }

        }

        for (unsigned int i = 0; i < 6; ++i)
        {

            // Denominator for specific layers pt efficiency
            md_all_track_pt_by_layer[i].push_back(pt);
            sg_all_track_pt_by_layer[i].push_back(pt);
            tl_all_track_pt_by_layer[i].push_back(pt);
            tc_all_track_pt_by_layer[i].push_back(pt);

            // Denominator for specific layers eta efficiency (notice the 1 GeV cut)
            if (trk.sim_pt()[isimtrk] > 1.)
            {
                md_all_track_eta_by_layer[i].push_back(eta);
                sg_all_track_eta_by_layer[i].push_back(eta);
                tl_all_track_eta_by_layer[i].push_back(eta);
                tc_all_track_eta_by_layer[i].push_back(eta);
            }

            if (pt > 0.9)
            {
                // Denominator for specific layers dxy efficiency
                md_all_track_dxy_by_layer[i].push_back(dxy);
                sg_all_track_dxy_by_layer[i].push_back(dxy);
                tl_all_track_dxy_by_layer[i].push_back(dxy);
                tc_all_track_dxy_by_layer[i].push_back(dxy);
            }

            // Numerators for matched specific layer pt effciency
            if (md_trackmatch_by_layer[i]) md_matched_track_pt_by_layer[i].push_back(pt);
            if (sg_trackmatch_by_layer[i]) sg_matched_track_pt_by_layer[i].push_back(pt);
            if (tl_trackmatch_by_layer[i]) tl_matched_track_pt_by_layer[i].push_back(pt);
            if (tc_trackmatch_by_layer[i]) tc_matched_track_pt_by_layer[i].push_back(pt);

            // Numerators for matched specific layer eta effciency (notice the 1 GeV cut)
            if (trk.sim_pt()[isimtrk] > 1.)
            {
                if (md_trackmatch_by_layer[i]) md_matched_track_eta_by_layer[i].push_back(eta);
                if (sg_trackmatch_by_layer[i]) sg_matched_track_eta_by_layer[i].push_back(eta);
                if (tl_trackmatch_by_layer[i]) tl_matched_track_eta_by_layer[i].push_back(eta);
                if (tc_trackmatch_by_layer[i]) tc_matched_track_eta_by_layer[i].push_back(eta);
            }

            if (pt > 0.9)
            {
                // Numerator for specific layers dxy efficiency
                if (md_trackmatch_by_layer[i]) md_matched_track_dxy_by_layer[i].push_back(dxy);
                if (sg_trackmatch_by_layer[i]) sg_matched_track_dxy_by_layer[i].push_back(dxy);
                if (tl_trackmatch_by_layer[i]) tl_matched_track_dxy_by_layer[i].push_back(dxy);
                if (tc_trackmatch_by_layer[i]) tc_matched_track_dxy_by_layer[i].push_back(dxy);
            }

        }

    }

}

