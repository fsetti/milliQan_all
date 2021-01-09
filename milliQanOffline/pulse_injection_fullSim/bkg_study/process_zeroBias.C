#include <iostream>
#include <vector>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#ifndef _INCL_GUARD
#define _INCL_GUARD
#endif
#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"
TChain * inTree;
#include "make_tree.C"
#include "tqdm.h"

vector<Int_t> ETs_chan = { 25 };
//branches
TH1D * waves_arr[numChan];
vector<TH1D*> vec_waves;

void prepareBranches(){
	waves_arr[0] = new TH1D();
	TString num_ch;
	for (unsigned int i=0; i<numChan; i++){
		num_ch = to_string(i);
		outTree->Branch( "waves_"+num_ch, &(waves_arr[i]), 4000 );
	}
}


void process_zeroBias (TString runToMixName="1440", TString tag="", int fileNumber=1, int files_per_output=89, int startEvent=0){

    runNum = atoi(runToMixName);
    sidebandPerFile[runNum] = map<int, vector<float> > ();
    loadSidebandList(Form("/net/cms26/cms26r0/milliqan/haddedTrees/sideband_files/sideband_mean_run_%i.txt",runNum), runNum);
    TString mixFileName;

	inTree = new TChain("Events");
    fileNumbers.clear();
	unsigned int totEventsMix_prev, fileNumber_tmp;
	fileNumber_tmp = fileNumber;
    while ( fileNumber_tmp + files_per_output > fileNumber ){
		mixFileName =  "/net/cms26/cms26r0/milliqan/UX5/Run"+runToMixName+"_software/MilliQan_Run"+runToMixName+"."+to_string(fileNumber)+"_software.root";
		inTree->Add(mixFileName);
		totEventsMix = inTree->GetEntries();
		if ( totEventsMix == totEventsMix_prev ){
			break;
		}
    	fileNumbers.resize(totEventsMix, fileNumber);
		fileNumber++;
		totEventsMix_prev = totEventsMix;
    }
    TString milliqanMixDir="/net/cms26/cms26r0/milliqan/milliqanOffline/mixTrees/zeroBias_fullSim";
    TString outFileName = milliqanMixDir+"/zeroBias_run"+runToMixName+"_"+tag+".root";
    TFile * outFile = new TFile(outFileName,"recreate");
    outTree = new TTree("Events","Events");
    outTree->SetDirectory(outFile);
	prepareBranches();
	int waves_idx = 1;
    loadBranchesMilliDAQ();
	tqdm bar;
    for(int i=startEvent;i<totEventsMix;i++){
		bar.progress(i, totEventsMix);
		inTree->GetEntry(i);
		for (unsigned int ic=0; ic<ETs_chan.size(); ic++){
			unsigned int chan_tmp = ETs_chan[ic];
			this_fileNum = fileNumbers[i-startEvent];
			if(waves[chan_tmp]) delete waves[chan_tmp];
			waves[chan_tmp] = (TH1D*)evt->GetWaveform( chan_tmp / 16 , chan_tmp % 16 , Form("h_%i_%i",i,waves_idx));
			waves[chan_tmp]->Scale(-1.);
			convertXaxis(waves[chan_tmp],chan_tmp);
			// subtract per-file sideband mean
			for(int ibin = 1; ibin <= waves[chan_tmp]->GetNbinsX(); ibin++){
				waves[chan_tmp]->SetBinContent(ibin,waves[chan_tmp]->GetBinContent(ibin)-sidebandPerFile[runNum][this_fileNum][chan_tmp]);
			}
			vec_waves.push_back( (TH1D*) waves[chan_tmp]->Clone() );
			waves_idx++;
		}
		if ( waves_idx >= numChan ){
			random_shuffle( vec_waves.begin() , vec_waves.end() );
			for (unsigned int iChan = 1; iChan < numChan; iChan++){
				waves_arr[iChan] = vec_waves.at(iChan-1);
			}
			vec_waves.erase(vec_waves.begin(), vec_waves.begin() + numChan - 1 );
			outTree->Fill();
			waves_idx = vec_waves.size()+1;
		}
	}
	bar.finish();	
	outFile->cd();
    outTree->Write();
    outFile->Close();
    return;
}

int main(int argc, char **argv)
{
    if(argc==1) process_zeroBias();
    else if(argc==2) process_zeroBias(argv[1]);
    else if(argc==3) process_zeroBias(argv[1],argv[2]);
    else if(argc==4) process_zeroBias(argv[1],argv[2],stoi(argv[3]));
    else if(argc==5) process_zeroBias(argv[1],argv[2],stoi(argv[3]),stoi(argv[4]));
    else if(argc==6) process_zeroBias(argv[1],argv[2],stoi(argv[3]),stoi(argv[4]),stoi(argv[5]));
}
