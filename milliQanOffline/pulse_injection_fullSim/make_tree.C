#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip> // for setw()
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "TSystem.h"
#include "TRandom2.h"
#include "TROOT.h"
#include "TF1.h"
#include "TMath.h"
#include "TArray.h"
#include "TFile.h"
#include "TVirtualFFT.h"
#include "TTree.h"
#include "TChain.h"
#include "TMath.h"
#include "TH1F.h"
#include "TParameter.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TBranch.h"
#include "TString.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TLatex.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TColor.h"
#include "TComplex.h"
#include "config/GlobalEvent.h"				//edited local version
#include "config/DemonstratorConfiguration.h"	//edited local version
#include "../pmt-calibration/photon_template_generator/SPEGen.h"
#include "tqdm.h"

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream> 
#include <iostream> 

struct stat info1;
struct stat info2;

#ifdef __MAKECINT__
#pragma link C++ class std::vector < std::vector<int> >+;
#pragma link C++ class std::vector < std::vector<float> >+;
#endif

using namespace std;

void scaleXaxis(TH1D *h, double scaling){
    TAxis * a = h->GetXaxis();
    a->Set( a->GetNbins(), a->GetXmin()*scaling, a->GetXmax()*scaling );
    h->ResetStats();
}

TH1D * doFFT(TH1D * hrIn){
    TH1 *hr = 0;
    TVirtualFFT::SetTransform(0);
    hr = hrIn->FFT(hr, "MAG");
    TH1D * hrOut = (TH1D *) hr;
    scaleXaxis(hrOut,1./hrIn->GetXaxis()->GetBinUpEdge(hrIn->GetNbinsX()));
    return hrOut;
}

TH1D * LPFilter(TH1D * hrIn,int butterworthOrder = 2,float filterMin=0.049){
    bool butterworth = butterworthOrder > 0;
    TH1 * hr = 0;
    TF1 *butterworthFilter = new TF1("butterworth", "1/sqrt(1+[0]*(x/[1])^(2*[2]))");
    butterworthFilter->SetParameters(1,filterMin,butterworthOrder);
    TVirtualFFT::SetTransform(0);
    hr = hrIn->FFT(hr, "MAG");
    hr->SetName(TString(hrIn->GetName())+"Transform");
    TVirtualFFT *fft = TVirtualFFT::GetCurrentTransform();
    //Maximum frequency is half of the distribution (+ve and -ve compents sep)
    float inputBinWidth = hrIn->GetXaxis()->GetBinCenter(2)-hrIn->GetXaxis()->GetBinCenter(1);

    Double_t maxFreq = 1./(2*inputBinWidth);
    Double_t freqBinWidth = maxFreq/hrIn->GetNbinsX();

    Int_t n = hrIn->GetNbinsX();

    Double_t *re_full = new Double_t[n];
    Double_t *im_full = new Double_t[n];
    fft->GetPointsComplex(re_full,im_full);

    //Now let's make a backward transform:
    TVirtualFFT *fft_back = TVirtualFFT::FFT(1, &n, "C2R M K");
    fft_back->SetPointsComplex(re_full,im_full);
    TComplex complex_zero = 0.;
    // std::cout << n << std::endl;
    //Desired filter in MHz
    Double_t re,im;
    int stop = round(filterMin/(2*freqBinWidth));
    if (butterworth){
	// Butterworth filter
	for (int kk=0; kk <= n/2; kk++) {
	    float multiplier=butterworthFilter->Eval(kk*2*freqBinWidth);
	    fft->GetPointComplex(kk, re,im);
	    TComplex complex_post_filter =  TComplex(re*multiplier,-im*multiplier);

	    fft_back->SetPointComplex(kk, complex_post_filter);
	    fft_back->SetPointComplex(n-kk, complex_post_filter);

	}
    }
    else{
	// //Brick wall filter
	for (int kk=0; kk <= n; kk++) {
	    fft->GetPointComplex(kk, re,im);
	    if (kk>=stop && kk<=n-stop)
	    {
		fft_back->SetPointComplex(kk, complex_zero);
	    }
	}
    }
    fft_back->Transform();
    TH1 *hb = 0;
    //Let's look at the output
    hb = TH1::TransformHisto(fft_back,hb,"Re");
    hb->Scale(1./n);
    TH1D * hbOut = (TH1D *) hb;
    hb->SetName(TString(hrIn->GetName())+"TransformBack");
    scaleXaxis(hbOut,hrIn->GetXaxis()->GetBinUpEdge(hrIn->GetNbinsX())/hrIn->GetNbinsX());
    //Need this or things get sloooow
    delete hrIn;
    delete hr;
    return hbOut;
}

//////////////////////////////////////////

// Compile like this:
// g++ -o make_tree make_tree.C /net/cms26/cms26r0/milliqan/milliDAQ/libMilliDAQ.so `root-config --cflags --glibs` -Wno-narrowing

//////////////////////////////////////////

//gSystem->Load("/net/cms26/cms26r0/milliqan/MilliDAQ/libMilliDAQ.so");

//Configurable parameters
const int numChan=889 + 1;
TArrayI * chanArray;
int maxEvents=-1; 
map<int, map<int,vector<float> > > sidebandPerFile;
float sideband_range[2] = {0,50}; //in ns
float triggerBand_range[2] = {360,390}; //in ns
float presampleStart= 17.5;
float presampleEnd = 2.5; 
//Measure presample from t0-17.5 to t0-2.5 ns
//
Float_t         chan_muDistDRS[numChan];
Int_t           chan_trueNPEDRS[numChan];
Int_t           fileIDDRS;
Float_t         scale1fbDRS;
Int_t           orig_evtDRS;
Bool_t          mcTruth_twoSlabDRS;
Bool_t          mcTruth_fourBarLineDRS;

float sample_rate[] = {1.6,1.6};

bool debug=false;

//Read output from new format instead of interactiveDAQ
bool milliDAQ=true;

//Activated to display specific events
bool displayMode=false;

TString configurationFolder = "/net/cms26/cms26r0/milliqan/milliqanOffline/configuration/";

TString tubeSpecies = "ET";

// returns global row, column, layer
vector<int> bar_coords(int bar){
    // map to layer 1
   int local_bar = -1;
    if(bar % 216 == 0) local_bar = 216;
    else local_bar = bar % 216;

    // module coordinates
    int module_col = ceil(local_bar*1.0/36)-1;
    if(local_bar%36 == 0) local_bar = 36;
    else local_bar = local_bar % 36;
    int module_row = ceil(local_bar*1.0/4)-1;
   if(local_bar%4 == 0) local_bar = 4;
    else local_bar = local_bar % 4;

    // bar coordinates within module
    vector<int> module_coords = {-1,-1};
    if(local_bar == 1) module_coords = {1,1};
    else if(local_bar == 2) module_coords = {1,0};
    else if(local_bar == 3) module_coords = {0,1};
    else if(local_bar == 4) module_coords = {0,0};

    // global coordiantes in (18, 24, 4) space
    vector<int> coords = {-1,-1,-1,0};
    coords[0] = module_col*2+module_coords[1];
    coords[1] = module_row*2+module_coords[0];
    coords[2] = ceil(bar*1.0/216);

    return coords;
}

vector< vector<int> > chanMap;
void load_chanMap(){
	for (unsigned int i=0; i<numChan; i++){
		//bars
		if ( i <= 864 && i > 0 ){
			/*
			int column, row, layer, type;
			column	=	 ( ( i % 216 -1 ) / 36 )  ;
			row		=	 ( ( i - 1 ) % 36 / 4  )  ;
			layer	= 	 ( i -1 ) / 216;
			type	= 0;
			chanMap.push_back( {column, row, layer, type} );
			*/
			chanMap.push_back( bar_coords( i ) );
		}
		else{
			chanMap.push_back( {-1, -1, -1, -1} );
		}
	}
	//panels
	chanMap.at(873)	=	{0,-2,0,2};
	chanMap.at(874)	=	{3,-2,0,2};
	chanMap.at(875)	=	{1,-2,0,2};
	chanMap.at(876)	=	{2,-2,0,2};
	chanMap.at(877)	=	{0,-2,0,2};	//second pmt for each panel
	chanMap.at(878)	=	{3,-2,0,2};	//second pmt for each panel
	chanMap.at(879)	=	{1,-2,0,2};	//second pmt for each panel
	chanMap.at(880)	=	{2,-2,0,2};	//second pmt for each panel
	chanMap.at(881)	=	{0,-2,1,2};
	chanMap.at(882)	=	{3,-2,1,2};
	chanMap.at(883)	=	{1,-2,1,2};
	chanMap.at(884)	=	{2,-2,1,2};
	//slabs
	chanMap.at(867)	=	{-1,-1,5,1};	//second pmt for each panel
	chanMap.at(868)	=	{-1,-1,0,1};	//second pmt for each panel
	chanMap.at(886)	=	{-1,-1,5,1};	//second pmt for each panel
	chanMap.at(887)	=	{-1,-1,0,1};	//second pmt for each panel
	chanMap.at(888)	=	{-1,-1,5,1};	//second pmt for each panel
	chanMap.at(889)	=	{-1,-1,0,1};	//second pmt for each panel
}
    

TRandom2 * straightPathRandom = new TRandom2();
TRandom2 * nPulseRandom = new TRandom2();
TRandom2 * indexRandom = new TRandom2();
int nPulseQ1 = 50000;

//variables needed in adapting to full detector / mixing stage
vector<int> fileNumbers;
vector<int> mix_ETs = { 9 , 17 , 24 , 25 };
mt19937 mt_event_gen(1729);
unsigned int totEventsMix;
int this_fileNum;			//keep track of which per-file sideband correction to apply to every channel

vector< vector<int> > straightPaths = 
{{0,6,2},
    {1,7,3},
    {24,16,22},
    {25,17,23},
    {8,12,4},
    {9,13,5},
};


vector<float> reds ={255./255.,31./255.,235./255.,111./255.,219./255.,151./255.,185./255.,194./255.,127./255.,98./255.,211./255.,69./255.,220./255.,72./255.,225./255.,145./255.,233./255.,125./255.,147./255.,110./255.,209./255.,44};
vector<float> greens={255./255.,30./255.,205./255.,48./255.,106./255.,206./255.,32./255.,188./255.,128./255.,166./255.,134./255.,120./255.,132./255.,56./255.,161./255.,39./255.,232./255.,23./255.,173./255.,53./255.,45./255.,54};
vector<float> blues={255./255.,30./255.,62./255.,139./255.,41./255.,230./255.,54./255.,130./255.,129./255.,71./255.,178./255.,179./255.,101./255.,150./255.,49./255.,139./255.,87./255.,22./255.,60./255.,21./255.,39./255.,23};

vector<TColor *> palette;
vector<int> colors;


//Declare global variables
vector<TH1D*> waves;
vector<TH1D*> waves_zb;
//TH1D * waves_zb[numChan];
double arrayVoltageDRS[numChan][1024];
//define all calibrations as for channel 25
float meanCalib	= -0.201;
float rmsCalib	= 0.8194;
//float interModuleCalibrations = 26.875;
float interModuleCalibrations = 0;	//set to 0
float channelCalibrations;
float channelSPEAreas =  30.;

#ifndef _INCL_GUARD
TTree * inTree;
#endif
TTree * outTree;

Long64_t initTDC=-1;
Long64_t initSecs=-1;
Long64_t prevTDC=-1;

int nRollOvers=0;

int event = 0;
int fileNum=0;
Long64_t runNum=0;
int runNumOrig=0;
Long64_t event_time_b0=0;
Long64_t event_time_b1=0;
double event_time_fromTDC=0;
bool present_b0;
bool present_b1;
int t_since_fill_start=0;
int t_since_fill_end=0;
int t_until_next_fill=0;
string event_t_string;
Long64_t event_trigger_time_tag_b0;
Long64_t event_trigger_time_tag_b1;
int fillNum;
float fillAvgLumi=0;
float fillTotalLumi=0;
bool beam=false;
bool hardNoBeam=true;
mdaq::GlobalEvent * evt = new mdaq::GlobalEvent(); //for MilliDAQ output only
mdaq::DemonstratorConfiguration * cfg = new mdaq::DemonstratorConfiguration(); 

vector<int> * v_npulses = new vector<int>(); 
vector<int> * v_ipulse = new vector<int>();
vector<int> * v_chan = new vector<int>();
vector<int> * v_layer = new vector<int>();
vector<int> * v_row = new vector<int>();
vector<int> * v_column = new vector<int>();
vector<int> * v_type = new vector<int>();
vector<float> * v_height = new vector<float>();
vector<float> * v_time = new vector<float>();
vector<float> * v_triggerCandidates = new vector<float>();
vector<float> * v_triggerCandidatesEnd = new vector<float>();
vector<int> * v_triggerCandidatesChannel = new vector<int>();
vector<float> * v_time_module_calibrated = new vector<float>();
vector<float> * v_delay = new vector<float>();
vector<float> * v_area = new vector<float>();
vector<float> * v_nPE = new vector<float>();
vector<float> * v_duration =  new vector<float>();
vector<bool> * v_quiet = new vector<bool>();
vector<float> * v_presample_mean = new vector<float>();
vector<float> * v_presample_RMS = new vector<float>();
vector<float> * v_sideband_mean = new vector<float>();
vector<float> * v_sideband_RMS = new vector<float>();
vector<float> * v_sideband_mean_perFile = new vector<float>();
vector<float> * v_triggerBand_mean = new vector<float>();
vector<float> * v_triggerBand_max = new vector<float>();
vector<float> * v_triggerBand_maxTime = new vector<float>();
vector<float> * v_triggerBand_RMS = new vector<float>();
vector<float> * v_sideband_mean_calib = new vector<float>();
vector<float> * v_sideband_RMS_calib = new vector<float>();

