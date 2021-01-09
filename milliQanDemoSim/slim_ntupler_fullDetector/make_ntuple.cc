#include <iostream>
#include <algorithm>
#include <numeric>
#include <string>
#include <cmath>

#include "mqROOTEvent.hh"
#include "mqPMTRHit.hh"
#include "mqScintRHit.hh"
#include "mqMuonTrack.hh"
#include "SlimTree/SlimTree.h"
#include "utils/TrackFinding.h"

#include "TRandom.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

bool isscint(std::string volname);

// constants for conversion from sim coordinates
const float rotAngle = 43.1 * M_PI / 180.0;
const float xOffset = -2.5023;
const float zOffset = -2.7381;
const bool APPLY_CALIB = true;

// bar, slab, and panel channel numbers
const int NLAYERS		= 4;
const int NMODULES		= 54;
const int NBARSPERMOD	= 4;
const int NBARSPERLAYER	= 216;
const int NBARS			= NBARSPERMOD * NMODULES * NLAYERS;
const int NSLABS 		= 4;
const int NPANELS 		= 8;
int bar_chans[NBARS];
const int slab_chans[NSLABS] = {886,887,888,889};
const int panel_chans[NPANELS] = {873,874,875,876, 881,882,883,884};

// channel time calibrations (manually extracted by finding peaks of uncalibrated time distributions)
float time_calibs[nchans] = {0.};

//	pmt_calibrations are given by the ratio of data / mc nPE for a cosmic muon
//	use calibration given by channel 25 for all channels
const float pmt_calibration = 6600. / 12000;

// these corrections have been observed in channel_firstTime for uncalibrated events
// based on the first pmt hits 'chan_firstTime[ch]' in /net/cms27/cms27r0/schmitz/mcpSignal_fullsim/hadoop/cms/store/user/ryan/beammcp_fullSim_v1/mcp_v8_v1_save2m_full_fullSim_v1/dataStore/m_0p14/q_0p01/eta/MilliQan_3.root
const float dt_min_l1	= 13.5 ;
const float dt_max_l1	= 17.5 ;
const float dt_min_l2	= 16.5 ;
const float dt_max_l2	= 20.0 ;
const float dt_min_l3	= 20.0 ;
const float dt_max_l3	= 23.0 ;
const float dt_min_l4	= 22.0 ;
const float dt_max_l4	= 26.0 ;
const int mod_per_col	= 9;	//modules per column

void load_arrays(){
	//set-up bar channel array
	for (unsigned int i=0; i<NBARS; i++){
		bar_chans[i]	= i+1;
	}

	//different time corrections are applied on a per-layer and 
	//per-module basis (module==4 channels, layer==216 channels)
	vector<float> time_calibs_l1, time_calibs_l2, time_calibs_l3, time_calibs_l4;
	float step = ( dt_max_l1 - dt_min_l1 ) / ( mod_per_col - 1 );
	float time_cal = dt_max_l1;
	for (unsigned int i=0; i<mod_per_col; i++){
		time_calibs_l1.push_back( time_cal );
		time_cal -= step;
	}
	step = ( dt_max_l2 - dt_min_l2 ) / ( mod_per_col - 1 );
	time_cal = dt_max_l2;
	for (unsigned int i=0; i<mod_per_col; i++){
		time_calibs_l2.push_back( time_cal );
		time_cal -= step;
	}
	step = ( dt_max_l3 - dt_min_l3 ) / ( mod_per_col - 1 );
	time_cal = dt_max_l3;
	for (unsigned int i=0; i<mod_per_col; i++){
		time_calibs_l3.push_back( time_cal );
		time_cal -= step;
	}
	step = ( dt_max_l4 - dt_min_l4 ) / ( mod_per_col - 1 );
	time_cal = dt_max_l4;
	for (unsigned int i=0; i<mod_per_col; i++){
		time_calibs_l4.push_back( time_cal );
		time_cal -= step;
	}

	for (unsigned int i=1; i<=NBARS; i++){
		if ( (i-1) / NBARSPERLAYER == 0 ){
			time_calibs[i] = time_calibs_l1.at( ( (i-1) / 4 ) % mod_per_col );
		}
		if ( (i-1) / NBARSPERLAYER == 1 ){
			time_calibs[i] = time_calibs_l2.at( ( (i-1) / 4 ) % mod_per_col );
		}
		if ( (i-1) / NBARSPERLAYER == 2 ){
			time_calibs[i] = time_calibs_l3.at( ( (i-1) / 4 ) % mod_per_col );
		}
		if ( (i-1) / NBARSPERLAYER == 3 ){
			time_calibs[i] = time_calibs_l4.at( ( (i-1) / 4 ) % mod_per_col );
		}
	}
	//set up time calibrations for slabs
	time_calibs[886]	= 34;
	time_calibs[887]	= 22;
	time_calibs[888]	= 40;
	time_calibs[889]	= 29;
	//missing time calibrations for panels
}

