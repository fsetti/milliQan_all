
#include "SlimTree.h"
#include "TTree.h"
#include "TDirectory.h"
#include <iostream>
#include <vector>

using namespace std;

TString ch_num[nchans];

SlimTree::SlimTree(TTree *t){
	for(unsigned int i=0; i<nchans; i++){
		ch_num[i]	= to_string(i);
	}
    if(t != NULL)
        Init(t);
}

void SlimTree::Init(TTree *t){

    if(t == NULL){
        // if we don't pass a tree, open in "write" mode. vector pointers have to be initialized,
        // and then create the branches.
        this->t = new TTree("Events","");


        b_fileID                 = this->t->Branch("fileID", &fileID, "fileID/I");
        b_orig_evt               = this->t->Branch("orig_evt", &orig_evt, "orig_evt/I");
        b_scale1fb               = this->t->Branch("scale1fb", &scale1fb, "scale1fb/F");
        b_procID                 = this->t->Branch("procID", &procID, "procID/I");
        b_mu_E0                  = this->t->Branch("mu_E0", &mu_E0, "mu_E0/F");
        b_mu_x0                  = this->t->Branch("mu_x0", &mu_x0, "mu_x0/F");
        b_mu_y0                  = this->t->Branch("mu_y0", &mu_y0, "mu_y0/F");
        b_chan_nPE               = this->t->Branch("chan_nPE", chan_nPE, "chan_nPE[8]/I");
        b_chan_fracMuon          = this->t->Branch("chan_fracMuon", chan_fracMuon, "chan_fracMuon[8]/F");
        b_chan_fracElScint       = this->t->Branch("chan_fracElScint", chan_fracElScint, "chan_fracElScint[8]/F");
        b_chan_fracElAl          = this->t->Branch("chan_fracElAl", chan_fracElAl, "chan_fracElAl[8]/F");
        b_chan_fracElRock        = this->t->Branch("chan_fracElRock", chan_fracElRock, "chan_fracElRock[8]/F");
        b_chan_fracElPb          = this->t->Branch("chan_fracElPb", chan_fracElPb, "chan_fracElPb[8]/F");
        b_chan_fracOther         = this->t->Branch("chan_fracOther", chan_fracOther, "chan_fracOther[8]/F");
        b_chan_firstTime         = this->t->Branch("chan_firstTime", chan_firstTime, "chan_firstTime[8]/F");
        b_chan_meanTime          = this->t->Branch("chan_meanTime", chan_meanTime, "chan_meanTime[8]/F");
        b_chan_timeCalib         = this->t->Branch("chan_timeCalib", chan_timeCalib, "chan_timeCalib[8]/F");
        b_chan_muDist            = this->t->Branch("chan_muDist", chan_muDist, "chan_muDist[8]/F");
        b_chan_muTime            = this->t->Branch("chan_muTime", chan_muTime, "chan_muTime[8]/F");
        b_chan_firstPEPathLength = this->t->Branch("chan_firstPEPathLength", chan_firstPEPathLength, "chan_firstPEPathLength[8]/F");
        b_chan_maxSourceType     = this->t->Branch("chan_maxSourceType", chan_maxSourceType, "chan_maxSourceType[8]/I");
        b_chan_maxSourceE0       = this->t->Branch("chan_maxSourceE0", chan_maxSourceE0, "chan_maxSourceE0[8]/F");
        b_chan_maxSourceNPE      = this->t->Branch("chan_maxSourceNPE", chan_maxSourceNPE, "chan_maxSourceNPE[8]/I");
        b_chan_type              = this->t->Branch("chan_type", chan_type, "chan_type[8]/I");
        b_mcTruth_nMuSlabHits    = this->t->Branch("mcTruth_nMuSlabHits", &mcTruth_nMuSlabHits, "mcTruth_nMuSlabHits/I");
        b_mcTruth_nMuPanelHits   = this->t->Branch("mcTruth_nMuPanelHits", &mcTruth_nMuPanelHits, "mcTruth_nMuPanelHits/I");
        b_mcTruth_nMuBarHits     = this->t->Branch("mcTruth_nMuBarHits", &mcTruth_nMuBarHits, "mcTruth_nMuBarHits/I");
        b_mcTruth_fourSlab       = this->t->Branch("mcTruth_fourSlab", &mcTruth_fourSlab, "mcTruth_fourSlab/O");
        b_mcTruth_fourBarLine   = this->t->Branch("mcTruth_fourBarLine", &mcTruth_fourBarLine, "mcTruth_fourBarLine/O");
        b_mcTruth_noBar          = this->t->Branch("mcTruth_noBar", &mcTruth_noBar, "mcTruth_noBar/O");
        b_mcTruth_noChan         = this->t->Branch("mcTruth_noChan", &mcTruth_noChan, "mcTruth_noChan/O");
        b_mcTruth_verticalCosmic = this->t->Branch("mcTruth_verticalCosmic", &mcTruth_verticalCosmic, "mcTruth_verticalCosmic/I");
		for(unsigned int i=0; i<nchans; i++){
			b_chan_PEtimes[i]	= this->t->Branch("chan"+ch_num[i]+"_PEtimes", &chan_PEtimes[i], "chan"+ch_num[i]+"_PEtimes[160]/F");
		}

        Reset();
    }else{
        // if we do pass a tree, open in "read" mode
        this->t = t;
        //this->t->SetMakeClass(1);

        this->t->SetBranchAddress("fileID", &fileID, &b_fileID);
        this->t->SetBranchAddress("orig_evt", &orig_evt, &b_orig_evt);
        this->t->SetBranchAddress("scale1fb", &scale1fb, &b_scale1fb);
        this->t->SetBranchAddress("procID", &procID, &b_procID);
        this->t->SetBranchAddress("mu_E0", &mu_E0, &b_mu_E0);
        this->t->SetBranchAddress("mu_x0", &mu_x0, &b_mu_x0);
        this->t->SetBranchAddress("mu_y0", &mu_y0, &b_mu_y0);
        this->t->SetBranchAddress("chan_nPE", chan_nPE, &b_chan_nPE);
        this->t->SetBranchAddress("chan_fracMuon", chan_fracMuon, &b_chan_fracMuon);
        this->t->SetBranchAddress("chan_fracElScint", chan_fracElScint, &b_chan_fracElScint);
        this->t->SetBranchAddress("chan_fracElAl", chan_fracElAl, &b_chan_fracElAl);
        this->t->SetBranchAddress("chan_fracElRock", chan_fracElRock, &b_chan_fracElRock);
        this->t->SetBranchAddress("chan_fracElPb", chan_fracElPb, &b_chan_fracElPb);
        this->t->SetBranchAddress("chan_fracOther", chan_fracOther, &b_chan_fracOther);
        this->t->SetBranchAddress("chan_firstTime", chan_firstTime, &b_chan_firstTime);
        this->t->SetBranchAddress("chan_meanTime", chan_meanTime, &b_chan_meanTime);
        this->t->SetBranchAddress("chan_timeCalib", chan_timeCalib, &b_chan_timeCalib);
        this->t->SetBranchAddress("chan_muDist", chan_muDist, &b_chan_muDist);
        this->t->SetBranchAddress("chan_muTime", chan_muTime, &b_chan_muTime);
        this->t->SetBranchAddress("chan_firstPEPathLength", chan_firstPEPathLength, &b_chan_firstPEPathLength);
        this->t->SetBranchAddress("chan_maxSourceType", chan_maxSourceType, &b_chan_maxSourceType);
        this->t->SetBranchAddress("chan_maxSourceE0", chan_maxSourceE0, &b_chan_maxSourceE0);
        this->t->SetBranchAddress("chan_maxSourceNPE", chan_maxSourceNPE, &b_chan_maxSourceNPE);
        this->t->SetBranchAddress("chan_type", chan_type, &b_chan_type);
        this->t->SetBranchAddress("mcTruth_nMuSlabHits", &mcTruth_nMuSlabHits, &b_mcTruth_nMuSlabHits);
        this->t->SetBranchAddress("mcTruth_nMuPanelHits", &mcTruth_nMuPanelHits, &b_mcTruth_nMuPanelHits);
        this->t->SetBranchAddress("mcTruth_nMuBarHits", &mcTruth_nMuBarHits, &b_mcTruth_nMuBarHits);
        this->t->SetBranchAddress("mcTruth_fourSlab", &mcTruth_fourSlab, &b_mcTruth_fourSlab);
        this->t->SetBranchAddress("mcTruth_fourBarLine", &mcTruth_fourBarLine, &b_mcTruth_fourBarLine);
        this->t->SetBranchAddress("mcTruth_noBar", &mcTruth_noBar, &b_mcTruth_noBar);
        this->t->SetBranchAddress("mcTruth_noChan", &mcTruth_noChan, &b_mcTruth_noChan);
        this->t->SetBranchAddress("mcTruth_verticalCosmic", &mcTruth_verticalCosmic, &b_mcTruth_verticalCosmic);
		for(unsigned int i=0; i<nchans; i++){
			this->t->SetBranchAddress("chan"+ch_num[i]+"_PEtimes", &chan_PEtimes[i], &b_chan_PEtimes[i]);
		}
    }

    t_old = std::chrono::system_clock::now();
}