vector<Long64_t> * v_groupTDC_b0 = new vector<Long64_t>();
vector<Long64_t> * v_groupTDC_b1 = new vector<Long64_t>();

vector<float> * v_bx = new vector<float>();
vector<float> * v_by = new vector<float>();
vector<float> * v_bz = new vector<float>();
vector<float> * v_max = new vector<float>();
vector<float> * v_min = new vector<float>();
vector<float> * v_max_afterFilter = new vector<float>();
vector<float> * v_max_threeConsec = new vector<float>();
vector<float> * v_triggerThresholds = new vector<float>();
vector<bool> * v_triggerEnable = new vector<bool>();
vector<int> * v_triggerLogic = new vector<int>();
vector<int> * v_triggerMajority = new vector<int>();
vector<float> * v_min_afterFilter = new vector<float>();

Float_t         chan_muDist[numChan];
Int_t           chan_trueNPE[numChan];
Int_t           fileID = -1;
Float_t         scale1fb = -1.;
Int_t           orig_evt = -1;
Bool_t          mcTruth_twoSlab = false;
Bool_t          mcTruth_fourBarLine = false;

bool addTriggerTimes = true;

double timestampSecs = 0;


//Temporary sanity check

TString milliqanOfflineDir="/net/cms26/cms26r0/milliqan/milliqanOffline/";

void make_tree(TString fileName, int eventNum=-1, TString tag="", TString grid_point="", float rangeMinX=-1.,float rangeMaxX=-1.,float rangeMinY=-1000.,float rangeMaxY=-1000., int displayPulseBounds=1, int onlyForceChans=0, int runFFT=0 , int applyLPFilter = 0,int injectPulses=0, float injectSignalQ=-1, int runDRS=0,set<int> forceChan={});
void loadFillList(TString fillFile=milliqanOfflineDir+"processedFillList2018.txt");
vector<TString> getFieldFileList(TString location);
void loadFieldList(TString fieldFile);
void loadSidebandList(TString sidebandFile, int runNum);
void loadPhotonList(TString photonFile);
tuple<int,int,float,float,float, float> findFill(int seconds);
int findField(int seconds);
void loadBranchesMilliDAQ();
void loadBranchesDRS();
void loadWavesMilliDAQ();
void loadWavesDRS();
void loadBranchesInteractiveDAQ();
void prepareOutBranches();
void clearOutBranches();
vector< vector<float> > processChannel(int ic,bool applyLPFilter, bool injectPulses, float injectSignalQ, int runDRS);
void prepareWave(int ic, float &sb_mean, float &sb_RMS, float &tb_mean, float &tb_RMS, float &tb_max, float &tb_maxTime, bool applyLPFilter, bool injectPulses, float injectSignalQ,int runDRS);
vector< vector<float> > findPulses(int ic,bool applyLPFilter, int runDRS);
void findTriggerCandidates(int ic, float sb_mean);
vector< vector<float> > findPulses_inside_out(int ic);
void displayPulse(int ic, float begin, float end, int ipulse);
void displayEvent(vector<vector<vector<float> > > bounds,TString tag,float rangeMinX,float rangeMaxX,float rangeMinY,float rangeMaxY,bool calibrateDisplay, bool displayPulseBounds, bool onlyForceChans, bool runFFT, set<int> forceChan={});
void h1cosmetic(TH1D* hist,int ic);
void getFileCreationTime(const char *path);
vector<int> eventsPrinted(numChan,0);
TString displayDirectory;
void writeVersion(bool);
string GetStdoutFromCommand(string cmd);

pair<float,float> measureSideband(int ic, float start, float end);
pair<float,float> getMaxInRange(int ic, float start, float end);
SPE* speET[2]; 
TRandom2 * areaRandom = new TRandom2();

TH1D * SPEGenLargeN(float sampFreq,TString species, double chanArea,uint iDigi,uint nPulse) {
    TH1D * out;
    double mean,rms,max;
    chanArea *= 1./1.6;
    if (species == "ET"){
	speET[iDigi]->SetChanArea(chanArea);
	out = speET[iDigi]->Generate();
	speET[iDigi]->GetMeanRMSMaxArea(mean,rms,max);
    }
    double randArea = areaRandom->Gaus(mean*nPulse,rms*TMath::Sqrt(1.*nPulse));
    out->Scale(randArea*chanArea/(out->Integral("width")*max));
    return out;
}
TH1D * SPEGen(float sampFreq,TString species, double chanArea,uint iDigi) {
    TH1D * out;
    chanArea *= 1./1.6;
    if (species == "ET"){
	speET[iDigi]->SetChanArea(chanArea);
	out = speET[iDigi]->Generate();
    }
    return out;
}
void defineColors(){
    for(int icolor=0;icolor<reds.size();icolor++){
	palette.push_back(new TColor(2000+icolor,reds[icolor],greens[icolor],blues[icolor]));
	colors.push_back(2000+icolor);
    }
    colors[9] = 419; //kGreen+3;
    colors[2] = 2009;
    //colors[3] = 2013;
    colors[12]= 30;
    colors[0]=28;

}

//Fill list tuple format:
//start time [s], end time [s], fill number, luminosity, average instantaneous lumi
vector<std::tuple<int, int, int, float,float>> fillList; 

//timestamp x1 y1 z1 ... x4 y4 z4
vector<std::tuple<int, float,float,float,float,float,float,float,float,float,float,float,float >> fieldList; 
vector<float> photonList; 

# ifndef __CINT__  // the following code will be invisible for the interpreter
#ifndef _INCL_GUARD
int main(int argc, char **argv)
{
    if(argc==2) 		make_tree(argv[1]);
    else if(argc==3) 	make_tree(argv[1],stoi(argv[2]));
    else if(argc==4) 	make_tree(argv[1],stoi(argv[2]),argv[3]);
    else if(argc==5) 	make_tree(argv[1],stoi(argv[2]),argv[3],argv[4]);
    else if(argc==6) 	make_tree(argv[1],stoi(argv[2]),argv[3],argv[4],stof(argv[5]),stof(argv[6]));
    else if(argc==7) 	make_tree(argv[1],stoi(argv[2]),argv[3],argv[4],stof(argv[5]),stof(argv[6]),stoi(argv[7]));
    else if(argc==8) 	make_tree(argv[1],stoi(argv[2]),argv[3],argv[4],stof(argv[5]),stof(argv[6]),stoi(argv[7]),stoi(argv[8]));
    else if(argc==9) 	make_tree(argv[1],stoi(argv[2]),argv[3],argv[4],stof(argv[5]),stof(argv[6]),stoi(argv[7]),stoi(argv[8]),stoi(argv[9]));
    else if(argc==10) 	make_tree(argv[1],stoi(argv[2]),argv[3],argv[4],stof(argv[5]),stof(argv[6]),stoi(argv[7]),stoi(argv[8]),stoi(argv[9]),stoi(argv[10]));
    else if(argc==11) 	make_tree(argv[1],stoi(argv[2]),argv[3],argv[4],stof(argv[5]),stof(argv[6]),stoi(argv[7]),stoi(argv[8]),stoi(argv[9]),stoi(argv[10]),stoi(argv[11]));
    else if(argc==12) 	make_tree(argv[1],stoi(argv[2]),argv[3],argv[4],stof(argv[5]),stof(argv[6]),stoi(argv[7]),stoi(argv[8]),stoi(argv[9]),stoi(argv[10]),stoi(argv[11]),stoi(argv[12]));
    else if(argc==13) 	make_tree(argv[1],stoi(argv[2]),argv[3],argv[4],stof(argv[5]),stof(argv[6]),stoi(argv[7]),stoi(argv[8]),stoi(argv[9]),stoi(argv[10]),stoi(argv[11]),stoi(argv[12]),stoi(argv[13]));
    else if(argc==14) 	make_tree(argv[1],stoi(argv[2]),argv[3],argv[4],stof(argv[5]),stof(argv[6]),stoi(argv[7]),stoi(argv[8]),stoi(argv[9]),stoi(argv[10]),stoi(argv[11]),stoi(argv[12]),stoi(argv[13]),atof(argv[14]));
    else if(argc==15) 	make_tree(argv[1],stoi(argv[2]),argv[3],argv[4],stof(argv[5]),stof(argv[6]),stoi(argv[7]),stoi(argv[8]),stoi(argv[9]),stoi(argv[10]),stoi(argv[11]),stoi(argv[12]),stoi(argv[13]),atof(argv[14]),stoi(argv[15]));
    else if(argc>=16) {
		//the arguments after index 10 are channels to be forced for the display.
		set<int> forceChans;
		for(int i=15;i<argc;i++){
		    forceChans.insert(stoi(argv[i]));
		}
		make_tree(argv[1],stoi(argv[2]),argv[3],argv[4],stof(argv[5]),stof(argv[6]),stoi(argv[7]),stoi(argv[8]),stoi(argv[9]),stoi(argv[10]),stoi(argv[11]),stoi(argv[12]),stoi(argv[13]),atof(argv[14]),stoi(argv[15]),forceChans);
    }
}
# endif
# endif


