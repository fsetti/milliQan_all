{
	gROOT->ProcessLine(".L selection.C+");
	gROOT->ProcessLine(".L ../scripts/plot_functions.C+");
	
	string mass;
	mass = "m_10p0_";
	TString out_name, grid_point;
	out_name = "outputs/nPEs_"+mass+".root";
	TChain *ch;
	vector<TString> grid_list = list_files("/net/cms26/cms26r0/milliqan/milliqanOffline/DRS_fullSim" , mass);

	/*
	gSystem->Exec(Form("rm -rf %s &>/dev/null",out_name.Data()));
	unsigned int iter = 0;
	while ( iter < grid_list.size() ){	
		grid_point = grid_list.at(iter);
		ch = new TChain("t");
		ch->Add("/net/cms26/cms26r0/milliqan/milliqanOffline/DRS_fullSim/"+grid_point+"/DRS4layers_output_*_v21.root");
		cout << "\n Processing: " << grid_point << endl;
		selection( ch , out_name, grid_point );	
		cout << "-------------------------------------" << endl;
		iter++;
	}
	*/

	plot_avg_nPE(mass);
}
