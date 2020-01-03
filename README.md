# TrackLooper

## Install

    git clone --recurse-submodules git@github.com:sgnoohc/TrackLooper.git
    cd TrackLooper
    source rooutil/thisrooutil.sh # set rooutil
    source rooutil/root.sh # set ROOT
    make clean;
    make -j;

## Running Executable ```doAnalysis```

    $ ./doAnalysis
    
             **********************
             *                    *
             *       Looper       *
             *                    *
             **********************
    
    Usage:
    
      $ doAnalysis [OPTION...]
    
      -i, --input arg    Comma separated input file list OR if just a directory
                         is provided it will glob all in the directory BUT must end
                         with '/' for the path
      -t, --tree arg     Name of the tree in the root file to open and loop over
      -o, --output arg   Output file name
      -n, --nevents arg  N events to loop over (default: -1)
      -d, --debug        Run debug job. i.e. overrides output option to
                         'debug.root' and 'recreate's the file.
      -h, --help         Print help

To Run a job

    ./doAnalysis -i <path/to/trackingNtuple.root> -t <path/to/TTree/in/trackingNtuple/root/file> -o <path/to/output.root> -n <nevents>

Or to run a test (debug) job (-o option will be ignored and will output to ```debug.root``` regardless of what you provide)

    ./doAnalysis -i <path/to/trackingNtuple.root> -t <path/to/TTree/in/trackingNtuple/root/file> (if provided will be ignored: -o <path/to/output.root>) -n <nevents> -d

Example:

    ./doAnalysis -i /hadoop/cms/store/user/slava77/CMSSW_10_4_0_patch1-tkNtuple/pass-e072c1a/27411.0_TenMuExtendedE_0_200/trackingNtuple.root -d -n -1 -t trackingNtuple/tree -n 1 # Running 1 event only

