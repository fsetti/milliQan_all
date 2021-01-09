#define SimTree_cxx
#include "SimTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#ifndef _INCL_GUARD
#define _INCL_GUARD
#endif
#include "TTree.h"
#include "TChain.h"
TChain * inTree = new TChain("Events");
#include "make_tree.C"
#include "tqdm.h"
#include <random>

void mix_events(TString runToMixName="1372", TString inFileName = "exampleInjectionInput.root", TString tag = "example", TString grid_point = "m_0p14_q_0p001", int fileNumber = 1, int startEvent = 0, int maxEvents = -1 );
void clearOutBranchesMix();
void prepareOutBranchesMix();


TBranch * b_timestamp;
double digisatV = -1250.;
double timestamp;

TBranch *         b_chan_muDist;
TBranch *         b_chan_trueNPE;
TBranch *         b_scale1fb;
TBranch *         b_fileID;
TBranch *         b_orig_evt;
TBranch *         b_mcTruth_twoSlab;
TBranch *         b_mcTruth_fourBarLine;

Double_t times[nchans][1024];
TBranch * b_times[nchans];

Double_t voltages[nchans][1024];
TBranch * b_voltages[nchans];

uint maxNPEForSample = 20;

//mt19937 marcello;

void mix_events(TString runToMixName, TString inFileName , TString tag, TString grid_point, int fileNumber, int startEvent, int maxEvents ){

    //vector<double> distanceToBottomSlab;
    double tubeTimeCorr = -11.5 ;

    runNum = atoi(runToMixName);
    sidebandPerFile[runNum] = map<int, vector<float> > ();
    loadSidebandList(Form("/net/cms26/cms26r0/milliqan/haddedTrees/sideband_files/sideband_mean_run_%i.txt",runNum), runNum);

    TFile * inFile = new TFile(inFileName);
    TTree * simTreeInput = (TTree *) inFile->Get("Events");
    uint totEvents = simTreeInput->GetEntries();
    if (totEvents > maxEvents && maxEvents >= 0) totEvents = maxEvents;

    SimTree sTree = SimTree(simTreeInput);
    TString mixFileName;
    //fileNumbers.clear();
	//mixFileName =  "/net/cms26/cms26r0/milliqan/milliqanOffline/mixTrees/zeroBias_fullSim/zeroBias_run"+runToMixName+"_batch*.root";
	mixFileName =  "/data1/milliqan/zeroBias/zeroBias_run"+runToMixName+"_batch*.root";
	inTree->Add(mixFileName);
	totEventsMix = inTree->GetEntries();
	//uniform_int_distribution<unsigned int> mix_rndm_entry(startEvent,totEventsMix);
	//TFile * f = new TFile("/net/cms3/cms3r0/milliqan/Run"+runToMixName+"_software/MilliQan_Run"+runToMixName+"."+to_string(fileNumber)+"_software.root");
	//TFile * f = new TFile("/net/cms26/cms26r0/milliqan/UX5/Run1357_software/MilliQan_Run1357.1_software.root");	//pretty useless info
	TFile * f = new TFile("/data1/milliqan/other_stuff/MilliQan_Run1357.1_software.root");	//pretty useless info

    //TString milliqanMixDir="/net/cms26/cms26r0/milliqan/milliqanOffline/mixTrees/fullSim/"+grid_point;
    TString milliqanMixDir="/data1/milliqan/mixTrees/"+grid_point;
    TString outFileName = milliqanMixDir+"/"+tag+".root";
    TFile * outFile = new TFile(outFileName,"recreate");
    outTree = new TTree("Events","Events");
    outTree->SetDirectory(outFile);
    prepareOutBranchesMix();
    TTree * metadata = (TTree*)f->Get("Metadata");
    metadata->SetBranchAddress("configuration", &cfg);
    metadata->GetEntry(0);
    sample_rate[0] = 3.2 / pow(2,cfg->digitizers[0].SAMFrequency);
    speET[0] = new SPE("et",sample_rate[0],false,-1);		//again, only concerned about ETs
    loadBranchesMilliDAQ_fullSim();
    Long64_t nbytes = 0, nb = 0;
	tqdm bar;
    for(int i=startEvent;i<totEvents;i++){
		bar.progress(i, totEvents);
		TDatime time = TDatime();
		timestamp = time.GetSecond()+60*time.GetMinute()+3600*time.GetHour();
		sTree.LoadTree(i);
		nb = sTree.fChain->GetEntry(i);   nbytes += nb;
		clearOutBranchesMix();
		//inTree->GetEntry(mix_rndm_entry(marcello));		//randomly picking entries slows down the process by a factor of x2, sadly
		inTree->GetEntry( i % totEventsMix );
		TH1D * generatedTemplate = NULL;
		int nPhoton[nchans];
		fileID = sTree.fileID;
		scale1fb = sTree.scale1fb;
		orig_evt = sTree.orig_evt;
		mcTruth_fourBarLine = sTree.mcTruth_fourBarLine;
		mcTruth_twoSlab = sTree.mcTruth_twoSlab;
		for (uint ic = 0; ic < nchans; ic++){
		    chan_muDist[ic] = sTree.chan_muDist[ic];    
		    chan_trueNPE[ic] = sTree.chan_nPE[ic];    
		}
		
		for (uint photonTime = 0; photonTime < 100; photonTime++){
		    for (uint iChan = 0; iChan < nchans; iChan++){
				nPhoton[iChan] = sTree.chan_PEtimes[iChan][photonTime]; 
				if (nPhoton[iChan] == 0) continue;
				if (nPhoton[iChan] < maxNPEForSample){
					for (uint arb = 0; arb < nPhoton[iChan]; arb++){
						generatedTemplate = SPEGen(sample_rate[0],tubeSpecies,channelSPEAreas,0);
						int signalPulsesStartBin = waves[iChan]->FindBin(380+photonTime+tubeTimeCorr);
						int templateStartBin = generatedTemplate->FindBin(-tubeTimeCorr);
						for(int ibin = 1; ibin <= 1024; ibin++){
						    if (ibin+signalPulsesStartBin > waves[iChan]->GetNbinsX()) break;
						    if (ibin < templateStartBin) continue;
						    waves[iChan]->SetBinContent(ibin+signalPulsesStartBin,waves[iChan]->GetBinContent(ibin+signalPulsesStartBin)+generatedTemplate->GetBinContent(ibin));
						}
						generatedTemplate->Delete();
					}
				}
				else{
				    generatedTemplate = SPEGenLargeN(sample_rate[0],tubeSpecies,channelSPEAreas,0,nPhoton[iChan]);
				    int signalPulsesStartBin = waves[iChan]->FindBin(380+photonTime+tubeTimeCorr);
				    int templateStartBin = generatedTemplate->FindBin(-tubeTimeCorr);
				    for(int ibin = 1; ibin <= 1024; ibin++){
						if (ibin+signalPulsesStartBin > waves[iChan]->GetNbinsX()) break;
						if (ibin < templateStartBin) continue;
						waves[iChan]->SetBinContent(ibin+signalPulsesStartBin,waves[iChan]->GetBinContent(ibin+signalPulsesStartBin)+generatedTemplate->GetBinContent(ibin));
				    }
				    generatedTemplate->Delete();
				}
			}
		}
		//sideband correction applied in pre-processing of zero bias runs

		for (uint iChan = 0; iChan < nchans; iChan++){
		    waves[iChan]->Scale(-1.);
		}
		for(int ibin = 1; ibin <= 1024; ibin++){
			for(unsigned int ic=0; ic<nchans; ic++){
		    	times[ic][ibin-1] = waves[ic]->GetBinCenter(ibin);
		    	voltages[ic][ibin-1] = std::min(-digisatV,std::max(digisatV,waves[ic]->GetBinContent(ibin)));
			}
		}
		outTree->Fill();
    }
	bar.finish();
	outFile->cd();
	TNamed testDate = TNamed(TString("date"),TString(tag));
	TParameter<float> sampleRate0 = TParameter<float>("sampleRate0",sample_rate[0]);
	TArrayI * chanArray =  new TArrayI(nchans);
	for(uint ic=0;ic<nchans;ic++){
		chanArray->SetAt(ic,ic);
	}
    sampleRate0.Write();
    testDate.Write();
    outFile->WriteObject(chanArray,TString("chans"));
    outTree->Write();
    outFile->Close();
    return;
}

