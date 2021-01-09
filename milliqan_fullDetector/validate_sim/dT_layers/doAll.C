{
	gROOT->ProcessLine(".L selection.C+");
	
	TChain *ch = new TChain("t");
	ch->Add("/net/cms26/cms26r0/milliqan/milliqanOffline/DRS_fullSim/m_0p14_q_0p002/DRS4layers_output_1_v21.root");
	selection( ch  );	
	cout << "-------------------------------------" << endl;
}