Example output running over the first two events

    $ ./doAnalysis -i /hadoop/cms/store/user/slava77/CMSSW_10_4_0_patch1-tkNtuple/pass-e072c1a/27411.0_TenMuExtendedE_0_200/trackingNtuple.root -d -n -1 -t trackingNtuple/tree -n 2
    =========================================================
     Setting of the analysis job based on provided arguments
    ---------------------------------------------------------
     ana.input_file_list_tstring: /hadoop/cms/store/user/slava77/CMSSW_10_4_0_patch1-tkNtuple/pass-e072c1a/27411.0_TenMuExtendedE_0_200/trackingNtuple.root
     ana.output_tfile: debug.root
     ana.n_events: 2
    =========================================================
    RooUtil:: Adding /hadoop/cms/store/user/slava77/CMSSW_10_4_0_patch1-tkNtuple/pass-e072c1a/27411.0_TenMuExtendedE_0_200/trackingNtuple.root
    RooUtil:: Start EventLooping
    RooUtil:: System info:
    uaf-10.t2.ucsd.edu
    Linux uaf-10.t2.ucsd.edu 2.6.32-754.3.5.el6.x86_64 #1 SMP Tue Aug 14 20:46:41 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux
    Fri Apr 12 07:58:34 PDT 2019
    phchang
    /home/users/phchang/public_html/analysis/sdl/TrackLooper
    RooUtil::  _
    RooUtil:: /\\
    RooUtil:: \ \\  \__/ \__/
    RooUtil::  \ \\ (oo) (oo)  Here we come!
    RooUtil::   \_\\/~~\_/~~\_
    RooUtil::  _.-~===========~-._
    RooUtil:: (___________________)
    RooUtil::       \_______/
    RooUtil::
    RooUtil::  Your friendly aliens
    RooUtil::      Surf & Turf
    RooUtil::
    Warning in <TClass::Init>: no dictionary for class ROOT::TIOFeatures is available
    RooUtil:: Total Events in this Chain to process = 2
    RooUtil:: Cut name                                                                                                                                                                                                                                                              |pass|weight|systs
    RooUtil:: ================================================================================================================================================================================================================================================================================================================================================================
    RooUtil:: Root                                                                                                                                                                                                                                                                  | 0 | 0.000000|
    RooUtil:: TTreeCache enabled
    RooUtil:: Looping /hadoop/cms/store/user/slava77/CMSSW_10_4_0_patch1-tkNtuple/pass-e072c1a/27411.0_TenMuExtendedE_0_200/trackingNtuple.root/TTree:tree
    SDL::
    SDL::  ==============
    SDL::  Printing Event
    SDL::  ==============
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=412361801, subdet=4, side=1, layer=5, rod=0, ring=2, module=18, isLower=1, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=-27.3033, y=-26.7023, z=-264.03, r3=266.778, rt=38.1901, phi=-2.36732)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=412361802, subdet=4, side=1, layer=5, rod=0, ring=2, module=18, isLower=0, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=-27.1477, y=-26.5467, z=-264.43, r3=267.142, rt=37.9701, phi=-2.36739)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=420746257, subdet=4, side=2, layer=5, rod=0, ring=1, module=4, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=27.8826, y=24.5097, z=265.97, r3=268.548, rt=37.1236, phi=0.721109)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=420746258, subdet=4, side=2, layer=5, rod=0, ring=1, module=4, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=27.4123, y=24.0464, z=265.57, r3=268.062, rt=36.4646, phi=0.720083)
    SDL::
    SDL::
    SDL::  ==============
    SDL::  Printing Event
    SDL::  ==============
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=412357665, subdet=4, side=1, layer=5, rod=0, ring=1, module=8, isLower=1, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=-1.55569, y=33.8583, z=-267.045, r3=269.187, rt=33.894, phi=1.61671)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=412357666, subdet=4, side=1, layer=5, rod=0, ring=1, module=8, isLower=0, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=-1.57039, y=34.078, z=-267.445, r3=269.612, rt=34.1142, phi=1.61685)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=420746329, subdet=4, side=2, layer=5, rod=0, ring=1, module=22, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=3.84125, y=-33.6007, z=265.97, r3=268.112, rt=33.8196, phi=-1.45697)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=420746330, subdet=4, side=2, layer=5, rod=0, ring=1, module=22, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=3.86589, y=-33.8194, z=265.57, r3=267.743, rt=34.0396, phi=-1.45698)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=437529621, subdet=5, side=1, layer=1, rod=11, ring=0, module=5, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-0.873181, y=26.3079, z=-20.8564, r3=33.5836, rt=26.3224, phi=1.60397)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=437529622, subdet=5, side=1, layer=1, rod=11, ring=0, module=5, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-0.873181, y=25.9697, z=-20.8163, r3=33.2942, rt=25.9843, phi=1.60441)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=437530645, subdet=5, side=1, layer=1, rod=12, ring=0, module=5, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-0.793181, y=23.733, z=-18.7553, r3=30.2596, rt=23.7462, phi=1.6042)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=437530646, subdet=5, side=1, layer=1, rod=12, ring=0, module=5, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-0.783181, y=24.253, z=-17.9148, r3=30.1623, rt=24.2657, phi=1.60308)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=437781561, subdet=5, side=2, layer=1, rod=1, ring=0, module=14, isLower=1, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=0.946819, y=-25.4994, z=18.9445, r3=31.7806, rt=25.517, phi=-1.53368)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=437781562, subdet=5, side=2, layer=1, rod=1, ring=0, module=14, isLower=0, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=0.946819, y=-25.7303, z=19.0846, r3=32.0495, rt=25.7478, phi=-1.53402)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=438050845, subdet=5, side=3, layer=1, rod=8, ring=0, module=7, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-21.8842, y=10.623, z=12.3629, r3=27.2875, rt=24.3262, phi=2.68968)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=438050846, subdet=5, side=3, layer=1, rod=8, ring=0, module=7, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-21.649, y=10.5103, z=11.7027, r3=26.76, rt=24.0655, phi=2.68962)
    SDL::  Lower Hit(x=-21.649, y=10.5103, z=11.7027, r3=26.76, rt=24.0655, phi=2.68962)
    SDL::  Upper Hit(x=-21.8842, y=10.623, z=12.3629, r3=27.2875, rt=24.3262, phi=2.68968)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=438060037, subdet=5, side=3, layer=1, rod=17, ring=0, module=1, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=19.768, y=-9.52823, z=-12.3629, r3=25.1874, rt=21.9445, phi=-0.449146)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=438060038, subdet=5, side=3, layer=1, rod=17, ring=0, module=1, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=19.5329, y=-9.41556, z=-11.7027, r3=24.6402, rt=21.6838, phi=-0.449174)
    SDL::  Lower Hit(x=19.5329, y=-9.41556, z=-11.7027, r3=24.6402, rt=21.6838, phi=-0.449174)
    SDL::  Upper Hit(x=19.768, y=-9.52823, z=-12.3629, r3=25.1874, rt=21.9445, phi=-0.449146)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=438578205, subdet=5, side=1, layer=2, rod=11, ring=0, module=7, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-1.26296, y=38.9612, z=-30.5399, r3=49.5203, rt=38.9817, phi=1.6032)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=438578206, subdet=5, side=1, layer=2, rod=11, ring=0, module=7, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-1.26296, y=38.0548, z=-31.2156, r3=49.2359, rt=38.0757, phi=1.60397)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=438579229, subdet=5, side=1, layer=2, rod=12, ring=0, module=7, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-1.18296, y=36.3209, z=-28.4771, r3=46.1687, rt=36.3401, phi=1.60335)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=438579230, subdet=5, side=1, layer=2, rod=12, ring=0, module=7, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-1.18296, y=36.546, z=-27.8043, r3=45.9357, rt=36.5652, phi=1.60315)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=438830161, subdet=5, side=2, layer=2, rod=1, ring=0, module=20, isLower=1, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=1.42694, y=-37.9875, z=28.3863, r3=47.4433, rt=38.0143, phi=-1.53325)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=438830162, subdet=5, side=2, layer=2, rod=1, ring=0, module=20, isLower=0, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=1.43694, y=-38.0452, z=28.722, r3=47.6912, rt=38.0723, phi=-1.53305)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=439103529, subdet=5, side=3, layer=2, rod=12, ring=0, module=10, isLower=1, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=-33.9718, y=16.5333, z=19.621, r3=42.5725, rt=37.7814, phi=2.68866)
    SDL::  Lower Hit(x=-33.9718, y=16.5333, z=19.621, r3=42.5725, rt=37.7814, phi=2.68866)
    SDL::  Upper Hit(x=-34.1179, y=16.5994, z=19.4009, r3=42.6141, rt=37.9417, phi=2.68878)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=439103530, subdet=5, side=3, layer=2, rod=12, ring=0, module=10, isLower=0, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=-34.1179, y=16.5994, z=19.4009, r3=42.6141, rt=37.9417, phi=2.68878)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=439116809, subdet=5, side=3, layer=2, rod=25, ring=0, module=2, isLower=1, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=31.8812, y=-15.334, z=-19.621, r3=40.454, rt=35.3772, phi=-0.448311)
    SDL::  Lower Hit(x=31.8812, y=-15.334, z=-19.621, r3=40.454, rt=35.3772, phi=-0.448311)
    SDL::  Upper Hit(x=32.0255, y=-15.4048, z=-19.4009, r3=40.4887, rt=35.5379, phi=-0.448346)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=439116810, subdet=5, side=3, layer=2, rod=25, ring=0, module=2, isLower=0, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=32.0255, y=-15.4048, z=-19.4009, r3=40.4887, rt=35.5379, phi=-0.448346)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=439626793, subdet=5, side=1, layer=3, rod=11, ring=0, module=10, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-1.67308, y=53.0021, z=-41.2306, r3=67.1713, rt=53.0285, phi=1.60235)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=439626794, subdet=5, side=1, layer=3, rod=11, ring=0, module=10, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-1.67308, y=53.1717, z=-40.6807, r3=66.9698, rt=53.1981, phi=1.60225)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=439879793, subdet=5, side=2, layer=3, rod=2, ring=0, module=28, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=2.07308, y=-53.7154, z=40.4919, r3=67.2997, rt=53.7554, phi=-1.53222)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=439879794, subdet=5, side=2, layer=3, rod=2, ring=0, module=28, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=2.06308, y=-53.1717, z=40.6807, r3=66.9807, rt=53.2117, phi=-1.53202)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=440156217, subdet=5, side=3, layer=3, rod=16, ring=0, module=14, isLower=1, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=-45.4832, y=22.1748, z=26.4002, r3=57.0737, rt=50.6008, phi=2.68796)
    SDL::  Lower Hit(x=-45.4832, y=22.1748, z=26.4002, r3=57.0737, rt=50.6008, phi=2.68796)
    SDL::  Upper Hit(x=-45.6268, y=22.2461, z=26.3268, r3=57.1821, rt=50.7611, phi=2.68794)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=440156218, subdet=5, side=3, layer=3, rod=16, ring=0, module=14, isLower=0, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=-45.6268, y=22.2461, z=26.3268, r3=57.1821, rt=50.7611, phi=2.68794)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=440174601, subdet=5, side=3, layer=3, rod=34, ring=0, module=2, isLower=1, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=45.6407, y=-21.902, z=-27.7205, r3=57.7165, rt=50.6238, phi=-0.447421)
    SDL::  Lower Hit(x=45.6407, y=-21.902, z=-27.7205, r3=57.7165, rt=50.6238, phi=-0.447421)
    SDL::  Upper Hit(x=45.7868, y=-21.969, z=-28.674, r3=58.3203, rt=50.7845, phi=-0.447367)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=440174602, subdet=5, side=3, layer=3, rod=34, ring=0, module=2, isLower=0, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=45.7868, y=-21.969, z=-28.674, r3=58.3203, rt=50.7845, phi=-0.447367)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=441201693, subdet=5, side=3, layer=4, rod=13, ring=0, module=7, isLower=1, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=-2.14917, y=70.415, z=-56.947, r3=90.5862, rt=70.4478, phi=1.60131)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=441201694, subdet=5, side=3, layer=4, rod=13, ring=0, module=7, isLower=0, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=-2.14917, y=70.595, z=-56.947, r3=90.7262, rt=70.6277, phi=1.60123)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=441210945, subdet=5, side=3, layer=4, rod=22, ring=0, module=16, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-60.2973, y=29.4697, z=37.463, r3=76.8616, rt=67.1135, phi=2.68699)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=441210946, subdet=5, side=3, layer=4, rod=22, ring=0, module=16, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-60.1362, y=29.3883, z=37.463, r3=76.704, rt=66.933, phi=2.68703)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=441226313, subdet=5, side=3, layer=4, rod=37, ring=0, module=18, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=2.77383, y=-70.145, z=52.0838, r3=87.4113, rt=70.1998, phi=-1.53127)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=441226314, subdet=5, side=3, layer=4, rod=37, ring=0, module=18, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=2.76483, y=-69.965, z=52.0838, r3=87.2666, rt=70.0196, phi=-1.5313)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=441235493, subdet=5, side=3, layer=4, rod=46, ring=0, module=9, isLower=1, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=60.7503, y=-29.0817, z=-37.1815, r3=76.9337, rt=67.3523, phi=-0.44647)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=441235494, subdet=5, side=3, layer=4, rod=46, ring=0, module=9, isLower=0, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=60.9131, y=-29.1589, z=-37.1815, r3=77.0916, rt=67.5326, phi=-0.44646)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=442253337, subdet=5, side=3, layer=5, rod=16, ring=0, module=6, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-2.57583, y=87.445, z=-66.247, r3=109.736, rt=87.4829, phi=1.60024)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=442253338, subdet=5, side=3, layer=5, rod=16, ring=0, module=6, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-2.56683, y=87.265, z=-66.247, r3=109.592, rt=87.3027, phi=1.6002)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=442264645, subdet=5, side=3, layer=5, rod=27, ring=0, module=17, isLower=1, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=-75.9793, y=37.234, z=42.5673, r3=94.7164, rt=84.6122, phi=2.68593)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=442264646, subdet=5, side=3, layer=5, rod=27, ring=0, module=17, isLower=0, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=-76.142, y=37.3113, z=42.5673, r3=94.8773, rt=84.7923, phi=2.68596)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=442284109, subdet=5, side=3, layer=5, rod=46, ring=0, module=19, isLower=1, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=3.57117, y=-87.715, z=67.2854, r3=110.607, rt=87.7877, phi=-1.53011)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=442284110, subdet=5, side=3, layer=5, rod=46, ring=0, module=19, isLower=0, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=3.57567, y=-87.895, z=67.2854, r3=110.75, rt=87.9677, phi=-1.53014)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=442295329, subdet=5, side=3, layer=5, rod=57, ring=0, module=8, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=76.0874, y=-36.3274, z=-46.7952, r3=96.4301, rt=84.3147, phi=-0.44544)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=442295330, subdet=5, side=3, layer=5, rod=57, ring=0, module=8, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=75.9248, y=-36.2501, z=-46.7952, r3=96.2727, rt=84.1347, phi=-0.445443)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=443306001, subdet=5, side=3, layer=6, rod=20, ring=0, module=4, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-3.12483, y=112.245, z=-85.5456, r3=141.162, rt=112.288, phi=1.59863)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=443306002, subdet=5, side=3, layer=6, rod=20, ring=0, module=4, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-3.12033, y=112.065, z=-85.5456, r3=141.019, rt=112.108, phi=1.59863)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=443320393, subdet=5, side=3, layer=6, rod=34, ring=0, module=18, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-100.722, y=49.5643, z=57.1088, r3=125.948, rt=112.256, phi=2.68429)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=443320394, subdet=5, side=3, layer=6, rod=34, ring=0, module=18, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=-100.561, y=49.483, z=57.1088, r3=125.788, rt=112.076, phi=2.68431)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=443347029, subdet=5, side=3, layer=6, rod=60, ring=0, module=21, isLower=1, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=4.75485, y=-112.497, z=86.8088, r3=142.176, rt=112.598, phi=-1.52856)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=443347030, subdet=5, side=3, layer=6, rod=60, ring=0, module=21, isLower=0, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=4.76485, y=-112.677, z=86.8088, r3=142.319, rt=112.778, phi=-1.52853)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=443360281, subdet=5, side=3, layer=6, rod=73, ring=0, module=6, isLower=0, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=98.5112, y=-46.8517, z=-61.222, r3=125.091, rt=109.085, phi=-0.443936)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=443360282, subdet=5, side=3, layer=6, rod=73, ring=0, module=6, isLower=1, isInverted=1)
    SDL::  ==============================
    SDL::  Hit(x=98.3467, y=-46.7786, z=-61.222, r3=124.934, rt=108.905, phi=-0.443979)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=443360285, subdet=5, side=3, layer=6, rod=73, ring=0, module=7, isLower=1, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=98.7555, y=-46.9667, z=-56.947, r3=123.294, rt=109.355, phi=-0.443926)
    SDL::
    SDL::  ==============================
    SDL::  Module(detId=443360286, subdet=5, side=3, layer=6, rod=73, ring=0, module=7, isLower=0, isInverted=0)
    SDL::  ==============================
    SDL::  Hit(x=98.9181, y=-47.0439, z=-56.947, r3=123.454, rt=109.535, phi=-0.443925)
    SDL::
    RooUtil:: Wrote output to debug.root
    RooUtil::
    RooUtil::    \__/    \__/
    RooUtil::  S (oo)    (oo)
    RooUtil:: (\//~~\\  //~~\\
    RooUtil::  \/\__//  \\__//\T
    RooUtil::    ||||    ||\\ Who cares!
    RooUtil:: __ |||| __ |||| ___
    RooUtil::   (_)(_)  (_)(_)
    RooUtil::

    ------------------------------
    CPU  Time:      3.5
    Real Time:      4.7

## Efficiency plots

    python plot.py
## High pileup sample location
    
    /home/users/phchang/public_html/analysis/sdl/trackingNtuple/CMSSW_10_4_0_mtd5/src/trackingNtuple_TTbar_PU200.root