void make_tree(TString fileName, int eventNum, TString tag, TString grid_point, float rangeMinX,float rangeMaxX, float rangeMinY,float rangeMaxY, int displayPulseBounds, int onlyForceChans,int runFFT,int applyLPFilter,int injectPulses, float injectSignalQ,int runDRS, set<int> forceChan){

    //	gROOT->ProcessLine( "gErrorIgnoreLevel = kError");
    if (injectPulses) cout << "Running in pulse injection mode!" << endl;
    if (injectSignalQ > 0) cout << "Running in signal injection mode with Q = " << injectSignalQ << "!" << endl;
    if (applyLPFilter) cout << "Applying low pass filter to waveforms!" << endl;
    if (runDRS){
	for (int ic=0;ic<numChan;ic++) {
	    channelCalibrations = 0;
	    // meanCalib[ic] = 0;
	}//+intraModuleCalibrations[i];}
    }
    else{
	for (int ic=0;ic<numChan;ic++) {channelCalibrations = interModuleCalibrations;}//+intraModuleCalibrations[i];}
    }
    bool calibrateDisplay = true;
    defineColors();
    if(eventNum>=0) displayMode=true;
    if(displayMode) cout<<"Display tag is "<<tag<<endl;
    loadFillList();

	load_chanMap();

    TString inFileName = fileName;
    TFile *f = TFile::Open(inFileName, "READ");
    TString date;
    TTree * metadata;
    TString baseFileName= ((TObjString*)inFileName.Tokenize("/")->Last())->String().Data();
    TString runNumber = ((TObjString*)baseFileName.Tokenize(".")->At(0))->String().Data();
    runNumber.ReplaceAll("MilliQan_Run","");

    if(milliDAQ && runDRS <= 0){ 
		inTree = (TChain*)f->Get("Events"); 

		metadata = (TTree*)f->Get("Metadata");
		metadata->SetBranchAddress("configuration", &cfg);
		metadata->GetEntry(0);
		fstream file; 
		runNum = atoi(runNumber.Data());
		file.open(configurationFolder+"Run"+runNum+".txt", ios::out); 
		// Backup streambuffers of  cout 
		streambuf* stream_buffer_cout = cout.rdbuf(); 
		streambuf* stream_buffer_cin = cin.rdbuf(); 

		// Get the streambuffer of the file 
		streambuf* stream_buffer_file = file.rdbuf(); 

		// Redirect cout to file 
		cout.rdbuf(stream_buffer_file); 

		// Redirect cout back to screen 
		cfg->PrintConfiguration();
		cout.rdbuf(stream_buffer_cout); 

		//SAMFrequency 1 -> 1.6 GHz
		//SAMFrequency 2 -> 0.8 GHz
		// actual rate = 3.2 / pow(2,SAMFrequency)

		sample_rate[0] = 3.2 / pow(2,cfg->digitizers[0].SAMFrequency);
		//sample_rate[1] = 3.2 / pow(2,cfg->digitizers[1].SAMFrequency);
		for (int i =0; i < numChan; i++){
		    float triggerThresh = cfg->digitizers[0].channels[i].triggerThreshold;
		    bool triggerEnable = cfg->digitizers[0].channels[i].triggerEnable;
		    int triggerMajority = cfg->digitizers[0].GroupTriggerMajorityLevel;
		    int triggerLogic = cfg->digitizers[0].GroupTriggerLogic;
		    v_triggerThresholds->push_back(triggerThresh);
		    v_triggerEnable->push_back(triggerEnable);
		    v_triggerMajority->push_back(triggerMajority);
		    v_triggerLogic->push_back(triggerLogic);
		}
		cout<<"Sample frequencies, boards 0 and 1: "<<sample_rate[0]<<" GHz and "<<sample_rate[1]<<" GHz."<<endl;
    }
	else if (runDRS > 0){
    	inTree = (TChain*)f->Get("Events"); 
    	TParameter<float> * rate0 = (TParameter<float> *) f->Get("sampleRate0");
    	sample_rate[0] = rate0->GetVal();
    	for (int i =0; i < numChan; i++){
			float triggerThresh = 0.03;
			bool triggerEnable = true;
			int triggerMajority = 2;
			int triggerLogic = 1;
			v_triggerThresholds->push_back(triggerThresh);
			v_triggerEnable->push_back(triggerEnable);
			v_triggerMajority->push_back(triggerMajority);
			v_triggerLogic->push_back(triggerLogic);
    	}
	}
	else {
	    inTree = (TChain*)f->Get("data"); 
	}

	if (runDRS > 0){
	    runNumber = "1";
	}
	speET[0] = new SPE("et",sample_rate[0],false,-1);
	//speET[1] = new SPE("et",sample_rate[1],false,-1);

	runNum=atoi(runNumber.Data());
	runNumOrig = runNum;
	if (injectPulses) runNum *= -1;
	if (injectSignalQ > 0) {
	    runNum *= 1E10;
	    Long64_t injectSignalQ_long = Long64_t(round(injectSignalQ*1E5));
	    runNum += injectSignalQ_long*1000;
    }
    TString signalString;
    if (injectSignalQ > 0){
    signalString += Form("SignalInjected_Q%.6f",injectSignalQ);
    signalString.ReplaceAll(".","p");
    }
    TString fileNumber;
    if (runDRS > 0){
    fileNumber = "0";
    }
    else{
    //Get file number from file name
    fileNumber = ((TObjString*)baseFileName.Tokenize(".")->At(1))->String().Data();
    fileNumber = ((TObjString*)fileNumber.Tokenize("_")->At(0))->String().Data();
    }
    fileNum=atoi(fileNumber.Data());
    TString configName; 
    if (runDRS > 0){
    TNamed * dateNamed = (TNamed *)f->Get("date");
    date = TString(dateNamed->GetTitle());
    configName = "DRS_"+date;
    }
    else{
    //Get config name from filename
    configName = ((TObjString*)baseFileName.Tokenize(".")->At(1))->String().Data();
    configName = ((TObjString*)configName.Tokenize("_")->At(1))->String().Data();
    configName.ReplaceAll(".root","");
    }

    if (runDRS > 0){
    baseFileName="DRS"+baseFileName;
    }
    else{
	baseFileName="UX5"+baseFileName;
    }

    baseFileName.ReplaceAll(".root","");
    baseFileName += signalString;

    //TString version = GetStdoutFromCommand("git describe --tag --abbrev=0"); //fixme- this is currently evaluated at runtime, instead of compile time
    TString version = "shorttagplaceholder";
    if(version.Contains("placeholder")){cout<<"This macro was compiled incorrectly. Please compile this macro using compile.sh"<<endl;return;}

    int offsetSignalQ = 0;

    if (runDRS){
		//if ( grid_point != "" ) milliqanOfflineDir+="DRS_fullSim/"+grid_point+"/";
		if ( grid_point != "" ) milliqanOfflineDir ="/data1/milliqan/DRS_fullSim/"+grid_point+"/";
		else milliqanOfflineDir+="DRS_fullSim/";
    }
    if (applyLPFilter) version += "_filtered";

    //TString treeDirectory= milliqanOfflineDir+"trees_"+version+"/Run"+to_string(runNum)+"_"+configName+signalString+"/";
    TString treeDirectory= milliqanOfflineDir;
    TString linkDirectory;
    if (applyLPFilter) linkDirectory= milliqanOfflineDir+"trees_filtered/Run"+to_string(runNum)+"_"+configName+signalString+"/";
    //else linkDirectory= milliqanOfflineDir+"trees/Run"+to_string(runNum)+"_"+configName+signalString+"/";
    else linkDirectory= milliqanOfflineDir;

    if (injectPulses) baseFileName.ReplaceAll(runNumber,"-"+runNumber);
    if (injectSignalQ > 0) baseFileName.ReplaceAll(runNumber,to_string(runNum));
    TString outFileName = treeDirectory+baseFileName+"_"+version+".root";

    if (injectSignalQ > 0 and !(displayMode)){
		while (stat(outFileName, &info1) == 0){
		    offsetSignalQ += 1;
		    runNum += 1;
		    treeDirectory= milliqanOfflineDir+"trees_"+version+"/Run"+to_string(runNum)+"_"+configName+signalString+"/";
		    if (applyLPFilter) linkDirectory= milliqanOfflineDir+"trees_filtered/Run"+to_string(runNum)+"_"+configName+signalString+"/";
		    else linkDirectory = milliqanOfflineDir+"trees/Run"+to_string(runNum)+"_"+configName+signalString+"/";
		    baseFileName.ReplaceAll(to_string(runNum-1),to_string(runNum));
		    outFileName = treeDirectory+baseFileName+"_"+version+".root";
		}
    }
    cout<<"Run "<<runNum<<", file "<<fileNum<<endl;

    if(maxEvents<0) maxEvents=inTree->GetEntries();
    if(!displayMode) cout<<"Entries: "<<inTree->GetEntries()<<endl;
    //if((int)tubeSpecies.size()!=numChan) cout<<"Tube species map does not match number of channels"<<endl;

    if(runDRS <= 0) {
	chanArray = new TArrayI(numChan);
	for(int ic=0;ic<numChan;ic++){
	    chanArray->SetAt(ic,ic);
	}
    }
    else {
	chanArray = (TArrayI *) f->Get("chans");
	//numChan = chanArray->GetSize();
    }

    if(milliDAQ && runDRS <= 0) loadBranchesMilliDAQ();
    else if(runDRS) loadBranchesDRS();
    else loadBranchesInteractiveDAQ();	

    //gSystem->mkdir(milliqanOfflineDir+"trees_"+version);	
    gSystem->mkdir(treeDirectory);	
    //gSystem->mkdir(linkDirectory);
    TFile * outFile;

    if(!displayMode){
	outFile = new TFile(outFileName,"recreate");
	outTree = new TTree("t","t");
	prepareOutBranches();
	writeVersion(applyLPFilter);
    }


    if (runDRS){
	displayDirectory = milliqanOfflineDir+"displaysDRS/Run"+to_string(runNum)+"_"+configName+signalString+"/";
    }
    else{
	displayDirectory = milliqanOfflineDir+"displays/Run"+to_string(runNum)+"_"+configName+signalString+"/";
    }
    gSystem->mkdir(displayDirectory);




    //if(!displayMode)
    std::vector<float> zeros(numChan,0.);
    sidebandPerFile[runNum] = map<int, vector<float> > ();
    sidebandPerFile[runNum][fileNum] = zeros;
    if (!runDRS){
		loadSidebandList(Form("/net/cms26/cms26r0/milliqan/haddedTrees/sideband_files/sideband_mean_run_%i.txt",runNum), runNum);
    }


    //Load entry 0 in order to get timestamp of first event to find corret field information
    if (!runDRS){
	inTree->GetEntry(0);
	TString fieldFileLocation ="/net/cms26/cms26r0/milliqan/EnvironSensorData";
	vector<TString> fieldFiles = getFieldFileList(fieldFileLocation);
	for(int i=0;i<fieldFiles.size();i++){
	    loadFieldList(fieldFiles[i]);
	}
	loadPhotonList("./ThruGoingSimPhotonTimes.dat");
    }

    if(debug) maxEvents=10;
    cout<<"Starting event loop"<<endl;  
	tqdm bar;
    for(int i=0;i<maxEvents;i++){
	if(displayMode && i!=eventNum) continue; //Find specified event
	bar.progress(i,maxEvents);
	//if(i%200==0) cout<<"Processing event "<<i<<endl;
	inTree->GetEntry(i);
	//cout<<"Got entry "<<i<<endl;
	if(milliDAQ && runDRS <= 0) loadWavesMilliDAQ();
	else if (runDRS) loadWavesDRS();
	//if(!displayMode) 
	clearOutBranches();
	event=i;

	if(milliDAQ){
	    //Waveforms are not inverted yet- done in processChannel
	    for (int iTemp = 0; iTemp < numChan; iTemp++){
			v_max->push_back(-1.*waves[iTemp]->GetMinimum());
			v_min->push_back(-1.*waves[iTemp]->GetMaximum());
	    }

	}

	if (runDRS){
	    event_time_b0 = timestampSecs;
	    event_time_b1 = timestampSecs;
	    event_time_fromTDC = timestampSecs;
	    present_b0 = true;
	    present_b1 = true;
	    event_t_string="";
	    fillAvgLumi=-1; 
	    fillTotalLumi=-1;
	}
	else if(milliDAQ) {
	    if(initSecs<0){ //if timestamps for first event are uninitialized
			if(evt->digitizers[0].DataPresent){ //If this event exists
			    initSecs=evt->digitizers[0].DAQTimeStamp.GetSec();
			    initTDC=evt->digitizers[0].TDC[0];
			    prevTDC=initTDC;
			}
	    }

	    int secs = evt->digitizers[0].DAQTimeStamp.GetSec();
	    //This defines the time in seconds in standard unix epoch since 1970
	    event_time_b0 = secs;

	    Long64_t thisTDC;
	    if(evt->digitizers[0].DataPresent) thisTDC = evt->digitizers[0].TDC[0];
	    else thisTDC=prevTDC;

	    //Check if rollover has happened since last event: if previous time is more than 10 minutes later than current time 
	    //NB events are not written strictly in chronological order
	    Long64_t diff = prevTDC - thisTDC;
	    if(diff > 1.2e+11) nRollOvers++;
	    //For each tDC rollover: add max value: pow(2,40)
	    event_time_fromTDC = 5.0e-9*(thisTDC+nRollOvers*pow(2,40)-initTDC)+initSecs;
	    //update previous TDC holder for next event
	    prevTDC = thisTDC;


	    //in case second digitizer has different time
	    secs = evt->digitizers[1].DAQTimeStamp.GetSec();
	    event_time_b1 = secs;


	    event_trigger_time_tag_b0 = evt->digitizers[0].TriggerTimeTag;
	    event_trigger_time_tag_b1 = evt->digitizers[1].TriggerTimeTag;

	    //event_t_string = evt->digitizers[0].DAQTimeStamp.AsString("s");
	    event_t_string = TTimeStamp(event_time_fromTDC).AsString("s");

	    for(int ig=0; ig<8; ig++){
		v_groupTDC_b0->push_back(evt->digitizers[0].TDC[ig]);
		v_groupTDC_b1->push_back(evt->digitizers[1].TDC[ig]);
	    }	

	    present_b0 = evt->digitizers[0].DataPresent;
	    present_b1 = evt->digitizers[1].DataPresent;

	    fillNum=0;
	    t_since_fill_start= -1;
	    t_since_fill_end= -1;
	    t_until_next_fill= -1;
	    fillAvgLumi=-1;
	    fillTotalLumi=-1;
	    secs = round(event_time_fromTDC);
	    tuple<int,int,float,float,float, float> fillInfo = findFill(secs);
	    fillNum= get<0>(fillInfo);
	    t_since_fill_start= get<1>(fillInfo);
	    fillAvgLumi = get<2>(fillInfo);
	    fillTotalLumi = get<3>(fillInfo);
	    t_until_next_fill = get<4>(fillInfo);
	    t_since_fill_end = get<5>(fillInfo);

	    if(fillNum>0) beam=true;
	    else beam = false;

	    if (t_until_next_fill > 3600 && t_since_fill_end > 600) hardNoBeam = true;
	    else hardNoBeam = false;

	    // cout<<"This secs "<<secs<<endl;
	    int fieldPoint = findField(secs);
	    //cout<<"This field point "<<fieldPoint<<endl;
	    if(fieldPoint>=0){
			v_bx->push_back(get<1>(fieldList[fieldPoint]));
			v_bx->push_back(get<4>(fieldList[fieldPoint]));
			v_bx->push_back(get<7>(fieldList[fieldPoint]));
			v_bx->push_back(get<10>(fieldList[fieldPoint]));

			v_by->push_back(get<2>(fieldList[fieldPoint]));
			v_by->push_back(get<5>(fieldList[fieldPoint]));
			v_by->push_back(get<8>(fieldList[fieldPoint]));
			v_by->push_back(get<11>(fieldList[fieldPoint]));

			v_bz->push_back(get<3>(fieldList[fieldPoint]));
			v_bz->push_back(get<6>(fieldList[fieldPoint]));
			v_bz->push_back(get<9>(fieldList[fieldPoint]));
			v_bz->push_back(get<12>(fieldList[fieldPoint]));
	    }
	    else{
			v_bx->push_back(-50);
			v_bx->push_back(-50);
			v_bx->push_back(-50);
			v_bx->push_back(-50);

			v_by->push_back(-50);
			v_by->push_back(-50);
			v_by->push_back(-50);
			v_by->push_back(-50);

			v_bz->push_back(-50);
			v_bz->push_back(-50);
			v_bz->push_back(-50);
			v_bz->push_back(-50);
	    }
	    //get<2>(fillList[index_of_first_fill_with_larger_start_time-1])
	}
	else{ event_time_b0=0;event_time_b1=0;event_t_string="";fillNum=0;beam=false;hardNoBeam=false;fillAvgLumi=-1; fillTotalLumi=-1;}

	vector<vector<vector<float> > > allPulseBounds;
	straightPathRandom->SetSeed(0);
	nPulseRandom->SetSeed(0);
	indexRandom->SetSeed(0);
	int straightPathIndex = straightPathRandom->Integer(6);
	std::vector<int> straightPath = straightPaths[straightPathIndex];
	fileID = fileIDDRS;
	scale1fb = scale1fbDRS;
	orig_evt = orig_evtDRS;
	mcTruth_twoSlab = mcTruth_twoSlabDRS;
	mcTruth_fourBarLine = mcTruth_fourBarLineDRS;
	for(int ic=0;ic<numChan;ic++){
	    if (runDRS){
		chan_muDist[ic] = chan_muDistDRS[ic];
		chan_trueNPE[ic] = chan_trueNPEDRS[ic];
	    }

	    /* if(ic==15){//skip timing card channel
	       vector<vector<float> > empty;
	       allPulseBounds.push_back(empty);
	       continue;
	       }*/
	    //	cout<<Form("Chan %i min: ",ic)<<waves[ic]->GetMinimum()<<endl;
	    if (injectSignalQ < 0 || std::find(straightPath.begin(), straightPath.end(), ic) == straightPath.end()) allPulseBounds.push_back(processChannel(ic,applyLPFilter,injectPulses,-1,runDRS));
	    else allPulseBounds.push_back(processChannel(ic,applyLPFilter,injectPulses,injectSignalQ,runDRS));
	}
	if(displayMode){
	    displayEvent(allPulseBounds,tag,rangeMinX,rangeMaxX,rangeMinY,rangeMaxY,calibrateDisplay,displayPulseBounds,onlyForceChans,runFFT,forceChan);
	}
	else outTree->Fill();
    }
	bar.finish();

    if(!displayMode) cout<<"Processed "<<maxEvents<<" events."<<endl;

    if(!displayMode){
        outTree->Write();
        outFile->Close();
        
        cout<<"Closed output tree."<<endl;
		//not sure why we need a link
		/*
        //TString currentDir=gSystem->pwd();
        //TString target = currentDir+"/"+outFileName;
        TString target = outFileName;
        TString linkname =linkDirectory+baseFileName+".root";
        remove(linkname); //remove if already exists
        gSystem->Symlink(target,linkname);
        cout<<"Made link to "<<target<<" called "<<linkname<<endl;
		*/
    }
}


