//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Oct  2 10:33:12 2019 by ROOT version 6.06/01
// from TTree SimTree/
// found on file: exampleInjectionInput.root
//////////////////////////////////////////////////////////

#ifndef SimTree_h
#define SimTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

const int nchans	= 889 + 1;
TString ch_num[nchans];

class SimTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           fileID;
   Int_t           orig_evt;
   Float_t         scale1fb;
   Float_t         mu_E0;
   Float_t         mu_x0;
   Float_t         mu_y0;
   //Float_t         mu_initE;
   Int_t           chan_nPE[nchans];
   Float_t         chan_fracMuon[nchans];
   Float_t         chan_fracElScint[nchans];
   Float_t         chan_fracElAl[nchans];
   Float_t         chan_fracElRock[nchans];
   Float_t         chan_fracElPb[nchans];
   Float_t         chan_fracOther[nchans];
   Float_t         chan_firstTime[nchans];
   Float_t         chan_meanTime[nchans];
   Float_t         chan_timeCalib[nchans];
   Float_t         chan_muDist[nchans];
   Float_t         chan_muTime[nchans];
   Int_t           chan_type[nchans];
   Int_t           mcTruth_nMuSlabHits;
   Int_t           mcTruth_nMuPanelHits;
   Int_t           mcTruth_nMuBarHits;
   Bool_t          mcTruth_twoSlab;
   Bool_t          mcTruth_fourBarLine;
   Bool_t          mcTruth_noBar;
   Bool_t          mcTruth_noChan;
   Int_t           mcTruth_verticalCosmic;
   Float_t         chan_PEtimes[nchans][100];

   // List of branches
   TBranch        *b_fileID;   //!
   TBranch        *b_orig_evt;   //!
   TBranch        *b_scale1fb;   //!
   //TBranch        *b_mu_initE;   //!
   TBranch        *b_chan_nPE;   //!
   TBranch        *b_chan_fracMuon;   //!
   TBranch        *b_chan_fracElScint;   //!
   TBranch        *b_chan_fracElAl;   //!
   TBranch        *b_chan_fracElRock;   //!
   TBranch        *b_chan_fracElPb;   //!
   TBranch        *b_chan_fracOther;   //!
   TBranch        *b_chan_firstTime;   //!
   TBranch        *b_chan_meanTime;   //!
   TBranch        *b_chan_timeCalib;   //!
   TBranch        *b_chan_muDist;   //!
   TBranch        *b_chan_muTime;   //!
   TBranch        *b_chan_type;   //!
   TBranch        *b_mcTruth_nMuSlabHits;   //!
   TBranch        *b_mcTruth_nMuPanelHits;   //!
   TBranch        *b_mcTruth_nMuBarHits;   //!
   TBranch        *b_mcTruth_twoSlab;   //!
   TBranch        *b_mcTruth_fourBarLine;   //!
   TBranch        *b_mcTruth_noBar;   //!
   TBranch        *b_mcTruth_noChan;   //!
   TBranch        *b_mcTruth_verticalCosmic;   //!
   TBranch        *b_chan_PEtimes[nchans];   //!

   SimTree(TTree *tree=0);
   virtual ~SimTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef SimTree_cxx
SimTree::SimTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("exampleInjectionInput.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("exampleInjectionInput.root");
      }
      f->GetObject("Events",tree);

   }
   Init(tree);
}

SimTree::~SimTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t SimTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t SimTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void SimTree::Init(TTree *tree)
{
	for(unsigned int i=0; i<nchans; i++){
		ch_num[i]   = std::to_string(i);
	}

	// The Init() function is called when the selector needs to initialize
	// a new tree or chain. Typically here the branch addresses and branch
	// pointers of the tree will be set.
	// It is normally not necessary to make changes to the generated
	// code, but the routine can be extended by the user if needed.
	// Init() will be called many times when running on PROOF
	// (once per file to be processed).
	
	// Set branch addresses and branch pointers
	if (!tree) return;
	fChain = tree;
	fCurrent = -1;
	fChain->SetMakeClass(1);
	
	fChain->SetBranchAddress("fileID", &fileID, &b_fileID);
	fChain->SetBranchAddress("orig_evt", &orig_evt, &b_orig_evt);
	fChain->SetBranchAddress("scale1fb", &scale1fb, &b_scale1fb);
	//fChain->SetBranchAddress("mu_initE", &mu_initE, &b_mu_initE);
	fChain->SetBranchAddress("chan_nPE", &chan_nPE, &b_chan_nPE);
	fChain->SetBranchAddress("chan_fracMuon", &chan_fracMuon, &b_chan_fracMuon);
	fChain->SetBranchAddress("chan_fracElScint", &chan_fracElScint, &b_chan_fracElScint);
	fChain->SetBranchAddress("chan_fracElAl", &chan_fracElAl, &b_chan_fracElAl);
	fChain->SetBranchAddress("chan_fracElRock", &chan_fracElRock, &b_chan_fracElRock);
	fChain->SetBranchAddress("chan_fracElPb", &chan_fracElPb, &b_chan_fracElPb);
	fChain->SetBranchAddress("chan_fracOther", &chan_fracOther, &b_chan_fracOther);
	fChain->SetBranchAddress("chan_firstTime", &chan_firstTime, &b_chan_firstTime);
	fChain->SetBranchAddress("chan_meanTime", &chan_meanTime, &b_chan_meanTime);
	fChain->SetBranchAddress("chan_timeCalib", &chan_timeCalib, &b_chan_timeCalib);
	fChain->SetBranchAddress("chan_muDist", &chan_muDist, &b_chan_muDist);
	fChain->SetBranchAddress("chan_muTime", &chan_muTime, &b_chan_muTime);
	fChain->SetBranchAddress("chan_type", &chan_type, &b_chan_type);
	fChain->SetBranchAddress("mcTruth_nMuSlabHits", &mcTruth_nMuSlabHits, &b_mcTruth_nMuSlabHits);
	fChain->SetBranchAddress("mcTruth_nMuPanelHits", &mcTruth_nMuPanelHits, &b_mcTruth_nMuPanelHits);
	fChain->SetBranchAddress("mcTruth_nMuBarHits", &mcTruth_nMuBarHits, &b_mcTruth_nMuBarHits);
	fChain->SetBranchAddress("mcTruth_twoSlab", &mcTruth_twoSlab, &b_mcTruth_twoSlab);
	fChain->SetBranchAddress("mcTruth_fourBarLine", &mcTruth_fourBarLine, &b_mcTruth_fourBarLine);
	fChain->SetBranchAddress("mcTruth_noBar", &mcTruth_noBar, &b_mcTruth_noBar);
	fChain->SetBranchAddress("mcTruth_noChan", &mcTruth_noChan, &b_mcTruth_noChan);
	fChain->SetBranchAddress("mcTruth_verticalCosmic", &mcTruth_verticalCosmic, &b_mcTruth_verticalCosmic);
	for(unsigned int i=0; i<nchans; i++){
		fChain->SetBranchAddress("chan"+ch_num[i]+"_PEtimes", &chan_PEtimes[i], &b_chan_PEtimes[i]);
	}
	Notify();
}

Bool_t SimTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void SimTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t SimTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef SimTree_cxx