void SlimTree::Fill(){
    t->Fill();
}

void SlimTree::Reset(){
    fileID = -1;
    orig_evt = -1;
    scale1fb = 1.0;
    procID = 0;
    mu_E0 = -1;
    mu_x0 = -999;
    mu_y0 = -999;
    mcTruth_nMuSlabHits = -1;
    mcTruth_nMuPanelHits = -1;
    mcTruth_nMuBarHits = -1;
    mcTruth_fourSlab = false;
    mcTruth_fourBarLine = false;
    mcTruth_noBar = false;
    mcTruth_noChan = false;
    mcTruth_verticalCosmic = 0;
    for(int i=0; i<nchans; i++){
        chan_nPE[i] = 0;
        chan_fracMuon[i] = -1;
        chan_fracElScint[i] = -1;
        chan_fracElAl[i] = -1;
        chan_fracElRock[i] = -1;
        chan_fracElPb[i] = -1;
        chan_fracOther[i] = -1;
        chan_firstTime[i] = -999;
        chan_meanTime[i] = -999;
        chan_timeCalib[i] = -999;
        chan_muDist[i] = -1;
        chan_muTime[i] = -1;
        chan_firstPEPathLength[i] = -1;
        chan_maxSourceType[i] = -1;
        chan_maxSourceE0[i] = -1;
        chan_maxSourceNPE[i] = -1;
        chan_type[i] = -1;
    	for(int j=0; j<160; j++){
        	chan_PEtimes[i][j] = 0;
		}
    }
}

