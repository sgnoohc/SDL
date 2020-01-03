#!/bin/env python

import plottery_wrapper as p
from plottery import plottery as plt
import ROOT as r

import sys

filename = "debug.root"
if len(sys.argv) > 2:
    filename = sys.argv[2]

option = 1
if len(sys.argv) > 1:
    option = int(sys.argv[1])

drawMDplots = False
drawSGplots =False
drawSGSelPlots = False
drawSGTruthSelPlots = False
drawTLplots = False
drawTLSelPlots = False
drawTCplots = False
drawTCSelPlots = False
drawTPSelPlots = False

if option == 1:
    drawMDplots = True

if option == 2:
    drawSGplots = True

if option == 3:
    drawTLplots = True

if option == 4:
    drawTCplots = True

if option == 5:
    drawTLSelPlots = True

if option == 6:
    drawTCSelPlots = True

if option == 7:
    drawTPSelPlots = True

eff_file = r.TFile("eff.root", "recreate")

def plot_eff(num_name, den_name, output_name, dirname="lin"):
    f = r.TFile(filename)
    num = f.Get(num_name)
    den = f.Get(den_name)

    p.plot_hist(bgs=[den.Clone()],
            data=num.Clone(),
            options={
                "yaxis_log":True if "_dxy_" in output_name else False ,
                "legend_smart":False,
                "print_yield":False,
                "output_name":"plots/{}/{}".format(dirname, output_name.replace(".pdf","_numden.pdf")),
                # "remove_underflow":True,
                # "remove_overflow":True,
                # "yaxis_range": [0.95, 1.05] if "eta" in output_name else [],
                "yaxis_range": [0.1, den.GetMaximum() * 1000] if "_dxy_" in output_name else [],
                # "no_ratio":False,
                "draw_points":False,
                "do_stack":False,
                # "print_yield":True,
                "yield_prec":4,
                # "xaxis_log":False if "eta" in output_name else True,
                # "hist_disable_xerrors": True if "eta" in output_name else False,
                # "hist_black_line": True,
                "show_bkg_errors": True,
                "hist_line_black": True,
                "ratio_range": [0., 1.05],
                # "divide_by_bin_width":True,
                "print_yield":True,
                "xaxis_log":True if "_pt" in output_name else False,
                "no_ratio":True,
                # "remove_overflow": True,
                },
        )

    teff = r.TEfficiency(num, den)
    eff = teff.CreateGraph()
    #eff.Print("all")
    c1 = r.TCanvas()
    c1.SetBottomMargin(0.15)
    c1.SetLeftMargin(0.15)
    c1.SetTopMargin(0.15)
    c1.SetRightMargin(0.15)
    if "_pt" in output_name:
        c1.SetLogx()
    eff.Draw("epa")
    eff.SetMarkerStyle(19)
    eff.SetMarkerSize(1.2)
    eff.SetLineWidth(2)
    if "_eta" in output_name:
        title = "#eta"
    elif "phi" in output_name:
        title = "#phi"
    elif "_z" in output_name:
        title = "z [cm]"
    elif "_dxy" in output_name:
        title = "d0 [cm]"
    else:
        title = "p_{T} [GeV]"
    eff.GetXaxis().SetTitle(title)
    eff.GetXaxis().SetTitleSize(0.05)
    eff.GetXaxis().SetLabelSize(0.05)
    eff.GetYaxis().SetLabelSize(0.05)
    if "_eta" in output_name and "sg_" not in output_name:
        eff.GetYaxis().SetRangeUser(0.9, 1.005)
    if "_z" in output_name and "sg_" not in output_name:
        eff.GetYaxis().SetRangeUser(0.98, 1.02)
    if "barrelflat_eta" in output_name:
        eff.GetYaxis().SetRangeUser(0.97, 1.03)
    if "_eta" in output_name and "sg_" in output_name:
        eff.GetYaxis().SetRangeUser(0.9, 1.005)
    if "_z" in output_name and "sg_" in output_name:
        eff.GetYaxis().SetRangeUser(0.98, 1.02)
    if "_ptzoom" in output_name and "sg_" in output_name:
        eff.GetYaxis().SetRangeUser(0.95, 1.05)
    if "_ptzoom" in output_name and "md_" in output_name:
        eff.GetYaxis().SetRangeUser(0.98, 1.02)
    if "_ptzoom" in output_name and "tl_" in output_name:
        eff.GetYaxis().SetRangeUser(0.9, 1.1)
        # eff.GetYaxis().SetRangeUser(0.985, 1.015)
    if "_eta" in output_name and "tl_" in output_name:
        eff.GetYaxis().SetRangeUser(0.9, 1.005)
    c1.SaveAs("plots/{}/{}".format(dirname, output_name))
    c1.SaveAs("plots/{}/{}".format(dirname, output_name.replace("pdf", "png")))
    eff_file.cd()
    eff.SetName(output_name.replace(".png",""))
    eff.Write()