void convertXaxis(TH1D *h, int ic){
    TAxis * a = h->GetXaxis();
    a->Set( a->GetNbins(), a->GetXmin()/sample_rate[0], a->GetXmax()/sample_rate[0] );

    h->ResetStats();
}
void prepareWave(int ic, float &sb_meanPerEvent, float &sb_RMSPerEvent, float &sb_triggerMeanPerEvent, float &sb_triggerRMSPerEvent,
	float &sb_triggerMaxPerEvent,float &sb_timeTriggerMaxPerEvent,bool applyLPFilter, bool injectPulses, float injectSignalQ, int runDRS){
    //Invert waveform and convert x-axis to ns

    waves[ic]->Scale(-1.0);
    if (runDRS <= 0){
		convertXaxis(waves[ic],ic);
    }

    //subtract calibrated mean
	for(int ibin = 1; ibin <= waves[ic]->GetNbinsX(); ibin++){
		waves[ic]->SetBinContent(ibin,waves[ic]->GetBinContent(ibin)-meanCalib);
	}
    //subtract per file mean
	for(int ibin = 1; ibin <= waves[ic]->GetNbinsX(); ibin++){
		waves[ic]->SetBinContent(ibin,waves[ic]->GetBinContent(ibin)-sidebandPerFile[runNum][fileNum][25]);
	}
    if (injectPulses){
	int injectPulsesStartBin = waves[ic]->FindBin(200.-channelCalibrations);
	TH1D * generatedTemplate = SPEGen(sample_rate[0],tubeSpecies,channelSPEAreas,0);

	for(int ibin = 1; ibin <= generatedTemplate->GetNbinsX(); ibin++){
	    waves[ic]->SetBinContent(ibin+injectPulsesStartBin,waves[ic]->GetBinContent(ibin+injectPulsesStartBin)+generatedTemplate->GetBinContent(ibin));
	    if (ibin+injectPulsesStartBin > waves[ic]->GetNbinsX()) break;
	}
	delete generatedTemplate;
    }
    if (injectSignalQ > 0){
	int totalN = nPulseRandom->Poisson(injectSignalQ*injectSignalQ*nPulseQ1);
	for (int iSig = 0; iSig < totalN; iSig++){
	    int index = indexRandom->Integer(photonList.size());
	    float photonTimeCalib = photonList[index];
	    TH1D * generatedTemplate = SPEGen(sample_rate[0],tubeSpecies,channelSPEAreas, 0);
	    int signalPulsesStartBin = waves[ic]->FindBin(380+photonTimeCalib-channelCalibrations);
	    for(int ibin = 1; ibin <= generatedTemplate->GetNbinsX(); ibin++){
		waves[ic]->SetBinContent(ibin+signalPulsesStartBin,waves[ic]->GetBinContent(ibin+signalPulsesStartBin)+generatedTemplate->GetBinContent(ibin));
		if (ibin+signalPulsesStartBin > waves[ic]->GetNbinsX()) break;
	    }
	}

    }
    if (applyLPFilter){
	int butterworthOrder = -1;
	// if (tubeSpecies[ic] == "R878" || tubeSpecies[ic] == "R7725"){
	//     butterworthOrder = 2;
	// }
	// else{
	//     butterworthOrder = 1;
	// }
	butterworthOrder = 2;
	waves[ic] = LPFilter(waves[ic],butterworthOrder);
	// waves[ic] = LPFilter(waves[ic],4,0.099);
    }

    //Measure event by event mean and RMS from the sideband
    pair<float,float> mean_rms = measureSideband(ic,sideband_range[0],sideband_range[1]);
    sb_meanPerEvent = mean_rms.first;
    sb_RMSPerEvent = mean_rms.second;

    //Measure event by event mean and RMS from the sideband
    pair<float,float> mean_rms_trigger = measureSideband(ic,triggerBand_range[0],triggerBand_range[1]);
    sb_triggerMeanPerEvent = mean_rms_trigger.first;
    sb_triggerRMSPerEvent = mean_rms_trigger.second;

    //Measure event by event mean and RMS from the sideband
    pair<float,float> max_timeMax_trigger = getMaxInRange(ic,triggerBand_range[0],triggerBand_range[1]);
    sb_triggerMaxPerEvent = max_timeMax_trigger.first;
    sb_timeTriggerMaxPerEvent = max_timeMax_trigger.second;

    // Subtract dynamically measured pedestal for channel 30
    /*
    if (ic == 30){
		for(int ibin = 1; ibin <= waves[ic]->GetNbinsX(); ibin++){
		    waves[ic]->SetBinContent(ibin,waves[ic]->GetBinContent(ibin)-mean_rms.first);
		}
    }
	*/
}

//Measure mean and RMS of samples in range from start to end (in ns)
pair<float,float> getMaxInRange(int ic, float start, float end){

    float sum_sb=0.;
    float sum2_sb=0.;
    int startbin = waves[ic]->FindBin(start);
    int endbin = waves[ic]->FindBin(end);
    float maxInRange = -9999;
    float timeMaxInRange = -1;
    for(int ibin=startbin; ibin <= endbin; ibin++){
	if (waves[ic]->GetBinContent(ibin) > maxInRange){
	    maxInRange = waves[ic]->GetBinContent(ibin);
	    timeMaxInRange = waves[ic]->GetBinLowEdge(ibin);
	}
    }
    return make_pair(maxInRange,timeMaxInRange);

}
//Measure mean and RMS of samples in range from start to end (in ns)
pair<float,float> measureSideband(int ic, float start, float end){

    float sum_sb=0.;
    float sum2_sb=0.;
    int startbin = waves[ic]->FindBin(start);
    int endbin = waves[ic]->FindBin(end);
    int n_sb = 0;
    for(int ibin=startbin; ibin <= endbin; ibin++){
	sum_sb = sum_sb + waves[ic]->GetBinContent(ibin);
	sum2_sb = sum2_sb + pow(waves[ic]->GetBinContent(ibin),2);
	n_sb++;
    }
    if(n_sb == 0) n_sb = 1.;
    float mean = sum_sb/n_sb;
    float RMS =pow( sum2_sb/n_sb - pow(mean,2), 0.5);

    return make_pair(mean,RMS);

}


vector< vector<float> > processChannel(int ic,bool applyLPFilter, bool injectPulses, float injectSignalQ, int runDRS){
    float sb_meanPerEvent, sb_RMSPerEvent, sb_triggerMeanPerEvent, sb_triggerRMSPerEvent,  sb_triggerMaxPerEvent, sb_timeTriggerMaxPerEvent;
    prepareWave(ic, sb_meanPerEvent, sb_RMSPerEvent,sb_triggerMeanPerEvent, sb_triggerRMSPerEvent, sb_triggerMaxPerEvent, sb_timeTriggerMaxPerEvent,applyLPFilter, injectPulses, injectSignalQ,runDRS); 
    float sb_mean = meanCalib;
    float sb_RMS = rmsCalib;

    vector<vector<float> > pulseBounds;
    //if(tubeSpecies[ic]!="ET") 
    pulseBounds = findPulses(ic,applyLPFilter, runDRS);
    if (addTriggerTimes) findTriggerCandidates(ic,sb_mean);
    //else pulseBounds = findPulses_inside_out(ic); //Use inside-out method for narrow ET pulses

    int npulses = pulseBounds.size();
    //float channelCalibrations[] = {0.,0.,-2.0,-7.5,0.5,0.,0.88,12.58,1.22,0.,-6.51,-4.75,1.2,0.,25.7,6.8};
    //Replaced after pulse finding modification postTS1   float channelSPEAreas[] = {60.,81.5,64.5,48.7,55.2,84.8,57.0,57.2,60.,181.6,576.6,60.,77.5,60.,52.6,50.4};
    //                          0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15 16  17 18   19  20  21  22   23  24  25  26  27  28  29  30  31
    // Archive the values used before low pass filtering   float channelSPEAreas[] = {65.,70.,85.,70.,73.,83.,75.,80.,100.,65.,90.,80.,73.,95.,75.,1.,65.,45.,95.,85.,68.,90.,100.,58.,48.,33.,78.,75.,80.,62.,68.,70.};
    //NB: v8 march13- currently approximate guess (60 nVs) for unmeasured ch0,ch8,ch11,ch13

    v_sideband_mean->push_back(sb_meanPerEvent);
    v_sideband_RMS->push_back(sb_RMSPerEvent);	
    v_triggerBand_mean->push_back(sb_triggerMeanPerEvent);
    v_triggerBand_RMS->push_back(sb_triggerRMSPerEvent);	
    v_triggerBand_max->push_back(sb_triggerMaxPerEvent);
    v_triggerBand_maxTime->push_back(sb_timeTriggerMaxPerEvent);	
    v_sideband_mean_calib->push_back(sb_mean);
    v_sideband_RMS_calib->push_back(sb_RMS);	
    v_sideband_mean_perFile->push_back(sidebandPerFile[runNum][fileNum][25]);
    float maxThreeConsec = -100;
    for (int iBin = 1; iBin < waves[ic]->GetNbinsX(); iBin++){
	float maxList[] = {waves[ic]->GetBinContent(iBin),waves[ic]->GetBinContent(iBin+1),waves[ic]->GetBinContent(iBin+2)};
	float tempMax = *std::min_element(maxList,maxList+3);
	if (maxThreeConsec < tempMax) maxThreeConsec = tempMax;

    }
    v_max_threeConsec->push_back(maxThreeConsec);
    v_max_afterFilter->push_back(waves[ic]->GetMaximum());
    v_min_afterFilter->push_back(waves[ic]->GetMinimum());



    for(int ipulse = 0; ipulse<npulses; ipulse++){
	//Set waveform range to this pulse
	waves[ic]->SetAxisRange(pulseBounds[ipulse][0],pulseBounds[ipulse][1]);
	if(debug) cout<<"Chan "<<ic<<", pulse bounds: "<<pulseBounds[ipulse][0]<<" to "<<pulseBounds[ipulse][1]<<endl;
	//Fill branches


	v_chan->push_back(chanArray->GetAt(ic));
	//chanMap: col,row,layer,type
    v_column->push_back(chanMap[ic][0]);
    v_row->push_back(chanMap[ic][1]);
    v_layer->push_back(chanMap[ic][2]);
    v_type->push_back(chanMap[ic][3]);

	v_height->push_back(waves[ic]->GetMaximum());
	v_time->push_back(pulseBounds[ipulse][0]);
	v_time_module_calibrated->push_back(pulseBounds[ipulse][0]+channelCalibrations);
	v_area->push_back(waves[ic]->Integral());
	v_nPE->push_back((waves[ic]->Integral()/(channelSPEAreas))*(1.6/sample_rate[0]));
	v_ipulse->push_back(ipulse);
	v_npulses->push_back(npulses);
	v_duration->push_back(pulseBounds[ipulse][1] - pulseBounds[ipulse][0]);
	if(ipulse>0) v_delay->push_back(pulseBounds[ipulse][0] - pulseBounds[ipulse-1][1]); //interval between end of previous pulse and start of this one
	else v_delay->push_back(1999.);

	//get presample info
	pair<float,float> presampleInfo = measureSideband(ic,pulseBounds[ipulse][0]-presampleStart,pulseBounds[ipulse][0]-presampleEnd);
	v_presample_mean->push_back(presampleInfo.first);
	v_presample_RMS->push_back(presampleInfo.second);	
	bool quiet = (fabs(presampleInfo.first)<1. && presampleInfo.second <2.0 )&& (pulseBounds[ipulse][1] < waves[ic]->GetBinLowEdge(waves[ic]->GetNbinsX())-0.01) ;
	v_quiet->push_back(quiet); //preliminary: mean between -1 and 1, and RMS<2

	//if(event<0 || eventsPrinted[ic]<0) displayPulse(ic,pulseBounds[ipulse][0],pulseBounds[ipulse][1],ipulse);
    }
    //if(event<0 || (event<0 && npulses>0) || eventsPrinted[ic]<0) {displayEvent(ic,pulseBounds); eventsPrinted[ic]++;}
    return pulseBounds;
}