void SlimTree::Write(TDirectory *d){
    d->cd();
    t->Write();
}

void SlimTree::GetEntry(ULong64_t i){
    this->t->GetEntry(i);
}

void SlimTree::progress( int curr, int tot, int period, unsigned int smoothing) {
    if(curr%period == 0) {
        auto now = std::chrono::system_clock::now();
        double dt = ((std::chrono::duration<double>)(now - t_old)).count();
        t_old = now;
        // if (deq.size() >= smoothing) deq.pop_front();                                                                                                                                                            
        if (deq.size() >= smoothing) deq.erase(deq.begin());
        deq.push_back(dt);
        double avgdt = std::accumulate(deq.begin(),deq.end(),0.)/deq.size();
        float prate = (float)period/avgdt;
        float peta = (tot-curr)/prate;
        if (isatty(1)) {
            float pct = (float)curr/(tot*0.01);
            if( ( tot - curr ) <= period ) pct = 100.0;
            printf("\015\033[32m ---> \033[1m\033[31m%4.1f%% \033[34m [%.3f kHz, ETA: %.0f s] \033[0m\033[32m  <---\033[0m\015 ", pct, prate/1000.0, peta);
            if( ( tot - curr ) > period ) fflush(stdout);
            else cout << endl;
        }
    }
}
