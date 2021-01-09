{
	gROOT->ProcessLine(".L SlimTree/SlimTree.cc+");
	gROOT->ProcessLine(".L selection.C+");
	
	TChain *ch = new TChain("Events");
	ch->Add("/net/cms25/cms25r5/fsetti/milliQan_files/Geant_fullDetector/m_0p14_q_0p002/output_*.root");
	selection( ch  );	
	cout << "-------------------------------------" << endl;
}
