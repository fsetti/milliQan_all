// adapted F. Setti, Jul. 2019.
#include "../../headers/mCPs/analysis.cc"
#include "../../headers/milliHists.h"
#include "TStyle.h"

void selection( TChain *chain ) 
{

	InitializeChain(chain);

	//Number of events to loop over
	Int_t nentries = (Int_t)chain->GetEntries();
	tqdm bar;	
	float totEvts = 0;
	float totEvts_data = 0;
	float totEvts_check = 0;
	for(int ia = 0; ia<nentries; ia++){
	
		chain->GetEntry(ia);
		bar.progress(ia,nentries);

		if ( mcTruth_fourBarLine ) totEvts += scale1fb;

		vector<vector<float>> mCPs;
		mCPs = select_mCPs_line ( chan,height,ptime,nPE,time_module_calibrated );
	
		if ( mCPs.size() != 0 ) totEvts_data += scale1fb;

		unsigned int nPulses = pulse_check1(chan,height,ptime,nPE,time_module_calibrated );
		if ( nPulses != 0 && nPulses < 5 ) totEvts_check += scale1fb;
		/*
		for (unsigned int i=0; i<mCPs.size(); i++){
			h_nPE->Fill( mCPs.at(i).at(3) , scale1fb );
		}
		*/
	}
	bar.finish();

	cout << "total events w/ mcTruth: " << totEvts * 3000 << endl;	
	cout << "total events w/ reco	: " << totEvts_data * 3000 << endl;	
	cout << "total events w/ check	: " << totEvts_check * 3000 << endl;	

}
