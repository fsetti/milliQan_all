// adapted F. Setti, Jul. 2019.
#include "TStyle.h"
#include "TFile.h"
#include "TH1F.h"
#include "SlimTree/SlimTree.h"
#include "../../headers/mCPs/analysis.cc"


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
bool   mcTruth_twoSlab;
bool   mcTruth_fourBarLine;
bool   mcTruth_noBar;
bool   mcTruth_noChan;
int    mcTruth_verticalCosmic;
float  chan_PEtimes[nchans][160];


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
TBranch *b_mcTruth_twoSlab = 0;
TBranch *b_mcTruth_fourBarLine = 0;
TBranch *b_mcTruth_noBar = 0;
TBranch *b_mcTruth_noChan = 0;
TBranch *b_mcTruth_verticalCosmic = 0;
TBranch *b_chan_PEtimes[nchans] = {0};


void selection( TChain *chain ) 
{
	chain->SetBranchAddress("fileID", &fileID, &b_fileID);
	chain->SetBranchAddress("orig_evt", &orig_evt, &b_orig_evt);
	chain->SetBranchAddress("scale1fb", &scale1fb, &b_scale1fb);
	chain->SetBranchAddress("procID", &procID, &b_procID);
	chain->SetBranchAddress("mu_E0", &mu_E0, &b_mu_E0);
	chain->SetBranchAddress("mu_x0", &mu_x0, &b_mu_x0);
	chain->SetBranchAddress("mu_y0", &mu_y0, &b_mu_y0);
	chain->SetBranchAddress("chan_nPE", chan_nPE, &b_chan_nPE);
	chain->SetBranchAddress("chan_fracMuon", chan_fracMuon, &b_chan_fracMuon);
	chain->SetBranchAddress("chan_fracElScint", chan_fracElScint, &b_chan_fracElScint);
	chain->SetBranchAddress("chan_fracElAl", chan_fracElAl, &b_chan_fracElAl);
	chain->SetBranchAddress("chan_fracElRock", chan_fracElRock, &b_chan_fracElRock);
	chain->SetBranchAddress("chan_fracElPb", chan_fracElPb, &b_chan_fracElPb);
	chain->SetBranchAddress("chan_fracOther", chan_fracOther, &b_chan_fracOther);
	chain->SetBranchAddress("chan_firstTime", chan_firstTime, &b_chan_firstTime);
	chain->SetBranchAddress("chan_meanTime", chan_meanTime, &b_chan_meanTime);
	chain->SetBranchAddress("chan_timeCalib", chan_timeCalib, &b_chan_timeCalib);
	chain->SetBranchAddress("chan_muDist", chan_muDist, &b_chan_muDist);
	chain->SetBranchAddress("chan_muTime", chan_muTime, &b_chan_muTime);
	chain->SetBranchAddress("chan_firstPEPathLength", chan_firstPEPathLength, &b_chan_firstPEPathLength);
	chain->SetBranchAddress("chan_maxSourceType", chan_maxSourceType, &b_chan_maxSourceType);
	chain->SetBranchAddress("chan_maxSourceE0", chan_maxSourceE0, &b_chan_maxSourceE0);
	chain->SetBranchAddress("chan_maxSourceNPE", chan_maxSourceNPE, &b_chan_maxSourceNPE);
	chain->SetBranchAddress("chan_type", chan_type, &b_chan_type);
	chain->SetBranchAddress("mcTruth_nMuSlabHits", &mcTruth_nMuSlabHits, &b_mcTruth_nMuSlabHits);
	chain->SetBranchAddress("mcTruth_nMuPanelHits", &mcTruth_nMuPanelHits, &b_mcTruth_nMuPanelHits);
	chain->SetBranchAddress("mcTruth_nMuBarHits", &mcTruth_nMuBarHits, &b_mcTruth_nMuBarHits);
	chain->SetBranchAddress("mcTruth_twoSlab", &mcTruth_twoSlab, &b_mcTruth_twoSlab);
	chain->SetBranchAddress("mcTruth_fourBarLine", &mcTruth_fourBarLine, &b_mcTruth_fourBarLine);
	chain->SetBranchAddress("mcTruth_noBar", &mcTruth_noBar, &b_mcTruth_noBar);
	chain->SetBranchAddress("mcTruth_noChan", &mcTruth_noChan, &b_mcTruth_noChan);
	chain->SetBranchAddress("mcTruth_verticalCosmic", &mcTruth_verticalCosmic, &b_mcTruth_verticalCosmic);

	TFile *f = new TFile( "test.root", "UPDATE");
	TH1F *h_dT = new TH1F("dT","dT between layer 2-1", 60 , -50 , 50 );

	//Number of events to loop over
	Int_t nentries = (Int_t)chain->GetEntries();
	tqdm bar;	
	for(int ia = 0; ia<nentries; ia++){
	
		chain->GetEntry(ia);
		bar.progress(ia,nentries);

		for ( unsigned int ch=1; ch< 217; ch++){
			if ( chan_fracMuon[ch] >= 0 && chan_fracMuon[ch+216] >= 0 && chan_fracMuon[ch+216*2] >= 0 && chan_fracMuon[ch+216*3] >= 0 ){
				h_dT->Fill ( chan_firstTime[ch+216*1] - chan_firstTime[ch] );
			}
		}
	}
	bar.finish();

	f->Write();
	f->Close();
}