void displayPulse(int ic, float begin, float end, int ipulse){
    //This must be called inside the pulse loop, after branches are filled,
    // since it displays the last value of each branch as a sanity check.

    TCanvas c;
    gStyle->SetGridStyle(3);
    gStyle->SetGridColor(13);
    c.SetGrid();

    waves[ic]->SetAxisRange(begin-130,end+130);
    waves[ic]->SetLineWidth(2);
    waves[ic]->SetTitle(Form("Zoom on event %i, channel %i, pulse %i (%s);Time [ns];Amplitude [mV];",event,ic,ipulse,tubeSpecies.Data()));
    h1cosmetic(waves[ic],ic);	
    waves[ic]->Draw("hist");



    //Show boundaries of pulse
    TLine line; line.SetLineColor(1); line.SetLineWidth(4); line.SetLineStyle(3);	
    line.DrawLine(begin,0,begin,0.4*waves[ic]->GetMaximum());
    line.DrawLine(end,0,end,0.4*waves[ic]->GetMaximum());

    //Display values stored for this pulse
    TLatex tla;
    tla.SetTextSize(0.045);
    tla.SetTextFont(42);
    tla.DrawLatexNDC(0.13,0.83,Form("Height: %0.2f mV",v_height->back()));
    tla.DrawLatexNDC(0.13,0.78,Form("Area: %0.2f nVs",v_area->back()));
    tla.DrawLatexNDC(0.13,0.73,Form("Duration: %0.2f ns",v_duration->back()));

    c.SaveAs(displayDirectory+Form("Event%i_Chan%i_begin%0.0f.pdf",event,ic,begin));
}

void displayEvent(vector<vector<vector<float> > > bounds, TString tag,float rangeMinX,float rangeMaxX,float rangeMinY,float rangeMaxY,bool calibrateDisplay, bool displayPulseBounds,bool onlyForceChans,bool runFFT, set<int> forceChan){
    if (runFFT) displayPulseBounds = false;
    TCanvas c("c1","",1400,800);
    gPad->SetRightMargin(0.39);
    gStyle->SetGridStyle(3);
    gStyle->SetGridColor(13);
    c.SetGrid();

    float drawThresh=5;

    gStyle->SetTitleX(0.35);
    vector<int> chanList;
    float maxheight=0;
    float timeRange[2];
    timeRange[1]=1024./min(sample_rate[0],sample_rate[1]); timeRange[0]=0.;
    if (runFFT) {timeRange[1]=max(sample_rate[0],sample_rate[1])/2.; timeRange[0]=0.;}
    vector<vector<vector<float>>> boundsShifted;
    vector<TH1D*> wavesShifted;
    vector<TH1D*> wavesShiftedTemp;
    float originalMaxHeights[32];
    for(uint ic=0;ic<bounds.size();ic++){
	int chan = chanArray->GetAt(ic);
	vector<vector<float>> boundShifted = bounds[ic];
	TH1D * waveShifted = (TH1D*) waves[ic]->Clone();
	if(calibrateDisplay){
	    waveShifted->Reset();

	    for (uint iBin = 1;iBin <= waves[ic]->GetNbinsX();iBin++)
	    {
		float binLowEdgeShifted = waves[ic]->GetBinLowEdge(iBin) + channelCalibrations;
		int iBinShifted = waveShifted->FindBin(binLowEdgeShifted + 1E-4);
		if (iBinShifted > 0 && iBinShifted <= waves[ic]->GetNbinsX()){
		    waveShifted->SetBinContent(iBinShifted,waves[ic]->GetBinContent(iBin));
		    waveShifted->SetBinError(iBinShifted,waves[ic]->GetBinError(iBin));
		}

	    }
	    for(uint iBoundVec =0;iBoundVec < bounds[ic].size();iBoundVec++){
		for(uint iBoundVec2 =0;iBoundVec2 < bounds[ic][iBoundVec].size();iBoundVec2++){
		    boundShifted[iBoundVec][iBoundVec2] += channelCalibrations; 
		}
	    }
	}
	if(boundShifted.size()>0 || waveShifted->GetMaximum()>drawThresh || forceChan.find(chan)!=forceChan.end()){
	    //if(ic==15 && forceChan.find(ic)==forceChan.end()) continue;
	    if (!(onlyForceChans && forceChan.find(chan)==forceChan.end())){
		chanList.push_back(ic);
		TString beamState = "off";
		if(beam) beamState="on";
		if (calibrateDisplay) waveShifted->SetTitle(Form("Run %i, File %i, Event %i (beam %s);Time [ns];Amplitude [mV];",runNum,fileNum,event,beamState.Data()));
		else waveShifted->SetTitle(Form("Run %i, File %i, Event %i (beam %s);Uncalibrated Time [ns];Amplitude [mV];",runNum,fileNum,event,beamState.Data()));
		if(ic!=-15){ 
		    //Reset range to find correct maxima
		    waveShifted->SetAxisRange(0,1024./sample_rate[ic/16]);
		    //Keep track of max amplitude
		    if(waveShifted->GetMaximum()>maxheight) maxheight=waveShifted->GetMaximum();
		    if(boundShifted.size()>0){
			//keep track of earliest pulse start time
			if(boundShifted[0][0]<timeRange[0]) timeRange[0]=boundShifted[0][0];
			//keep track of latest pulse end time (pulses are ordered chronologicaly for each channel)
			if(boundShifted[boundShifted.size()-1][1]>timeRange[1]) timeRange[1]=boundShifted[boundShifted.size()-1][1];
		    }
		}
	    }
	}
	if (runFFT) wavesShiftedTemp.push_back(waveShifted);
	else {
	    // TH1D * waveShiftedFiltered = LPFilter(waveShifted);
	    // wavesShifted.push_back(waveShiftedFiltered);
	    wavesShifted.push_back(waveShifted);
	}
	boundsShifted.push_back(boundShifted);
    }
    int maxheightbin = -1;
    if (runFFT){
	maxheight = -999;
	boundsShifted = bounds;
	for(uint ic=0;ic<bounds.size();ic++){
	    // if (std::find(chanList.begin(), chanList.end(), ic) == chanList.end()) continue;
	    int chan = chanArray->GetAt(ic);
	    float binSize = wavesShiftedTemp[ic]->GetBinCenter(2)-wavesShiftedTemp[ic]->GetBinCenter(1);
	    float totalRange = wavesShiftedTemp[ic]->GetXaxis()->GetBinUpEdge(wavesShiftedTemp[ic]->GetNbinsX());
	    TString name; name.Form("%d",ic); 
	    TH1D * hist_transformT = doFFT(wavesShiftedTemp[ic]);
	    hist_transformT->SetName(name+"Temp");
	    // scaleXaxis(hist_transformT,1./totalRange);
	    TH1D * hist_transform = new TH1D (name,"",int(hist_transformT->GetNbinsX()/2),0,hist_transformT->GetXaxis()->GetBinUpEdge(hist_transformT->GetNbinsX()));
	    for (uint iBin = 1; iBin <= hist_transform->GetNbinsX(); iBin ++){
		hist_transform->SetBinContent(iBin,TMath::Sqrt(hist_transformT->GetBinContent(iBin)));
	    }
	    wavesShifted.push_back(hist_transformT);
	    if (!(onlyForceChans && forceChan.find(chan)==forceChan.end())){
		wavesShifted[ic]->GetXaxis()->SetRange(2,wavesShifted[ic]->GetNbinsX()-1);
		if(wavesShifted[ic]->GetMaximum()>maxheight) maxheight=wavesShifted[ic]->GetMaximum();
		if(wavesShifted[ic]->GetMaximum()>maxheight) maxheightbin=wavesShifted[ic]->GetMaximumBin();
		wavesShifted[ic]->GetXaxis()->SetRange(1,wavesShifted[ic]->GetNbinsX());
	    }
	    wavesShifted[ic]->SetTitle(wavesShiftedTemp[ic]->GetTitle());
	}
    }
    maxheight*=1.1;

    //maxheight=30;
    if (rangeMaxY > -999) maxheight = rangeMaxY;
    if (rangeMinX < 0) timeRange[0]*=0.9;
    else timeRange[0] = rangeMinX;
    if (runFFT){
	if (rangeMaxX < 0) timeRange[1]= max(sample_rate[0],sample_rate[1])/2.;
	else timeRange[1] = rangeMaxX;
    }
    else {
	if (rangeMaxX < 0) timeRange[1]= min(1.1*timeRange[1],1024./min(sample_rate[0],sample_rate[1]));
	else timeRange[1] = rangeMaxX;
    }

    float depth = 0.075*chanList.size();
    TLegend leg(0.45,0.9-depth,0.65,0.9);
    for(uint i=0;i<chanList.size();i++){
	int ic = chanList[i];	
	int chan = chanArray->GetAt(ic);
	if (onlyForceChans && forceChan.find(chan)==forceChan.end()) continue;
	if(ic==-15 && forceChan.find(chan)==forceChan.end()) continue;
	wavesShifted[ic]->SetAxisRange(timeRange[0],timeRange[1]);
	originalMaxHeights[ic] = wavesShifted[ic]->GetMaximum();
	if (rangeMinY > -999) wavesShifted[ic]->SetMinimum(rangeMinY);
	wavesShifted[ic]->SetMaximum(maxheight);
	int column= chanMap[ic][0];
	int row= chanMap[ic][1];
	int layer= chanMap[ic][2];
	int type= chanMap[ic][3];
	int colorIndex = 4-2*(row-1)+column-1+6*(layer-1);

	if(type==1) colorIndex = layer; //slabs: 0-3
	if(type==2) colorIndex = 4 + 3*(layer-1) + (column+1); //sheets
	if(ic==-15) colorIndex=1;
	//if(type!=0) continue;
	h1cosmetic(wavesShifted[ic],colorIndex);
	if(type==1) wavesShifted[ic]->SetLineStyle(3);
	if(type==2) wavesShifted[ic]->SetLineStyle(7);
	if(i==0) wavesShifted[ic]->Draw("hist");
	else wavesShifted[ic]->Draw("hist same");

	leg.AddEntry(wavesShifted[ic],Form("Channel %i",ic),"l");
	//Show boundaries of pulse
	if(displayPulseBounds){
	    TLine line; line.SetLineWidth(2); line.SetLineStyle(3);	line.SetLineColor(colors[colorIndex]);
	    for(uint ip=0; ip<boundsShifted[ic].size();ip++){
		if (boundsShifted[ic][ip][0] > timeRange[0] && boundsShifted[ic][ip][1] < timeRange[1]){
		    line.DrawLine(boundsShifted[ic][ip][0],0,boundsShifted[ic][ip][0],0.2*maxheight);
		    line.DrawLine(boundsShifted[ic][ip][1],0,boundsShifted[ic][ip][1],0.2*maxheight);
		}
	    }
	}   
	//Display values stored for this pulse
    }
    float boxw= 0.025;
    float boxh=0.0438;
    float barw=0.03;
    float barh=0.53;
    //    vector<float> xpos = {0.93,0.96,0.93,0.96,0.93,0.96,
    // 0.815,0.845,0.815,0.845,0.815,0.845,
    // 0.7,0.73,0.7,0.73,0.7,0.73
    //    };
    //    


    vector<float> xstart = {0.66,0.78,0.9};
    vector<float> ystart= {0.798,0.851,0.904};

    float sheet_width = 0.006;
    float sheet_offset= 0.013+sheet_width/2.;
    float sheet_left_to_right = 4*0.006+barw+boxw+0.002;
    vector<float> xstart_leftsheets = {xstart[0]-sheet_offset,xstart[1]-sheet_offset,xstart[2]-sheet_offset}; 
    vector<float> xstart_topsheets = {xstart[0]-0.002,xstart[1]-0.002,xstart[2]-0.002};
    float ystart_topsheets = ystart[2]+boxh+0.017;
    float ystart_sidesheets = ystart[0]-0.006;
    float vert_sheet_length = ystart[2]-ystart[0]+boxh+0.01;
    float hori_sheet_length= barw+boxw+0.004;

    float slab_width = 0.015;
    vector<float> slab_xstart = {xstart_leftsheets[0]-0.008-slab_width,xstart_leftsheets[1]-0.008-slab_width,xstart_leftsheets[2]-0.008-slab_width,xstart_leftsheets[2]+0.01+sheet_left_to_right}; 
    float slab_height = vert_sheet_length-0.02;
    float slab_ystart = ystart_sidesheets+0.01;
    // vector<float> ypos = 
    // {0.924,0.924,0.871,0.871,0.818,0.818,
    //     0.924,0.924,0.871,0.871,0.818,0.818,
    //     0.924,0.924,0.871,0.871,0.818,0.818
    // };

    TPave L1frame(xstart[0]-0.006,ystart[0]-0.01,xstart[0]+barw+boxw+0.006,ystart[2]+boxh+0.01,1,"NDC");
    L1frame.SetFillColor(0);
    L1frame.SetLineWidth(2);
    L1frame.Draw();

    TPave L2frame(xstart[1]-0.006,ystart[0]-0.01,xstart[1]+barw+boxw+0.006,ystart[2]+boxh+0.01,1,"NDC");
    L2frame.SetFillColor(0);
    L2frame.SetLineWidth(2);
    L2frame.Draw();

    TPave L3frame(xstart[2]-0.006,ystart[0]-0.01,xstart[2]+barw+boxw+0.006,ystart[2]+boxh+0.01,1,"NDC");
    L3frame.SetFillColor(0);
    L3frame.SetLineWidth(2);
    L3frame.Draw();


    TLatex tla;
    tla.SetTextSize(0.04);
    tla.SetTextFont(42);
    float height= 0.06;
    //tla.DrawLatexNDC(0.13,0.83,Form("Number of pulses: %i",(int)bounds.size()));
    float currentYpos=0.737;
    float headerX=0.67;
    float rowX=0.69;
    int pulseIndex=0; // Keep track of pulse index, since all pulses for all channels are actually stored in the same 1D vectors
    int maxPerChannel = 0;
    if (chanList.size() > 0) maxPerChannel = 10/chanList.size();

    for(uint i=0;i<chanList.size();i++){
	int ic = chanList[i];	
	int chan = chanArray->GetAt(ic);
	if (onlyForceChans && forceChan.find(chan)==forceChan.end()) continue;
	if(ic==-15 && forceChan.find(chan)==forceChan.end()) continue;
	//if(i==15) continue;
	//xyz
	int column= chanMap[ic][0];
	int row= chanMap[ic][1];
	int layer= chanMap[ic][2];
	int type= chanMap[ic][3];
	int colorIndex = 4-2*(row-1)+column-1+6*(layer-1);
	if(type==1) colorIndex = layer; //slabs: 0-3
	else if(type==2) colorIndex = 4 + 3*(layer-1) + (column+1); //sheets

	if(ic==-15) colorIndex=1;

	TPave * pave;
	if (type==1){
	    float xpos,ypos;
	    ypos = slab_ystart;
	    xpos = slab_xstart[layer];
	    pave = new TPave(xpos,ypos,xpos+slab_width,ypos+slab_height,0,"NDC");
	    pave->SetFillColor(colors[colorIndex]);
	    pave->Draw();
	}

	else if (type==2){
	    float xpos,ypos;

	    if (column!=0){
		xpos= xstart_leftsheets[layer-1];
		if(column>0) xpos += sheet_left_to_right;
		ypos = ystart_sidesheets;
		pave = new TPave(xpos,ypos,xpos+sheet_width,ypos+vert_sheet_length,0,"NDC");
	    }
	    else{
		xpos = xstart_topsheets[layer-1];
		ypos = ystart_topsheets;
		pave = new TPave(xpos,ypos,xpos+hori_sheet_length,ypos+1.4/0.8*sheet_width,0,"NDC");
	    }

	    pave->SetFillColor(colors[colorIndex]);
	    pave->Draw();

	}
	else if(type==0){
	    float xpos = xstart[layer-1]+(column-1)*barw;
	    float ypos= ystart[row-1];
	    pave = new TPave(xpos,ypos,xpos+boxw,ypos+boxh,0,"NDC");
	    pave->SetFillColor(colors[colorIndex]);
	    pave->Draw();
	}

	tla.SetTextColor(colors[colorIndex]);
	tla.SetTextSize(0.04);
	tla.DrawLatexNDC(headerX,currentYpos,Form("Channel %i, V_{max} = %0.0f, N_{pulses}= %i",chan,originalMaxHeights[ic],(int)boundsShifted[ic].size()));
	tla.SetTextColor(kBlack);
	currentYpos-=height;
	tla.SetTextSize(0.035);

	for(int ip=0;ip<boundsShifted[ic].size();ip++){
	    while (v_chan->at(pulseIndex) != chan){
		pulseIndex++; 
	    }
	    TString digis="%.1f";
	    if(v_height->at(pulseIndex)>10) digis = "%.0f";
	    TString row;
	    if (calibrateDisplay) row = Form("%.0f ns: "+digis+" mV, %.0f pVs, %.0f ns",v_time_module_calibrated->at(pulseIndex),v_height->at(pulseIndex),v_area->at(pulseIndex),v_duration->at(pulseIndex));
	    else row = Form("%.0f ns: "+digis+" mV, %.0f pVs, %.0f ns",v_time->at(pulseIndex),v_height->at(pulseIndex),v_area->at(pulseIndex),v_duration->at(pulseIndex));
	    pulseIndex++; 
	    if(ip < maxPerChannel){			
		tla.DrawLatexNDC(rowX,currentYpos,row);
		currentYpos-=height*0.8;
	    }
	}
	currentYpos-=height*0.2;

    }

    // tla.DrawLatexNDC(0.13,0.78,Form("Area: %0.2f",v_area->back()));
    // tla.DrawLatexNDC(0.13,0.73,Form("Duration: %0.2f",v_duration->back()));
    // leg.Draw();
    cout<<"Display directory is "<<displayDirectory<<endl;
    TString displayName;
    if (runFFT) displayName=Form(displayDirectory+"Run%i_File%i_Event%i_%s_FFT.pdf",runNum,fileNum,event,tag.Data()); 
    else displayName=Form(displayDirectory+"Run%i_File%i_Event%i_%s.pdf",runNum,fileNum,event,tag.Data());
    c.Print(displayName);

}