int main(int argc, char** argv){

	load_arrays();
	
    if(argc < 4){
        std::cout << "usage: " << argv[0] << " <outdir> <file_id> <file1> [<file2> ...]\n";
        return -1;
    }

    gRandom->SetSeed(42);

    char* outdir = argv[1];
    char* file_id = argv[2];
    char* weight_str = argv[3];
	float evt_weight = stof(weight_str);
    int file_id_int = atoi(file_id);

    mqROOTEvent *revent = new mqROOTEvent();   
    TFile *fout = new TFile(Form("%s/output_%s.root", outdir, file_id), "RECREATE");
    SlimTree s;
    s.Init();

    std::vector<track>* tracks = new std::vector<track>;
    std::map<std::string, int> origin_counts;

    TChain *ch = new TChain("Events");
    for(int i=4; i<argc; i++)
        ch->Add(argv[i]);
    TIter fileIter(ch->GetListOfFiles());
    TFile *currentFile = 0;
    while ( (currentFile = (TFile*)fileIter.Next()) ) {
        std::cout << "running on file: " << currentFile->GetTitle() << std::endl;
        
        TFile *f = TFile::Open(currentFile->GetTitle());
        if(!f or f->IsZombie()){
            std::cout << "Problem with file, skipping!" << std::endl;
            delete f;
            continue;
        }
        TTree *t = (TTree*)f->Get("Events");
        if(!t){
            std::cout << "Problem with tree in file, skipping!" << std::endl;
            delete f;
            continue;
        }
        uint nEvents = t->GetEntries();
        // uint nEvents = 100;

        t->SetBranchAddress("ROOTEvent", &revent);


        for(int ievt=0 ; ievt<nEvents; ievt++){
            s.progress(ievt, nEvents, 20);

            t->GetEntry(ievt);
            s.Reset();

            // compile a list of tracks so that we can trace track ancestry
            populate_tracks(revent, tracks);
            
            bool ISMU = false;
            mqMuonTrack* mutrack = revent->GetMuonTrack(1);
            if(mutrack){
                ISMU = true;
                s.mu_E0 = mutrack->GetInitialEnergy() / 1000;
                float x0 = mutrack->GetFirstPositionX();
                float y0 = mutrack->GetFirstPositionY();
                float z0 = mutrack->GetFirstPositionZ();
                s.mu_x0 = ((x0-xOffset)/cos(rotAngle) - (z0-zOffset)/sin(rotAngle)) / 2;
                s.mu_y0 = y0;
            }
            mqMCPTrack* mcptrack = revent->GetMCPTrack(1);
            if(mcptrack){
                s.mu_E0 = mcptrack->GetInitialEnergy() / 1000;
                float x0 = mcptrack->GetFirstPositionX();
                float y0 = mcptrack->GetFirstPositionY();
                float z0 = mcptrack->GetFirstPositionZ();
                s.mu_x0 = ((x0-xOffset)/cos(rotAngle) - (z0-zOffset)/sin(rotAngle)) / 2;
                s.mu_y0 = y0;
            }

            // initialize values
            for(unsigned int ich =0; ich<nchans; ich++){
                s.chan_nPE[ich] = 0;
                s.chan_fracMuon[ich] = -1;
                s.chan_fracElScint[ich] = -1;
                s.chan_fracElRock[ich] = -1;
                s.chan_fracElAl[ich] = -1;
                s.chan_fracElPb[ich] = -1;
                s.chan_fracOther[ich] = -1;
                s.chan_firstTime[ich] = 9999999.;
                s.chan_meanTime[ich] = 0.0;
                for(int j=0; j<100; j++)
                    s.chan_PEtimes[ich][j] = 0;
                //channel type: 0=slab, 1=bar, 2=panel
                s.chan_type[ich] = -1;
                for(int j=0; j<max(NBARS, max(NPANELS, NSLABS)); j++){
                    if(j < NSLABS && ich == slab_chans[j]){
                        s.chan_type[ich] = 0;
                        break;
                    }else if(j < NBARS && ich == bar_chans[j]){
                        s.chan_type[ich] = 1;
                        break;
                    }else if(j < NPANELS && ich == panel_chans[j]){
                        s.chan_type[ich] = 2;
                        break;
                    }
                }
                s.chan_timeCalib[ich] = time_calibs[ich];
                s.chan_muDist[ich] = -1;
                s.chan_muTime[ich] = -1;
            }

            s.fileID = file_id_int;
            s.orig_evt = ievt;
            //s.scale1fb = revent->GetEventWeight() * (ISMU ? 1000 : 1);
            s.scale1fb = evt_weight * (ISMU ? 1000 : 1);
            s.procID = revent->GetProcessID();

            std::map<int,int> unique_parents[nchans];
            std::pair<float, int> first_parent[nchans];
            std::pair<int, int> max_parent[nchans];
            for(unsigned int i=0; i<nchans; i++){
                first_parent[i] = {9999, 0};
                max_parent[i] = {0, 0};
            }

            // loop over PMT hits and store relevant information
            mqPMTRHitVector* pes = revent->GetPMTRHits();
            for(mqPMTRHit *hit : *pes){ 
                //int mqchan = channel_map[hit->GetPMTNumber()];
                int mqchan = hit->GetPMTNumber();
				if ( mqchan < 0 ) continue;
                if(APPLY_CALIB && gRandom->Rndm() > pmt_calibration )	//only apply correction to bars
                    continue;
                s.chan_nPE[mqchan]++;
                
                //timing information
                float time = hit->GetFirstHitTime() - ( time_calibs[mqchan] - 10.0 );	//initially, the offset was 7ns rahter than 10ns
                int time_index = std::max(0, std::min(159, int(time/0.625)));
                s.chan_PEtimes[mqchan][time_index] += 1;
                s.chan_firstTime[mqchan] = std::min(s.chan_firstTime[mqchan], time);
                s.chan_meanTime[mqchan] += time;

                // find the unique hit parent IDs for this channel
                int pid = hit->GetParentID();
                if(unique_parents[mqchan].find(pid) == unique_parents[mqchan].end())
                    unique_parents[mqchan][pid] = 0;
                unique_parents[mqchan][pid] += 1;

                if(time < first_parent[mqchan].first)
                    first_parent[mqchan] = {time, pid};
            }

            // trace track ancestry
            for(int ich=0; ich<nchans; ich++){
                std::map<int,int> source_encodings;
                for(std::pair<int,int> up : unique_parents[ich]){
                    int pid = up.first;
                    int count = up.second;
                    if(count > max_parent[ich].first)
                        max_parent[ich] = {count, pid};
                    std::vector<track> path = get_path(pid, tracks);
                    string source = "UNDEFINED";
                    int enc = -1;
                    if(path.size() > 0){
                        track source_track = path.back();
                        if(path.size() > 1)
                            source_track = path.at(path.size()-2);
                        track_info ti = get_track_info(source_track, revent);
                        if(isscint(ti.start_volume) && ich == ti.start_copyNo){
                            // if muon produces an electron in the same chan, count as muon
                            source_track = path.at(path.size()-1);
                            ti = get_track_info(source_track, revent);
                        }
                        source = ti.type + "_" + ti.start_volume + "_" + ti.prod_process;
                        enc = ti.source_encoding;
                    }
                    if(origin_counts.find(source) == origin_counts.end())
                        origin_counts[source] = 0;
                    origin_counts[source] += count;
                    if(source_encodings.find(enc) == source_encodings.end())
                        source_encodings[enc] = 0;
                    source_encodings[enc] += count; 
                }
                if(s.chan_nPE[ich] > 0){
                    s.chan_fracMuon[ich] = source_encodings[0] / float(s.chan_nPE[ich]);
                    s.chan_fracElScint[ich] = source_encodings[1] / float(s.chan_nPE[ich]);
                    s.chan_fracElRock[ich] = source_encodings[2] / float(s.chan_nPE[ich]);
                    s.chan_fracElAl[ich] = source_encodings[3] / float(s.chan_nPE[ich]);
                    s.chan_fracElPb[ich] = source_encodings[4] / float(s.chan_nPE[ich]);
                    s.chan_fracOther[ich] = source_encodings[-1] / float(s.chan_nPE[ich]);
                }
                // get source info for max PE
                if(max_parent[ich].second > 0){
                    std::vector<track> path = get_path(max_parent[ich].second, tracks);
                    if(path.size() > 0){
                        int ip;
                        for(ip=0; ; ip++){
                            track_info ti = get_track_info(path.at(ip), revent);
                            if(!isscint(ti.start_volume) || ich != ti.start_copyNo)
                                break;                        
                        }
                        s.chan_maxSourceType[ich] = path.at(ip).type;
                        s.chan_maxSourceE0[ich] = path.at(ip).init_E;
                        s.chan_maxSourceNPE[ich] = max_parent[ich].first;
                    }
                }
            }
            
            // normalize the mean, and set default values for 0-hit events
            for(int ich=0; ich<nchans; ich++){
                if(s.chan_nPE[ich] == 0){
                    s.chan_firstTime[ich] = -1;
                    s.chan_meanTime[ich] = -1;
                }else{
                    s.chan_meanTime[ich] /= s.chan_nPE[ich];
                }
            }

            // handle scintHits
            mqScintRHitVector* scinthits = revent->GetScintRHits();
            for(mqScintRHit *hit : *scinthits){
                int ch = hit->GetCopyNo();
                s.chan_muDist[ch] = hit->GetTrackLength();
                s.chan_muTime[ch] = hit->GetHitTime();
            }

            // set the mcTruth booleans
            s.mcTruth_nMuBarHits = 0;
            s.mcTruth_nMuSlabHits = 0;
            s.mcTruth_nMuPanelHits = 0;
            s.mcTruth_noChan = true;
            for(int ich=0; ich<nchans; ich++){
                float muDist = s.chan_muDist[ich];
                bool ishit = (muDist > 0);
                s.mcTruth_noChan &= !ishit;
                if(s.chan_type[ich] == 0 || ich == 867 || ich == 868){	//add slabs 867 and 868
                    s.mcTruth_nMuSlabHits += int(ishit);
				}
                else if(s.chan_type[ich] == 1){
                    s.mcTruth_nMuBarHits += int(ishit);
				}
                else if(s.chan_type[ich] == 2){
                    s.mcTruth_nMuPanelHits += int(ishit);
				}
				//check if hits in four bars in a straight line
				if ( !s.mcTruth_fourBarLine &&  ich <= NBARS / 4 ){
					if ( s.chan_muDist[ich+216] 	> 0 
					  && s.chan_muDist[ich+216*2]	> 0 
					  && s.chan_muDist[ich+216*3]	> 0 
					  && s.chan_muDist[ich] > 0 ) {
            			s.mcTruth_fourBarLine = true;
					}
				}
            }
            // redundant, but keep for consistency reasons
            s.mcTruth_twoSlab = (s.mcTruth_nMuSlabHits >= 2);	//use 2 instead of == NSLABS since two PMTs on slabs not working
            s.mcTruth_noBar = (s.mcTruth_nMuBarHits == 0);
			/*
            // check cosmic paths
            s.mcTruth_verticalCosmic = 0;
            for(int ipath=0; ipath<cosmicPaths.size(); ipath++){
                bool pass = true;
                for(int ch : cosmicPaths.at(ipath))
                    if(s.chan_muDist[ch] <= 0)
                        pass = false;
                if(pass){
                    s.mcTruth_verticalCosmic = ipath+1;
                    break;
                }
            }
			*/
            // check hits at reco level
            bool recoHits = false;
            for(int ich=0; ich<nchans; ich++){
				if ( s.chan_nPE[ich] > 0 ){
					recoHits = true;
					break;
				}
			}

			if ( s.mcTruth_noChan && !recoHits) continue;

            s.Fill();
        }

        f->Close();
        delete f;
        delete currentFile;
    }

    // print PE origin counts
    std::cout << "Sources of PEs\n-------------------------------------------\n";
    std::vector<std::pair<std::string, int> > vec;
    std::copy(origin_counts.begin(), origin_counts.end(), std::back_inserter<std::vector<std::pair<std::string, int> > >(vec));
    std::sort(vec.begin(), vec.end(), 
              [](const std::pair<std::string, int> l, const std::pair<std::string, int> r) {
                  return l.second>r.second;
              });
    for(std::pair<std::string, int> oc : vec){
        std::cout << oc.second << "\t" << oc.first << std::endl;
    }
        
    s.Write(fout);
    fout->Close();

    return 0;

}

bool isscint(std::string volname){
    return volname.find("plScin") != std::string::npos ||
        volname.find("slab_physic") != std::string::npos ||
        volname.find("panel_physic") != std::string::npos;
}