void clearOutBranchesMix(){
}

void prepareOutBranchesMix(){
    b_timestamp = outTree->Branch("timestamp",&timestamp,"timestamp/D");
    b_chan_muDist = outTree->Branch("chan_muDist",&chan_muDist,"chan_muDist[890]/F");
    b_chan_trueNPE = outTree->Branch("chan_trueNPE",&chan_trueNPE,"chan_trueNPE[890]/I");
    b_scale1fb = outTree->Branch("scale1fb",&scale1fb,"scale1fb/F");
    b_fileID = outTree->Branch("fileID",&fileID,"fileID/I");
    b_orig_evt = outTree->Branch("orig_evt",&orig_evt,"orig_evt/I");
    b_mcTruth_fourBarLine = outTree->Branch("mcTruth_fourBarLine",&mcTruth_fourBarLine,"mcTruth_fourBarLine/O");
    b_mcTruth_twoSlab = outTree->Branch("mcTruth_twoSlab",&mcTruth_twoSlab,"mcTruth_twoSlab/O");
	for(unsigned int ic=0; ic<nchans; ic++){
    	b_times[ic] = outTree->Branch("times_"+ch_num[ic],&times[ic],"times_"+ch_num[ic]+"[1024]/D");
    	b_voltages[ic] = outTree->Branch("voltages_"+ch_num[ic],&voltages[ic],"voltages_"+ch_num[ic]+"[1024]/D");
	}

}
int main(int argc, char **argv)
{
    if(argc==1) mix_events();
    else if(argc==2) mix_events(argv[1]);
    else if(argc==3) mix_events(argv[1],argv[2]);
    else if(argc==4) mix_events(argv[1],argv[2],argv[3]);
    else if(argc==5) mix_events(argv[1],argv[2],argv[3],argv[4]);
    else if(argc==6) mix_events(argv[1],argv[2],argv[3],argv[4],stoi(argv[5]));
    else if(argc==7) mix_events(argv[1],argv[2],argv[3],argv[4],stoi(argv[5]),stoi(argv[6]));
    else if(argc==8) mix_events(argv[1],argv[2],argv[3],argv[4],stoi(argv[5]),stoi(argv[6]),stoi(argv[7]));
}
void SimTree::Loop()
{
    //   In a ROOT session, you can do:
    //      root> .L SimTree.C
    //      root> SimTree t
    //      root> t.GetEntry(12); // Fill t data members with entry number 12
    //      root> t.Show();       // Show values of entry 12
    //      root> t.Show(16);     // Read and show values of entry 16
    //      root> t.Loop();       // Loop on all entries
    //

    //     This is the loop skeleton where:
    //    jentry is the global entry number in the chain
    //    ientry is the entry number in the current Tree
    //  Note that the argument to GetEntry must be:
    //    jentry for TChain::GetEntry
    //    ientry for TTree::GetEntry and TBranch::GetEntry
    //
    //       To read only selected branches, Insert statements like:
    // METHOD1:
    //    fChain->SetBranchStatus("*",0);  // disable all branches
    //    fChain->SetBranchStatus("branchname",1);  // activate branchname
    // METHOD2: replace line
    //    fChain->GetEntry(jentry);       //read all branches
    //by  b_branchname->GetEntry(ientry); //read only this branch
    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();

    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) break;
	nb = fChain->GetEntry(jentry);   nbytes += nb;
	// if (Cut(ientry) < 0) continue;
    }
}