void findTriggerCandidates(int ic,float sb_mean){
    float tstart = sideband_range[1]+1;
    int istart = waves[ic]->FindBin(tstart);
    float v;
    bool inTrigger = false;
    for (int i=istart; i<=waves[ic]->GetNbinsX(); i++) { // Loop over all samples looking for pulses
	v = waves[ic]->GetBinContent(i);
	// if (ic == 7) std::cout  << triggerThreshold << " "<< waves[ic]->GetMaximum()+ sb_mean << " "<< max_7  << std::endl;
	// std::cout << sb_mean << std::endl;
	// std::cout << waves[ic]->GetBinContent(i) << std::endl;
	// std::cout << triggerThreshold << std::endl;
	if (waves[ic]->GetBinContent(i) + sb_mean >= (v_triggerThresholds->at(ic)*-1000.)){
	    if (!inTrigger){
		v_triggerCandidates->push_back((float)waves[ic]->GetBinLowEdge(i));
		v_triggerCandidatesChannel->push_back(ic);
		inTrigger = true;
	    }
	}
	else {
	    if (inTrigger) v_triggerCandidatesEnd->push_back((float)waves[ic]->GetBinLowEdge(i));
	    inTrigger = false;
	}
    }
    if (inTrigger) v_triggerCandidatesEnd->push_back((float)waves[ic]->GetBinLowEdge(waves[ic]->GetNbinsX()));
}
vector< vector<float> > findPulses(int ic, bool applyLPFilter, int runDRS){
    //Configurable:
    // int Nconsec = 4;
    // int NconsecEnd = 3;
    // float thresh = 2.5; //mV
    int NconsecConfig1p6 = 4;
    int NconsecEndConfig1p6 =4;
    float threshConfig1p6 = 2.0;

    int NconsecConfig1p6Filtered = 3;
    int NconsecEndConfig1p6Filtered =4;
    float threshConfig1p6Filtered = 1.4;

    int NconsecConfig0p8 = 2;
    int NconsecEndConfig0p8 =3;
    float threshConfig0p8 = 2.5;

    int Nconsec = 6;
    int NconsecEnd = 12;
    float thresh = 2.0;
    float lowThresh = 1.0;

    if (runDRS >= 0 || int(sample_rate[0]/0.4+0.1) == 4)
    {
	Nconsec = NconsecConfig1p6;
	NconsecEnd = NconsecEndConfig1p6;
	thresh = threshConfig1p6;
	lowThresh = thresh - rmsCalib/2;
    }
    else if (int(sample_rate[0]/0.4+0.1) == 2)
    {
	Nconsec = NconsecConfig0p8;
	NconsecEnd = NconsecEndConfig0p8;
	thresh = threshConfig0p8;
	lowThresh = thresh - rmsCalib;
    }
    if (applyLPFilter){
	Nconsec = 3;//Nconsec-6; // Narrower pulses allowed after low pass filtering
	if (Nconsec<2) Nconsec = 2;
	thresh = threshConfig1p6Filtered;
	lowThresh = threshConfig1p6Filtered;
    }
    // if (event==264) {
    //   cout << "Debug "<< thresh << " " << lowThresh<< " "<< Nconsec << " " << NconsecEnd<<endl;
    // }

    // float scaling = sample_rate[ic/16]/1.6;


    // if(scaling)<1.{
    // Nconsec = int(ceil(Nconsec*scaling)+0.1);
    // NconsecEnd = int(ceil(NconsecEnd*scaling)+0.1);
    // if (Nconsec < 2 && Nconsec < NconsecConfig[ic]) Nconsec = 2;
    // if (NconsecEnd < 2 && NconsecEnd < NconsecEndConfig[ic]) NconsecEnd = 2;
    // }

    vector<vector<float> > bounds;
    float tstart = sideband_range[1]+1;
    int istart = waves[ic]->FindBin(tstart);

    // Now hunt for pulses
    bool inpulse = false;
    int nover = 0; // Number of samples seen consecutively over threshold
    int nunder = 0; // Number of samples seen consecutively under threshold
    int i_begin = istart;
    int i_stop_searching = waves[ic]->GetNbinsX()-Nconsec;
    int i_stop_final_pulse = waves[ic]->GetNbinsX();
    // int tWindow[2];
    for (int i=istart; i<i_stop_searching || (inpulse && i<i_stop_final_pulse); i++) { // Loop over all samples looking for pulses
	float v = waves[ic]->GetBinContent(i);
	if (!inpulse) { // Not in a pulse?
	    if (v<lowThresh) {
		// Reset any prepulse counters
		nover = 0;
		i_begin = i; // most recent sample below threshold
	    }
	    else if (v>=thresh){		
		nover++; // Another sample over threshold
		//cout << "DEBUG: Over pulse, t = "<< w.t[i] <<", v = "<<v<<", nover = "<<nover<<endl;
	    }
	    else{
		i_begin = i; // most recent sample below threshold
	    }

	    if (nover>=Nconsec) {
		//cout << "DEBUG: Starting pulse, t = "<< w.t[i] <<", v = "<<v<<endl;
		inpulse = true; // Start a pulse
		nunder = 0; // Counts number of samples underthreshold to end a pulse
	    }
	} // Not in a pulse?
	else { // In a pulse?
	    if (v<thresh) nunder++;
	    else if (v >= thresh+rmsCalib/2){
		// Restart the tail counting
		nunder = 0;
	    }
	    //cout << "DEBUG: Inside pulse, t = "<< w.t[i] <<", v = "<<v<<", nunder = "<<nunder<<endl;
	    if (nunder>=NconsecEnd || i==(i_stop_final_pulse-1)) { // The end of a pulse, or pulse has reached the end of range 

		//cout<<"DEBUG: i_begin "<<i_begin<<endl;
		// cout<<"DEBUG: tWindow 0 and 1: "<<w.t[i_begin]<<" "<<w.t[i]<<endl;

		bounds.push_back({(float)waves[ic]->GetBinLowEdge(i_begin), (float)waves[ic]->GetBinLowEdge(i+1)-0.01}); //start and end of pulse
		if(debug) cout<<"i_begin, i: "<<i_begin<<" "<<i<<endl;

		inpulse = false; // End the pulse
		nover = 0;
		nunder = 0;
		i_begin = i;
	    }
	}
    }
    return bounds;
}

vector< vector<float> > findPulses_inside_out(int ic){

    //Configurable
    float thresh = 7; //mV. Threshold to identify a pulse
    float thresh_noise = 3.5; //mV. Threshold to end a pulse


    vector<vector<float> > bounds;
    float tstart = sideband_range[1]+1;
    int istart = waves[ic]->FindBin(tstart);

    int i_begin, i_end;

    for (int i=istart; i<waves[ic]->GetNbinsX(); i++) { // Loop over all samples looking for pulses
		float v = waves[ic]->GetBinContent(i);
		if(v>thresh){
		    //Add a pulse, adding bins on either side starting from i
		    i_begin=i;
		    i_end=i;

		    //loop to find last previous sample beneath noise threshold
		    while(v>thresh_noise && i_begin>0){
			i_begin--;
			v = waves[ic]->GetBinContent(i_begin);
		    }
		    i_begin++; //start pulse at first sample ABOVE threshold.

		    //loop to find next sample beneath noise threshold
		    v = waves[ic]->GetBinContent(i);
		    while(v>thresh_noise && i_end<=waves[ic]->GetNbinsX()){
			i_end++;
			v = waves[ic]->GetBinContent(i_end);
		    }
		    i_end--; //end pulse at last sample ABOVE threshold.

		    i=i_end+1; //Start where we left off next time

		    bounds.push_back({(float)waves[ic]->GetBinLowEdge(i_begin), (float)waves[ic]->GetBinLowEdge(i_end+1)});
		}
    }

    return bounds;
}

