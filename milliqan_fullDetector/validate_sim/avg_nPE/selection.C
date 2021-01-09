// adapted F. Setti, Jul. 2019.
#include "../headers/mCPs/analysis.cc"
#include "../headers/milliHists.h"
#include "TStyle.h"


void selection( TChain *chain, TString output_filename, TString proc ) 
{

	TString ChStr[nChannels];
	for (unsigned int i=0; i<nChannels; i++){
		ChStr[i] = std::to_string(i);
	}
	
	InitializeChain(chain);

	double mCP_nPE_num = 0;	
	double mCP_nPE_den = 0;	
	vector<double> mCP_nPEs;
	vector<double> mCP_nPEs_w;
	//Number of events to loop over
	Int_t nentries = (Int_t)chain->GetEntries();
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//	Main Event Loop 
	for(int ia = 0; ia<nentries; ia++){
	
		chain->GetEntry(ia);

		vector<vector<float>> mCPs;
		mCPs = select_mCPs ( chan,height,ptime,nPE,time_module_calibrated, row, column );
	
		//if ( mCPs.size() != 0 ) cout  << " mCP size: " << mCPs.size() << endl;
		for (unsigned int i=0; i<mCPs.size(); i++){
			mCP_nPE_num	+=  mCPs.at(i).at(3) * scale1fb ;
			mCP_nPE_den	+=  scale1fb ;
			mCP_nPEs.push_back( mCPs.at(i).at(3) );
			mCP_nPEs_w.push_back( scale1fb );
		}
	}
	double nPE_avg = mCP_nPE_num / mCP_nPE_den;
	double nPE_std = 0;
	double nPE_sum_w = 0;
	for(unsigned int i=0; i<mCP_nPEs.size(); i++){
		nPE_sum_w += mCP_nPEs_w.at(i);
		nPE_std	  += mCP_nPEs_w.at(i) * ( mCP_nPEs.at(i) - nPE_avg ) * ( mCP_nPEs.at(i) - nPE_avg ) ;
	}
	nPE_sum_w	*= (double)( mCP_nPEs.size() - 1 ) / mCP_nPEs.size();
	nPE_std = nPE_std / nPE_sum_w;
	nPE_std = TMath::Sqrt( nPE_std );

	cout << " nPE average: " << nPE_avg << endl;
	cout << " nPE std	: " << nPE_std << endl;

	TFile *outFile = new TFile(output_filename.Data(),"update");
	outFile->cd();	
	
	double h_min, h_max;
	h_min	= nPE_avg - 5*nPE_std;
	h_max	= nPE_avg + 5*nPE_std;
	TH1D *h_nPE = new TH1D ("nPE"+proc,"average nPE for mCP", 120, h_min, h_max );

	tqdm bar;	
	for(int ia = 0; ia<nentries; ia++){
	
		chain->GetEntry(ia);
		bar.progress(ia,nentries);

		vector<vector<float>> mCPs;
		mCPs = select_mCPs ( chan,height,ptime,nPE,time_module_calibrated, row, column );
	
		//if ( mCPs.size() != 0 ) cout  << " mCP size: " << mCPs.size() << endl;
		for (unsigned int i=0; i<mCPs.size(); i++){
			h_nPE->Fill( mCPs.at(i).at(3) , scale1fb );
		}
	}
	bar.finish();
	

	outFile->cd();
	outFile->Write();
	outFile->Close();

}