if drawMDplots:

    mdcombos = ["barrel"]

    for mdcombo in mdcombos:

        for i in xrange(6):
            plot_eff("Root__md_{}_matched_track_pt_by_layer{}".format(mdcombo, i), "Root__md_{}_all_track_pt_by_layer{}".format(mdcombo, i), "md_eff_{}_pt_by_layer{}.pdf".format(mdcombo, i), "mdeff")
            plot_eff("Root__md_{}_matched_track_pt_by_layer{}".format(mdcombo, i), "Root__md_{}_all_track_pt_by_layer{}".format(mdcombo, i), "md_eff_{}_ptzoom_by_layer{}.pdf".format(mdcombo, i), "mdeff")
            plot_eff("Root__md_{}_matched_track_eta_by_layer{}".format(mdcombo, i), "Root__md_{}_all_track_eta_by_layer{}".format(mdcombo, i), "md_eff_{}_eta_by_layer{}.pdf".format(mdcombo, i), "mdeff")
            plot_eff("Root__md_{}_matched_track_dxy_by_layer{}".format(mdcombo, i), "Root__md_{}_all_track_dxy_by_layer{}".format(mdcombo, i), "md_eff_{}_dxy_by_layer{}.pdf".format(mdcombo, i), "mdeff")

if drawSGplots:

    sgcombos = ["bb"]

    for sgcombo in sgcombos:
        for i in xrange(5):
            plot_eff("Root__sg_{}_matched_track_pt_by_layer{}".format(sgcombo, i), "Root__sg_{}_all_track_pt_by_layer{}".format(sgcombo, i), "sg_eff_{}_pt_by_layer{}.pdf".format(sgcombo, i), "sgeff")
            plot_eff("Root__sg_{}_matched_track_pt_by_layer{}".format(sgcombo, i), "Root__sg_{}_all_track_pt_by_layer{}".format(sgcombo, i), "sg_eff_{}_ptzoom_by_layer{}.pdf".format(sgcombo, i), "sgeff")
            plot_eff("Root__sg_{}_matched_track_eta_by_layer{}".format(sgcombo, i), "Root__sg_{}_all_track_eta_by_layer{}".format(sgcombo, i), "sg_eff_{}_eta_by_layer{}.pdf".format(sgcombo, i), "sgeff")
            plot_eff("Root__sg_{}_matched_track_dxy_by_layer{}".format(sgcombo, i), "Root__sg_{}_all_track_dxy_by_layer{}".format(sgcombo, i), "sg_eff_{}_dxy_by_layer{}.pdf".format(sgcombo, i), "sgeff")