void prepareOutBranches(){


    //MAKE SURE TO ALWAYS ADD BRANCHES TO CLEAR FUNCTION AS WELL

    TBranch * b_event = outTree->Branch("event",&event,"event/I");
    TBranch * b_run = outTree->Branch("run",&runNumOrig,"run/I");
    TBranch * b_file = outTree->Branch("file",&fileNum,"file/I");
    TBranch * b_fill = outTree->Branch("fill",&fillNum,"fill/I");
    TBranch * b_nRollOvers = outTree->Branch("nRollOvers",&nRollOvers,"nRollOvers/I");
    TBranch * b_beam = outTree->Branch("beam",&beam,"beam/O");
    TBranch * b_hardNoBeam = outTree->Branch("hardNoBeam",&hardNoBeam,"hardNoBeam/O");
    TBranch * b_fillAvgLumi = outTree->Branch("fillAvgLumi",&fillAvgLumi,"fillAvgLumi/F");
    TBranch * b_fillTotalLumi = outTree->Branch("fillTotalLumi",&fillTotalLumi,"fillTotalLumi/F");
    TBranch * b_present_b0 = outTree->Branch("present_b0",&present_b0,"present_b0/O");
    TBranch * b_present_b1 = outTree->Branch("present_b1",&present_b1,"present_b1/O");
    TBranch * b_event_time_b0 = outTree->Branch("event_time_b0",&event_time_b0,"event_time_b0/L");
    TBranch * b_event_time_b1 = outTree->Branch("event_time_b1",&event_time_b1,"event_time_b1/L");
    TBranch * b_event_time_fromTDC = outTree->Branch("event_time_fromTDC",&event_time_fromTDC,"event_time_fromTDC/D");
    TBranch * b_t_since_fill_start = outTree->Branch("t_since_fill_start",&t_since_fill_start,"t_since_fill_start/I");
    TBranch * b_t_since_fill_end = outTree->Branch("t_since_fill_end",&t_since_fill_end,"t_since_fill_end/I");
    TBranch * b_t_until_next_fill = outTree->Branch("t_until_next_fill",&t_until_next_fill,"t_until_next_fill/I");
    TBranch * b_event_t_string = outTree->Branch("event_t_string",&event_t_string);
    TBranch * b_event_trigger_time_tag_b0 = outTree->Branch("event_trigger_time_tag_b0",&event_trigger_time_tag_b0,"event_trigger_time_tag_b0/L");
    TBranch * b_event_trigger_time_tag_b1 = outTree->Branch("event_trigger_time_tag_b1",&event_trigger_time_tag_b1,"event_trigger_time_tag_b1/L");

    TBranch * b_chan = outTree->Branch("chan",&v_chan);
    TBranch * b_triggerCandidates = outTree->Branch("triggerCandidates",&v_triggerCandidates);
    TBranch * b_triggerCandidatesEnd = outTree->Branch("triggerCandidatesEnd",&v_triggerCandidatesEnd);
    TBranch * b_triggerCandidatesChannel = outTree->Branch("triggerCandidatesChannel",&v_triggerCandidatesChannel);
    TBranch * b_layer = outTree->Branch("layer",&v_layer);
    TBranch * b_row = outTree->Branch("row",&v_row);
    TBranch * b_column = outTree->Branch("column",&v_column);
    TBranch * b_type = outTree->Branch("type",&v_type);
    TBranch * b_height = outTree->Branch("height",&v_height);
    TBranch * b_time = outTree->Branch("time",&v_time);
    TBranch * b_time_module_calibrated = outTree->Branch("time_module_calibrated",&v_time_module_calibrated);
    TBranch * b_delay = outTree->Branch("delay",&v_delay);
    TBranch * b_area = outTree->Branch("area",&v_area);
    TBranch * b_nPE = outTree->Branch("nPE",&v_nPE);
    TBranch * b_ipulse = outTree->Branch("ipulse",&v_ipulse);
    TBranch * b_npulses = outTree->Branch("npulses",&v_npulses);
    TBranch * b_duration = outTree->Branch("duration",&v_duration);
    TBranch * b_quiet = outTree->Branch("quiet",&v_quiet);
    TBranch * b_presample_mean = outTree->Branch("presample_mean",&v_presample_mean);
    TBranch * b_presample_RMS = outTree->Branch("presample_RMS",&v_presample_RMS);
    TBranch * b_sideband_mean = outTree->Branch("sideband_mean",&v_sideband_mean);
    TBranch * b_sideband_RMS = outTree->Branch("sideband_RMS",&v_sideband_RMS);
    TBranch * b_sideband_mean_perFile = outTree->Branch("sideband_mean_perFile",&v_sideband_mean_perFile);
    TBranch * b_triggerBand_mean = outTree->Branch("triggerBand_mean",&v_triggerBand_mean);
    TBranch * b_triggerBand_max = outTree->Branch("triggerBand_max",&v_triggerBand_max);
    TBranch * b_triggerBand_maxTime = outTree->Branch("triggerBand_maxTime",&v_triggerBand_maxTime);
    TBranch * b_triggerBand_RMS = outTree->Branch("triggerBand_RMS",&v_triggerBand_RMS);
    TBranch * b_sideband_mean_calib = outTree->Branch("sideband_mean_calib",&v_sideband_mean_calib);
    TBranch * b_sideband_RMS_calib = outTree->Branch("sideband_RMS_calib",&v_sideband_RMS_calib);

    TBranch * b_groupTDC_b0 = outTree->Branch("groupTDC_b0",&v_groupTDC_b0);
    TBranch * b_groupTDC_b1 = outTree->Branch("groupTDC_b1",&v_groupTDC_b1);

    TBranch * b_max = outTree->Branch("max",&v_max);
    TBranch * b_min = outTree->Branch("min",&v_min);
    TBranch * b_max_afterFilter = outTree->Branch("maxAfterFilter",&v_max_afterFilter);
    TBranch * b_max_threeConsec = outTree->Branch("maxThreeConsec",&v_max_threeConsec);
    TBranch * b_min_afterFilter = outTree->Branch("minAfterFilter",&v_min_afterFilter);
    TBranch * b_triggerThresholds = outTree->Branch("triggerThreshold",&v_triggerThresholds);
    TBranch * b_triggerEnable = outTree->Branch("triggerEnable",&v_triggerEnable);
    TBranch * b_triggerMajority = outTree->Branch("triggerMajority",&v_triggerMajority);
    TBranch * b_triggerLogic = outTree->Branch("triggerLogic",&v_triggerLogic);

    TBranch * b_bx = outTree->Branch("bx",&v_bx);
    TBranch * b_by = outTree->Branch("by",&v_by);
    TBranch * b_bz = outTree->Branch("bz",&v_bz);

    TBranch *  b_chan_muDist = outTree->Branch("chan_muDist",chan_muDist,"chan_muDist[890]/F");
    TBranch *  b_chan_trueNPE = outTree->Branch("chan_trueNPE",chan_trueNPE,"chan_trueNPE[890]/I");
    TBranch *  b_scale1fb = outTree->Branch("scale1fb",&scale1fb,"scale1fb/F");
    TBranch *  b_fileID = outTree->Branch("fileID",&fileID,"fileID/I");
    TBranch *  b_orig_evt = outTree->Branch("orig_evt",&orig_evt,"orig_evt/I");
    TBranch *  b_mcTruth_fourBarLine = outTree->Branch("mcTruth_fourBarLine",&mcTruth_fourBarLine,"mcTruth_fourBarLine/O");
    TBranch * b_mcTruth_twoSlab = outTree->Branch("mcTruth_twoSlab",&mcTruth_twoSlab,"mcTruth_twoSlab/O");

    outTree->SetBranchAddress("chan_muDist",chan_muDist,&b_chan_muDist);
    outTree->SetBranchAddress("chan_trueNPE",chan_trueNPE,&b_chan_trueNPE);
    outTree->SetBranchAddress("scale1fb",&scale1fb,&b_scale1fb);
    outTree->SetBranchAddress("fileID",&fileID,&b_fileID);
    outTree->SetBranchAddress("orig_evt",&orig_evt,&b_orig_evt);
    outTree->SetBranchAddress("mcTruth_fourBarLine",&mcTruth_fourBarLine,&b_mcTruth_fourBarLine);
    outTree->SetBranchAddress("mcTruth_twoSlab",&mcTruth_twoSlab,&b_mcTruth_twoSlab);

    outTree->SetBranchAddress("event",&event,&b_event);
    outTree->SetBranchAddress("run",&runNumOrig,&b_run);
    outTree->SetBranchAddress("file",&fileNum,&b_file);
    outTree->SetBranchAddress("nRollOvers",&nRollOvers,&b_nRollOvers);
    outTree->SetBranchAddress("event_time_b0",&event_time_b0,&b_event_time_b0);
    outTree->SetBranchAddress("event_time_b1",&event_time_b1,&b_event_time_b1);
    outTree->SetBranchAddress("event_time_fromTDC",&event_time_fromTDC,&b_event_time_fromTDC);
    outTree->SetBranchAddress("t_since_fill_start",&t_since_fill_start,&b_t_since_fill_start);
    outTree->SetBranchAddress("t_since_fill_end",&t_since_fill_end,&b_t_since_fill_end);
    outTree->SetBranchAddress("t_until_next_fill",&t_until_next_fill,&b_t_until_next_fill);
    outTree->SetBranchAddress("fill",&fillNum,&b_fill);
    outTree->SetBranchAddress("beam",&beam,&b_beam);
    outTree->SetBranchAddress("hardNoBeam",&hardNoBeam,&b_hardNoBeam);
    outTree->SetBranchAddress("fillAvgLumi",&fillAvgLumi,&b_fillAvgLumi);
    outTree->SetBranchAddress("fillTotalLumi",&fillTotalLumi,&b_fillTotalLumi);
    outTree->SetBranchAddress("present_b0",&present_b0,&b_present_b0);
    outTree->SetBranchAddress("present_b1",&present_b1,&b_present_b1);
    outTree->SetBranchAddress("event_trigger_time_tag_b0",&event_trigger_time_tag_b0,&b_event_trigger_time_tag_b0);
    outTree->SetBranchAddress("event_trigger_time_tag_b1",&event_trigger_time_tag_b1,&b_event_trigger_time_tag_b1);

    //outTree->SetBranchAddress("event_t_string",&event_t_string,&b_event_t_string);
    outTree->SetBranchAddress("chan",&v_chan,&b_chan);
    outTree->SetBranchAddress("triggerCandidates",&v_triggerCandidates,&b_triggerCandidates);
    outTree->SetBranchAddress("triggerCandidatesEnd",&v_triggerCandidatesEnd,&b_triggerCandidatesEnd);
    outTree->SetBranchAddress("triggerCandidatesChannel",&v_triggerCandidatesChannel,&b_triggerCandidatesChannel);
    outTree->SetBranchAddress("layer",&v_layer,&b_layer);
    outTree->SetBranchAddress("row",&v_row,&b_row);
    outTree->SetBranchAddress("column",&v_column,&b_column);
    outTree->SetBranchAddress("type",&v_type,&b_type);
    outTree->SetBranchAddress("height",&v_height,&b_height);
    outTree->SetBranchAddress("time_module_calibrated",&v_time_module_calibrated,&b_time_module_calibrated);
    outTree->SetBranchAddress("time",&v_time,&b_time);
    outTree->SetBranchAddress("delay",&v_delay,&b_delay);
    outTree->SetBranchAddress("area",&v_area,&b_area);
    outTree->SetBranchAddress("nPE",&v_nPE,&b_nPE);
    outTree->SetBranchAddress("ipulse",&v_ipulse,&b_ipulse);
    outTree->SetBranchAddress("npulses",&v_npulses,&b_npulses);
    outTree->SetBranchAddress("duration",&v_duration,&b_duration);
    outTree->SetBranchAddress("quiet",&v_quiet,&b_quiet);
    outTree->SetBranchAddress("presample_mean",&v_presample_mean,&b_presample_mean);
    outTree->SetBranchAddress("presample_RMS",&v_presample_RMS,&b_presample_RMS);
    outTree->SetBranchAddress("sideband_mean",&v_sideband_mean,&b_sideband_mean);
    outTree->SetBranchAddress("sideband_RMS",&v_sideband_RMS,&b_sideband_RMS);
    outTree->SetBranchAddress("sideband_mean_perFile",&v_sideband_mean_perFile,&b_sideband_mean_perFile);
    outTree->SetBranchAddress("triggerBand_mean",&v_triggerBand_mean,&b_triggerBand_mean);
    outTree->SetBranchAddress("triggerBand_max",&v_triggerBand_max,&b_triggerBand_max);
    outTree->SetBranchAddress("triggerBand_maxTime",&v_triggerBand_maxTime,&b_triggerBand_maxTime);
    outTree->SetBranchAddress("triggerBand_RMS",&v_triggerBand_RMS,&b_triggerBand_RMS);
    outTree->SetBranchAddress("sideband_mean_calib",&v_sideband_mean_calib,&b_sideband_mean_calib);
    outTree->SetBranchAddress("sideband_RMS_calib",&v_sideband_RMS_calib,&b_sideband_RMS_calib);

    outTree->SetBranchAddress("groupTDC_b0",&v_groupTDC_b0,&b_groupTDC_b0);
    outTree->SetBranchAddress("groupTDC_b1",&v_groupTDC_b1,&b_groupTDC_b1);
    outTree->SetBranchAddress("max",&v_max,&b_max);
    outTree->SetBranchAddress("triggerThreshold",&v_triggerThresholds,&b_triggerThresholds);
    outTree->SetBranchAddress("triggerLogic",&v_triggerLogic,&b_triggerLogic);
    outTree->SetBranchAddress("triggerMajority",&v_triggerMajority,&b_triggerMajority);
    outTree->SetBranchAddress("triggerEnable",&v_triggerEnable,&b_triggerEnable);
    outTree->SetBranchAddress("maxAfterFilter",&v_max_afterFilter,&b_max_afterFilter);
    outTree->SetBranchAddress("maxThreeConsec",&v_max_threeConsec,&b_max_threeConsec);
    outTree->SetBranchAddress("minAfterFilter",&v_min_afterFilter,&b_min_afterFilter);

    outTree->SetBranchAddress("bx",&v_bx,&b_bx);
    outTree->SetBranchAddress("by",&v_by,&b_by);
    outTree->SetBranchAddress("bz",&v_bz,&b_bz);



}

void clearOutBranches(){
    v_max->clear();
    v_max_afterFilter->clear();
    v_max_threeConsec->clear();
    v_min->clear();
    v_min_afterFilter->clear();
    v_chan->clear();
    v_triggerCandidates->clear();
    v_triggerCandidatesEnd->clear();
    v_triggerCandidatesChannel->clear();
    v_layer->clear();
    v_row->clear();
    v_column->clear();
    v_type->clear();
    v_height->clear();
    v_time->clear();
    v_time_module_calibrated->clear();
    v_area->clear();
    v_nPE->clear();
    v_ipulse->clear();
    v_npulses->clear();
    v_duration->clear();
    v_delay->clear();
    v_sideband_mean->clear();
    v_sideband_RMS->clear();
    v_sideband_mean_perFile->clear();
    v_triggerBand_mean->clear();
    v_triggerBand_max->clear();
    v_triggerBand_maxTime->clear();
    v_triggerBand_RMS->clear();
    v_sideband_mean_calib->clear();
    v_sideband_RMS_calib->clear();
    v_quiet->clear();
    v_presample_mean->clear();
    v_presample_RMS->clear();
    v_groupTDC_b0->clear();
    v_groupTDC_b1->clear();
    v_bx->clear();
    v_by->clear();
    v_bz->clear();


}


