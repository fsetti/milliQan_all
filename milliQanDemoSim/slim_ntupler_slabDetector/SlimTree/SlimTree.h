
#ifndef SlimTree_h
#define SlimTree_h

#include <vector>
#include <chrono>
#include <numeric>

#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TDirectory.h"


using namespace std;
const int nchans	= 8;

class SlimTree {
  public:

    int    fileID;
    int    orig_evt;
    float  scale1fb;
    int    procID;
    float  mu_E0;
    float  mu_x0;
    float  mu_y0;
    int    chan_nPE[nchans];
    float  chan_fracMuon[nchans];
    float  chan_fracElScint[nchans];
    float  chan_fracElAl[nchans];
    float  chan_fracElRock[nchans];
    float  chan_fracElPb[nchans];
    float  chan_fracOther[nchans];
    float  chan_firstTime[nchans];
    float  chan_meanTime[nchans];
    float  chan_timeCalib[nchans];
    float  chan_muDist[nchans];
    float  chan_muTime[nchans];
    float  chan_firstPEPathLength[nchans];
    int    chan_maxSourceType[nchans];
    float  chan_maxSourceE0[nchans];
    int    chan_maxSourceNPE[nchans];
    int    chan_type[nchans];
    int    mcTruth_nMuSlabHits;
    int    mcTruth_nMuPanelHits;
    int    mcTruth_nMuBarHits;
    bool   mcTruth_fourSlab;
    bool   mcTruth_fourBarLine;
    bool   mcTruth_noBar;
    bool   mcTruth_noChan;
    int    mcTruth_verticalCosmic;
	float  chan_PEtimes[nchans][160];

    SlimTree(TTree *t=0);
    void Init(TTree *t=0);
    void Fill();
    void Reset();
    void Write(TDirectory *d);
    void GetEntry(ULong64_t entry);
    TTree * tree(){ return t; }
    void progress(int nEventsTotal, int nEventsChain, int period=1000, uint smoothing=30);

  private:
    TTree *t;
    std::chrono::time_point<std::chrono::system_clock> t_old;
    std::vector<double> deq;

    TBranch *b_fileID = 0;
    TBranch *b_orig_evt = 0;
    TBranch *b_scale1fb = 0;
    TBranch *b_procID = 0;
    TBranch *b_mu_E0 = 0;
    TBranch *b_mu_x0 = 0;
    TBranch *b_mu_y0 = 0;
    TBranch *b_chan_nPE = 0;
    TBranch *b_chan_fracMuon = 0;
    TBranch *b_chan_fracElScint = 0;
    TBranch *b_chan_fracElAl = 0;
    TBranch *b_chan_fracElRock = 0;
    TBranch *b_chan_fracElPb = 0;
    TBranch *b_chan_fracOther = 0;
    TBranch *b_chan_firstTime = 0;
    TBranch *b_chan_meanTime = 0;
    TBranch *b_chan_timeCalib = 0;
    TBranch *b_chan_muDist = 0;
    TBranch *b_chan_muTime = 0;
    TBranch *b_chan_firstPEPathLength = 0;
    TBranch *b_chan_maxSourceType = 0;
    TBranch *b_chan_maxSourceE0 = 0;
    TBranch *b_chan_maxSourceNPE = 0;
    TBranch *b_chan_type = 0;
    TBranch *b_mcTruth_nMuSlabHits = 0;
    TBranch *b_mcTruth_nMuPanelHits = 0;
    TBranch *b_mcTruth_nMuBarHits = 0;
    TBranch *b_mcTruth_fourSlab = 0;
    TBranch *b_mcTruth_fourBarLine = 0;
    TBranch *b_mcTruth_noBar = 0;
    TBranch *b_mcTruth_noChan = 0;
    TBranch *b_mcTruth_verticalCosmic = 0;
    TBranch *b_chan_PEtimes[nchans] = {0};

};

#endif