if drawSGSelPlots:

    sgcombos = ["bb12", "bb23", "bb34", "bb45", "bb56"]
    recovars = ["zLo_cut", "sdCut", "sdSlope", "sdMuls", "sdPVoff", "deltaPhi"]

    for sgcombo in sgcombos:

        p.dump_plot(fnames=[filename],
            dirname="plots/segment",
            dogrep=False,
            filter_pattern="Root__sg_{}_cutflow".format(sgcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":True, "remove_overflow":False, "remove_underflow":False},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/segment_log",
            dogrep=False,
            filter_pattern="Root__sg_{}_cutflow".format(sgcombo),
            extraoptions={"yaxis_log":True, "legend_smart":False, "print_yield":True},
            )

        for recovar in recovars:

            p.dump_plot(fnames=[filename],
                dirname="plots/segment",
                dogrep=False,
                filter_pattern="Root__sg_{}_{}".format(sgcombo, recovar),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":True, "remove_overflow":True, "remove_underflow":False},
                )

if drawSGTruthSelPlots:

    sgcombos = ["bb12", "bb23", "bb34", "bb45", "bb56"]
    recovars = ["zLo_cut", "sdCut", "sdSlope", "sdMuls", "sdPVoff", "deltaPhi"]

    for sgcombo in sgcombos:

        for recovar in recovars:

            p.dump_plot(fnames=[filename],
                dirname="plots/segment",
                dogrep=False,
                filter_pattern="Root__sg_truth_{}_{}".format(sgcombo, recovar),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":True, "remove_overflow":True, "remove_underflow":False},
                )

if drawTLplots:

    tlcombos = ["bbbb"]

    for tlcombo in tlcombos:
        for i in xrange(3):
            plot_eff("Root__tl_{}_matched_track_pt_by_layer{}".format(tlcombo, i), "Root__tl_{}_all_track_pt_by_layer{}".format(tlcombo, i), "tl_eff_{}_pt_by_layer{}.pdf".format(tlcombo, i), "tleff")
            plot_eff("Root__tl_{}_matched_track_pt_by_layer{}".format(tlcombo, i), "Root__tl_{}_all_track_pt_by_layer{}".format(tlcombo, i), "tl_eff_{}_ptzoom_by_layer{}.pdf".format(tlcombo, i), "tleff")
            plot_eff("Root__tl_{}_matched_track_eta_by_layer{}".format(tlcombo, i), "Root__tl_{}_all_track_eta_by_layer{}".format(tlcombo, i), "tl_eff_{}_eta_by_layer{}.pdf".format(tlcombo, i), "tleff")
            plot_eff("Root__tl_{}_matched_track_dxy_by_layer{}".format(tlcombo, i), "Root__tl_{}_all_track_dxy_by_layer{}".format(tlcombo, i), "tl_eff_{}_dxy_by_layer{}.pdf".format(tlcombo, i), "tleff")

if drawTLSelPlots:

    tlcombos = ["bb1bb3", "bb1bb4", "bb1bb5", "bb2bb4", "bb3bb5", "all"]

    for tlcombo in tlcombos:

        for do_und_ov_flow in [True, False]:

            suffix = "" if do_und_ov_flow else "_w_overflow"

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta_midpoint".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta_3rdCorr".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta_4thCorr".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta_3rdCorr_maxzoom".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow, "xaxis_ndivisions":405},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta_4thCorr_maxzoom".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow, "xaxis_ndivisions":405},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta_standard".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta_midpoint_standard".format(tlcombo),
                # extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta_3rdCorr_standard".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta_4thCorr_standard".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta_zoom".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta_maxzoom".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow, "xaxis_ndivisions":405},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta_slava".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet_log{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta".format(tlcombo),
                extraoptions={"yaxis_log":True, "legend_smart":False, "print_yield":False},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_cutflow".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":True, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet_log{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_cutflow".format(tlcombo),
                extraoptions={"yaxis_log":True, "legend_smart":False, "print_yield":True},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_{}_deltaBeta_postCut".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow, "nbins":1},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_truth_{}_deltaBeta".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet_log{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_truth_{}_deltaBeta".format(tlcombo),
                extraoptions={"yaxis_log":True, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_truth_{}_deltaBeta_4thCorr_maxzoom".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet_log{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_truth_{}_deltaBeta_4thCorr_maxzoom".format(tlcombo),
                extraoptions={"yaxis_log":True, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_truth_{}_deltaBeta_4thCorr".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet_log{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_truth_{}_deltaBeta_4thCorr".format(tlcombo),
                extraoptions={"yaxis_log":True, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

            p.dump_plot(fnames=[filename],
                dirname="plots/tracklet{}".format(suffix),
                dogrep=False,
                filter_pattern="Root__tl_truth_{}_cutflow".format(tlcombo),
                extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":True, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
                )

if drawTCplots:
    tccombos = ["bbbbbb"]

    for tccombo in tccombos:
        for i in xrange(1):
            plot_eff("Root__tc_{}_matched_track_pt_by_layer{}".format(tccombo, i), "Root__tc_{}_all_track_pt_by_layer{}".format(tccombo, i), "tc_eff_{}_pt_by_layer{}.pdf".format(tccombo, i), "tceff")
            plot_eff("Root__tc_{}_matched_track_pt_by_layer{}".format(tccombo, i), "Root__tc_{}_all_track_pt_by_layer{}".format(tccombo, i), "tc_eff_{}_ptzoom_by_layer{}.pdf".format(tccombo, i), "tceff")
            plot_eff("Root__tc_{}_matched_track_eta_by_layer{}".format(tccombo, i), "Root__tc_{}_all_track_eta_by_layer{}".format(tccombo, i), "tc_eff_{}_eta_by_layer{}.pdf".format(tccombo, i), "tceff")
            plot_eff("Root__tc_{}_matched_track_dxy_by_layer{}".format(tccombo, i), "Root__tc_{}_all_track_dxy_by_layer{}".format(tccombo, i), "tc_eff_{}_dxy_by_layer{}.pdf".format(tccombo, i), "tceff")

if drawTCSelPlots:

    tlcombos = ["all"]

    for tlcombo in tlcombos:

        p.dump_plot(fnames=[filename],
            dirname="plots/trackcandidate",
            dogrep=False,
            filter_pattern="Root__tc_{}_inner_tl_deltaBeta".format(tlcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/trackcandidate",
            dogrep=False,
            filter_pattern="Root__tc_{}_nocut_inner_tl_deltaBeta".format(tlcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/trackcandidate",
            dogrep=False,
            filter_pattern="Root__tc_{}_outer_tl_deltaBeta".format(tlcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/trackcandidate",
            dogrep=False,
            filter_pattern="Root__tc_{}_nocut_outer_tl_deltaBeta".format(tlcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/trackcandidate",
            dogrep=False,
            filter_pattern="Root__tc_{}_inner_tl_betaIn_minus_outer_tl_betaOut".format(tlcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/trackcandidate",
            dogrep=False,
            filter_pattern="Root__tc_{}_dr".format(tlcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/trackcandidate",
            dogrep=False,
            filter_pattern="Root__tc_{}_dr_v_tc_{}_r".format(tlcombo,tlcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/trackcandidate",
            dogrep=False,
            filter_pattern="Root__truth_tc_{}_dr_v_truth_tc_{}_r".format(tlcombo,tlcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/trackcandidate",
            dogrep=False,
            filter_pattern="Root__truth_gt1pt_tc_{}_dr_v_truth_gt1pt_tc_{}_r".format(tlcombo,tlcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/trackcandidate",
            dogrep=False,
            filter_pattern="Root__tc_{}_cutflow".format(tlcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":True, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/trackcandidate",
            dogrep=False,
            filter_pattern="Root__tc_{}_inner_tl_betaAv_minus_outer_tl_betaAv".format(tlcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":True, "remove_overflow":do_und_ov_flow, "remove_underflow":do_und_ov_flow},
            )

if drawTPSelPlots:

    tpcombos = ["bb1bb2", "bb2bb3", "bb3bb4", "bb4bb5"]

    for tpcombo in tpcombos:

        p.dump_plot(fnames=[filename],
            dirname="plots/triplet",
            dogrep=False,
            filter_pattern="Root__tp_{}_deltaBeta".format(tpcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":False, "remove_underflow":False},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/triplet",
            dogrep=False,
            filter_pattern="Root__tp_{}_deltaBeta_midpoint".format(tpcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":False, "remove_underflow":False},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/triplet",
            dogrep=False,
            filter_pattern="Root__tp_{}_deltaBeta_3rdCorr".format(tpcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":False, "remove_underflow":False},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/triplet",
            dogrep=False,
            filter_pattern="Root__tp_{}_deltaBeta_4thCorr".format(tpcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":False, "remove_underflow":False},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/triplet",
            dogrep=False,
            filter_pattern="Root__tp_{}_deltaBeta_3rdCorr_maxzoom".format(tpcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":False, "remove_underflow":False},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/triplet",
            dogrep=False,
            filter_pattern="Root__tp_{}_deltaBeta_4thCorr_maxzoom".format(tpcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":False, "remove_underflow":False},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/triplet",
            dogrep=False,
            filter_pattern="Root__tp_{}_deltaBeta_standard".format(tpcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":False, "remove_underflow":False},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/triplet",
            dogrep=False,
            filter_pattern="Root__tp_{}_deltaBeta_midpoint_standard".format(tpcombo),
            # extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":False, "remove_underflow":False},
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":False, "remove_underflow":False},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/triplet",
            dogrep=False,
            filter_pattern="Root__tp_{}_deltaBeta_3rdCorr_standard".format(tpcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":False, "remove_underflow":False},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/triplet",
            dogrep=False,
            filter_pattern="Root__tp_{}_deltaBeta_4thCorr_standard".format(tpcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":False, "remove_underflow":False},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/triplet",
            dogrep=False,
            filter_pattern="Root__tp_{}_deltaBeta_zoom".format(tpcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":False, "remove_underflow":False},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/triplet",
            dogrep=False,
            filter_pattern="Root__tp_{}_deltaBeta_maxzoom".format(tpcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":False, "remove_underflow":False},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/triplet",
            dogrep=False,
            filter_pattern="Root__tp_{}_deltaBeta_slava".format(tpcombo),
            extraoptions={"yaxis_log":False, "legend_smart":False, "print_yield":False, "remove_overflow":False, "remove_underflow":False},
            )

        p.dump_plot(fnames=[filename],
            dirname="plots/triplet_log",
            dogrep=False,
            filter_pattern="Root__tp_{}_deltaBeta".format(tpcombo),
            extraoptions={"yaxis_log":True, "legend_smart":False, "print_yield":False},
            )

