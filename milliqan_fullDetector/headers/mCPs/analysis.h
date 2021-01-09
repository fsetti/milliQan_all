// Make milliqan validation plots. D. Stuart, Oct. 2017.
// adapted F. Setti, Jul. 2019.
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include "TMath.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TProfile.h"
#include "TString.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TSystemDirectory.h"


using namespace std;

const float totLumi 	= 35.2;	// total luminosity in fb^-1

const float tFire 		= 380;  // lowest limit of firing pulse window
const float dtMax 		= 100;    // max time difference btw pulses
const float dtMin 		= 15;     // max time difference btw calibrated times
const float tMin 		= tFire - dtMax;   //smallest time of 100ns window
const float tMax 		= tFire + dtMax;   //largest time of 100ns window
const float hMin 		= 100;
const float maxSPE 		= 5;
const float maxSlabdT 	= 12;
const float nPEminSlab 	= 150;
const float nPEminPanel = 15;
const float nPEminMuon 	= 750;
const float nPEmin 		= 1.;
const float nPEcalib 	= 0.7142857142857143;
const float dt_mCP		= 15.;
