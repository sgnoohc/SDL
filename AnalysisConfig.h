#ifndef AnalysisConfig_h
#define AnalysisConfig_h

#include "TString.h"
#include "rooutil.h"
#include "trktree.h"

class AnalysisConfig
{

public:

    // TString that holds the input file list (comma separated)
    TString input_file_list_tstring;

    // TString that holds the name of the TTree to open for each input files
    TString input_tree_name;

    // Output TFile
    TFile* output_tfile;

    // Number of events to loop over
    int n_events;

    // specific event_index to process
    int specific_event_index;

    // run efficiency study
    bool run_eff_study;

    // pt binning options
    int ptbound_mode;

    // pdg id of the particles to compute efficincies on
    int pdg_id;

    // verbose of the particles to compute efficincies on
    int verbose;

    // to print module connection info
    bool print_conn;

    // to print centroid
    bool print_centroid;

    // Debug boolean
    bool debug;

    // TChain that holds the input TTree's
    TChain* events_tchain;

    // Custom Looper object to facilitate looping over many files
    RooUtil::Looper<trktree> looper;

    // Custom Cutflow framework
    RooUtil::Cutflow cutflow;

    // Custom Histograms object compatible with RooUtil::Cutflow framework
    RooUtil::Histograms histograms;

    // TTree output for studies
    TTree* output_ttree;

    // TTreeX to facilitate writing output to ttree
    RooUtil::TTreeX* tx;

};

extern AnalysisConfig ana;

#endif