void h1cosmetic(TH1D *hist,int ic){
    hist->SetLineWidth(2);
    hist->SetLineColor(colors[ic]);
    hist->SetStats(0);
    hist->GetXaxis()->SetTitleSize(0.045);
    hist->GetXaxis()->SetLabelSize(0.04);
    hist->GetYaxis()->SetTitleSize(0.045);
    hist->GetYaxis()->SetLabelSize(0.04);
    hist->GetYaxis()->SetTitleOffset(1.01);
    hist->GetXaxis()->SetTitleOffset(0.96);

}

void writeVersion(bool applyLPFilter){
    string version = "longtagplaceholder";
    //string version = GetStdoutFromCommand("git describe --tags --long");
    cout<<"Git tag is "<<version+"_filtered"<<endl;
    TNamed v;
    if (applyLPFilter) v = TNamed("tag",version+"_filtered");
    else v= TNamed("tag",version);
    v.Write();

}
void loadBranchesDRS(){
    inTree->SetBranchAddress("chan_muDist",chan_muDistDRS);
    inTree->SetBranchAddress("chan_trueNPE",chan_trueNPEDRS);
    inTree->SetBranchAddress("fileID",&fileIDDRS);
    inTree->SetBranchAddress("scale1fb",&scale1fbDRS);
    inTree->SetBranchAddress("orig_evt",&orig_evtDRS);
    inTree->SetBranchAddress("mcTruth_fourBarLine",&mcTruth_fourBarLineDRS);
    inTree->SetBranchAddress("mcTruth_twoSlab",&mcTruth_twoSlabDRS);
    inTree->SetBranchAddress("timestamp",&timestampSecs);
    for(int ic=0;ic<numChan;ic++) 
    { 
		TString ch_num = std::to_string(ic);	
    	inTree->SetBranchAddress("voltages_"+ch_num,arrayVoltageDRS[ic]);
		int chan = chanArray->GetAt(ic);
		TString branchName; 
		waves.push_back(new TH1D(TString(chan),"",1024,0,1024*1./sample_rate[0]));
    }
}

void loadBranchesMilliDAQ_fullSim(){
	waves.clear();
	//I have no idea why waves.push_back needs to have its own loop,
	//but, if otherwise, the script crashes...
    for(int ic=0;ic<numChan;ic++){
		waves.push_back(new TH1D());
	}
    for(int ic=0;ic<numChan;ic++){
		TString ch_num = std::to_string(ic);
		inTree->SetBranchAddress("waves_"+ch_num, &waves[ic]);
	}
}

void loadBranchesMilliDAQ(){
    inTree->SetBranchAddress("event", &evt);
    for(int ic=0;ic<numChan;ic++) waves.push_back(new TH1D());
}

void loadWavesDRS(){
    for (int i = 0; i < 1024; i++){
    	for(int ic=0;ic<numChan;ic++){ 
			waves[ic]->SetBinContent(i+1,arrayVoltageDRS[ic][i]);
		}
    }
}
void loadWavesMilliDAQ(){
    int board,chan;
    for(int i=0;i<numChan;i++){
		if(waves[i]) delete waves[i];
		//waves[i] = (TH1D*) evt->GetWaveform(i, Form("h%i",i));
		waves[i] = (TH1D*)evt->GetWaveform(0, 9, Form("h%i",i));	//set all waveforms as old channel 25 (use digitizer 0, channel 9 / ET for convenience)
    }
}

/*
void loadWavesMilliDAQ_fullSim(){
    for(int i=0;i<numChan;i++){
		if(waves[i]) delete waves[i];
		waves[i] = (TH1D*)waves_zb[i];
    }
}
*/

void loadBranchesInteractiveDAQ(){
    for(int ic=0;ic<numChan;ic++) waves.push_back(new TH1D());

    auto branch0 = inTree->GetBranch("channel_0");
    auto branch1 = inTree->GetBranch("channel_1");
    auto branch2 = inTree->GetBranch("channel_2");
    auto branch3 = inTree->GetBranch("channel_3");
    auto branch4 = inTree->GetBranch("channel_4");
    auto branch5 = inTree->GetBranch("channel_5");
    auto branch6 = inTree->GetBranch("channel_6");
    auto branch7 = inTree->GetBranch("channel_7");
    auto branch8 = inTree->GetBranch("channel_8");
    auto branch9 = inTree->GetBranch("channel_9");
    auto branch10 = inTree->GetBranch("channel_10");
    auto branch11 = inTree->GetBranch("channel_11");
    auto branch12 = inTree->GetBranch("channel_12");
    auto branch13 = inTree->GetBranch("channel_13");
    auto branch14 = inTree->GetBranch("channel_14");
    auto branch15 = inTree->GetBranch("channel_15");

    branch0->SetAddress(&(waves[0]));
    branch1->SetAddress(&(waves[1]));
    branch2->SetAddress(&(waves[2]));
    branch3->SetAddress(&(waves[3]));
    branch4->SetAddress(&(waves[4]));
    branch5->SetAddress(&(waves[5]));
    branch6->SetAddress(&(waves[6]));
    branch7->SetAddress(&(waves[7]));
    branch8->SetAddress(&(waves[8]));
    branch9->SetAddress(&(waves[9]));
    branch10->SetAddress(&(waves[10]));
    branch11->SetAddress(&(waves[11]));
    branch12->SetAddress(&(waves[12]));
    branch13->SetAddress(&(waves[13]));
    branch14->SetAddress(&(waves[14]));
    branch15->SetAddress(&(waves[15]));

}

void getFileCreationTime(const char *path) {
    struct stat attr;
    stat(path, &attr);
    printf("Fill list last updated on %s", ctime(&attr.st_mtime));
}

void loadFillList(TString fillFile){

    int fillnumber, start,end;
    float lumi,inst_lumi;
    string date,time;

    getFileCreationTime(fillFile.Data());

    ifstream infile;
    infile.open(fillFile); 

    while(infile >> fillnumber >> start >> end >> lumi >> date >> time >> inst_lumi){
	if(end<=start && end>0) cout<<"Error in fill time list"<<endl; //end == -1 indicates ongoing fill
	else{
	    //cout<<Form("Appending %i %i %i %0.2f",start,end,fillnumber,lumi)<<endl;
	    fillList.push_back(make_tuple(start,end,fillnumber,lumi,inst_lumi));
	}
    }
    sort(fillList.begin(),fillList.end());
    cout<<"First fill "<<get<2>(fillList[0])<<endl;
    cout<<"Last fill "<<get<2>(fillList[fillList.size()-1])<<endl;

}

tuple<int,int,float,float,float, float> findFill(int seconds){
    auto index_of_first_fill_with_larger_start_time = distance(fillList.begin(), lower_bound(fillList.begin(),fillList.end(), 
		make_tuple(seconds, seconds, 0, 0.,0.) ));
    //cout<<"index "<<index_of_first_fill_with_larger_start_time<<endl;
    int this_fill=0;
    int time_since_start=-1;
    float average_instantaneous_lumi=-1;
    float total_lumi=-1;
    float time_until_next_fill = -1.;
    float time_since_end = -1.;
    if(index_of_first_fill_with_larger_start_time > (int)fillList.size()) this_fill=-1;
    //This event is during a fill if its time is less than the end time of the fill before the first fill with a larger start time
    else if(seconds < get<1>(fillList[index_of_first_fill_with_larger_start_time-1]) || get<1>(fillList[index_of_first_fill_with_larger_start_time-1])==-1){
	//end time == -1 indicates ongoing fill 
	this_fill = get<2>(fillList[index_of_first_fill_with_larger_start_time-1]); //fill number
	time_since_start = seconds - get<0>(fillList[index_of_first_fill_with_larger_start_time-1]); //time of this event - start time of fill
	total_lumi = get<3>(fillList[index_of_first_fill_with_larger_start_time-1]);
	average_instantaneous_lumi = get<4>(fillList[index_of_first_fill_with_larger_start_time-1]);
    }
    else{
	time_until_next_fill = get<0>(fillList[index_of_first_fill_with_larger_start_time]) - seconds;
	time_since_end = seconds - get<1>(fillList[index_of_first_fill_with_larger_start_time-1]);
    }
    return make_tuple(this_fill,time_since_start,average_instantaneous_lumi,total_lumi,time_until_next_fill,time_since_end);
}


vector<TString> getFieldFileList(TString location){
    vector<TString> files;
    TTimeStamp startOfFile = evt->digitizers[0].DAQTimeStamp;
    int date = startOfFile.GetDate();
    files.push_back(Form("%s/environ_data_%i.txt",location.Data(),date));

    //Also include day after, in case it spans two days.
    startOfFile.Add(86400); //add 24*3600 seconds
    date = startOfFile.GetDate();
    files.push_back(Form("%s/environ_data_%i.txt",location.Data(),date));
    return files;
}
void loadPhotonList(TString photonFile){
    float time;
    ifstream infile;
    infile.open(photonFile); 
    if(!infile.good()){
	cout<<"Photon time file doesn't exist: "<<photonFile<<endl;
	return;}
    infile.ignore(10000,'\n'); //skip first line
    while(infile >> time){
	//    if(end<=start && end>0) cout<<"Error in fill time list"<<endl; //end == -1 indicates ongoing fill
	//cout<<Form("Appending %i %i %i %0.2f",start,end,fillnumber,lumi)<<endl;
	photonList.push_back(time);
	//        cout<<timestamp<<" "<< x1<<" "<<y1<<" "<<z1<<" "<<x2<<" "<<y2<<" "<<z2<<" "<<x3<<" "<<y3<<" "<<z3<<" "<<x4<<" "<<y4<<" "<<z4<<endl;
    }
    cout<<"Loaded photon time file"<<photonFile<<", "<<photonList.size()<<" photon measurements."<<endl;
}

void loadSidebandList(TString sidebandFile, int runNum){
    int file;
    float sb0,sb1,sb2,sb3,sb4,sb5,sb6,sb7,sb8,sb9,sb10,sb11,sb12,sb13,sb14,sb15,sb16,sb17,sb18,sb19,sb20,sb21,sb22,sb23,sb24,sb25,sb26,sb27,sb28,sb29,sb30,sb31;
    ifstream infile;    
    infile.open(sidebandFile); 
    if (infile.good()){
	while(infile >> file >> sb0 >> sb1 >>sb2 >>sb3 >>sb4 >>sb5 >>sb6 >>sb7 >>sb8 >>sb9 >>sb10 >>sb11 >>sb12 >>sb13 >>sb14 >>sb15 >>sb16 >>sb17 >>sb18 >>sb19 >>sb20 >>sb21 >>sb22 >>sb23 >>sb24 >>sb25 >>sb26 >>sb27 >>sb28 >>sb29 >>sb30 >>sb31){
	    vector<float> sideband = {sb0,sb1,sb2,sb3,sb4,sb5,sb6,sb7,sb8,sb9,sb10,sb11,sb12,sb13,sb14,sb15,sb16,sb17,sb18,sb19,sb20,sb21,sb22,sb23,sb24,sb25,sb26,sb27,sb28,sb29,sb30,sb31};
	    sidebandPerFile[runNum][file] = sideband;
	}
    }
    else {
	std::cout << "sideband file doesn't exist - per file correction will be 0" << std::endl;

    }
}

void loadFieldList(TString fieldFile){
    string timestamp;
    int seconds;
    float x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;


    ifstream infile;
    infile.open(fieldFile); 
    if(!infile.good()){
	cout<<"Field file doesn't exist: "<<fieldFile<<endl;
	return;}
    infile.ignore(10000,'\n'); //skip first line
    while(infile >> seconds>> timestamp >> x1>>y1>>z1>>x2>>y2>>z2>>x3>>y3>>z3>>x4>>y4>>z4){
	//    if(end<=start && end>0) cout<<"Error in fill time list"<<endl; //end == -1 indicates ongoing fill
	//cout<<Form("Appending %i %i %i %0.2f",start,end,fillnumber,lumi)<<endl;
	fieldList.push_back(make_tuple(seconds,x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4));
	//        cout<<timestamp<<" "<< x1<<" "<<y1<<" "<<z1<<" "<<x2<<" "<<y2<<" "<<z2<<" "<<x3<<" "<<y3<<" "<<z3<<" "<<x4<<" "<<y4<<" "<<z4<<endl;
    }
    sort(fieldList.begin(),fieldList.end());
    cout<<"Loaded "<<fieldFile<<", "<<fieldList.size()<<" field measurements so far."<<endl;
    // for (int i=0;i<10;i++){
    //     cout<<get<0>(fieldList[i])<<" "<<get<1>(fieldList[i])<<" "<<get<2>(fieldList[i])<<endl;

    // }
}
int findField(int seconds){
    if(fieldList.size()==0) return -1;
    auto index_of_first_point_with_larger_time = distance(fieldList.begin(), lower_bound(fieldList.begin(),fieldList.end(), 
		make_tuple(seconds, 0., 0.,0., 0.,0., 0.,0., 0.,0., 0.,0., 0.) ));
    //cout<<"index "<<index_of_first_fill_with_larger_start_time<<endl;
    int this_point=0;

    if(fabs(seconds-get<0>(fieldList[index_of_first_point_with_larger_time -1])) < fabs(seconds-get<0>(fieldList[index_of_first_point_with_larger_time])))
	this_point = index_of_first_point_with_larger_time -1;
    else
	this_point = index_of_first_point_with_larger_time;

    //Give garbage value if closest point is more than an hour away.
    if(fabs(seconds - get<0>(fieldList[this_point])) > 3600) this_point=-1;

    return this_point;
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}
string GetStdoutFromCommand(string cmd) {
    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
	while (!feof(stream))
	    if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
	pclose(stream);
    }

    //Strip new lines
    rtrim(data);
    return data;
}
