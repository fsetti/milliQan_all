{
	gROOT->ProcessLine(".L selection.C+");
	
	TChain *ch = new TChain("t");
	ch->Add("/net/cms26/cms26r0/milliqan/milliqanOffline/DRS_fullSim/m_10p0_q_0p1/DRS4layers_output_*_v21.root");
	selection( ch  );	
	cout << "-------------------------------------" << endl;
}
