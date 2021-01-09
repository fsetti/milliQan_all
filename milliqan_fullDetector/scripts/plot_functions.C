#include <stdlib.h>
#include <iostream>
#include <string>
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TProfile.h"
#include "THStack.h"
#include "TString.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TKey.h"
#include "TDirectory.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TF1.h"

vector<TString> loopdir ( TFile *dir, TString obj_class = "TH1" ) {
	//loop on all keys of dir including possible subdirs
	//print a message for all keys with a class TH1

	vector<TString> out_string;	
	TIter next (dir->GetListOfKeys());
	TKey *key;
	while ((key = (TKey*)next())) {
		if (strstr(key->GetClassName(),obj_class)) {
			out_string.push_back( key->GetName() );
			//printf (" key : %s is a %s in %s\n",
			//key->GetName(),key->GetClassName(),dir->GetPath());
		}
	}
	return out_string;
}
	

void plot_avg_nPE(TString mass_point, TString out_dir = "/net/cms25/cms25r5/fsetti/milliQan_plots/full_detector/validation/" ){

	TFile* file = TFile::Open("/net/cms25/cms25r5/fsetti/milliqan_fullDetector/validate_sim/outputs/nPEs_"+mass_point+".root");

	vector<TString> hist_names = loopdir(file);
	
	TH1D *h_avg_nPE = new TH1D("avg_nPE","average nPE vs mass_charge", hist_names.size(), 0 , hist_names.size() );
	
	int bin_tmp=1;
	for(auto const &hist : hist_names){
		TH1D *h_tmp = (TH1D*)file->Get(hist);
		if ( h_tmp->GetEntries() == 0 ) continue;
		float fit_min	= h_tmp->GetMean() - h_tmp->GetRMS();
		float fit_max	= h_tmp->GetMean() + h_tmp->GetRMS();
		int fit_status	= h_tmp->Fit("gaus","Q");
		if ( fit_status >= 0){
			TFitResultPtr r	= h_tmp->Fit("gaus","SRMQ","", fit_min,fit_max);
			//cout << "mean: " << r->Parameter(1) << endl;
			//cout << "sigma: " << r->Parameter(2) << endl;
			h_avg_nPE->SetBinContent( bin_tmp 	, r->Parameter(1) );
			h_avg_nPE->SetBinError( bin_tmp 	, r->Parameter(2) );
			string bin_label = hist.Data();
			bin_label = bin_label.substr(bin_label.find("q"));
			TString bin_label_v1 = bin_label;
			h_avg_nPE->GetXaxis()->SetBinLabel(bin_tmp, bin_label_v1);
		}
		bin_tmp++;
	}

	TCanvas *c = new TCanvas("c", "canvas", 800, 600);
	c->cd();
	c->SetGridy();
	//c->SetLogy();
	h_avg_nPE->SetStats(0);
	//h_avg_nPE->Fit("pol1","F");
	h_avg_nPE->Draw();
	h_avg_nPE->LabelsOption("a","X");
	c->SaveAs(out_dir+"avg_nPE.png");
	c->SaveAs(out_dir+"avg_nPE.pdf");
	c->SetLogy();
	c->SaveAs(out_dir+"avg_nPE_logy.png");
	c->SaveAs(out_dir+"avg_nPE_logy.pdf");
}

