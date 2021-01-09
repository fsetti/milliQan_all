// adapted F. Setti, Jul. 2019.
#include "../../headers/mCPs/analysis.cc"
#include "../../headers/milliHists.h"
#include "TStyle.h"

void selection( TChain *chain ) 
{

	TH2F *h_dT = new TH2F("dT","delta T", 60 , -50 , 70 , 60 , -50 , 60);

	InitializeChain(chain);

	//Number of events to loop over
	Int_t nentries = (Int_t)chain->GetEntries();
	tqdm bar;	
	for(int ia = 0; ia<nentries; ia++){
	
		chain->GetEntry(ia);
		bar.progress(ia,nentries);

		vector<vector<float>> mCPs;
		mCPs = select_mCPs_debug ( chan,height,ptime,nPE,time_module_calibrated,row,column );
	
		if ( mCPs.size() == 0 ) continue;

		std::sort( mCPs.begin(), mCPs.end(), sort_wrt_channel );
		h_dT->Fill( mCPs.at(3).at(4) - mCPs.at(0).at(4) , mCPs.at(3).at(2) - mCPs.at(0).at(2) , scale1fb );
	
	}
	bar.finish();

	h_dT->Draw("colz");

}
