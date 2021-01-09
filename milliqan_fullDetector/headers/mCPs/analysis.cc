// Make milliqan validation plots. D. Stuart, Oct. 2017.
// adapted F. Setti, Jul. 2019.
#include "analysis.h"
#include "/net/cms25/cms25r5/fsetti/scripts/tqdm/tqdm.h"
 
bool sort_wrt_channel( vector<float> vec1, vector<float> vec2 ){
        return vec1[0] < vec2[0];
}

bool sort_wrt_height( vector<float> vec1, vector<float> vec2 ){
        return vec1[1] > vec2[1];
}

bool sort_wrt_time( vector<float> vec1, vector<float> vec2 ){
        return vec1[2] < vec2[2];
}

bool sort_wrt_nPE( vector<float> vec1, vector<float> vec2 ){
        return vec1[3] > vec2[3];
}

bool sort_wrt_calTime( vector<float> vec1, vector<float> vec2 ){
        return vec1[4] < vec2[4];
}


void read_list_files(const char *dirname, TChain *chainptr )
{  
    TSystemDirectory dir(dirname, dirname);
    TList *files = dir.GetListOfFiles();
    if (files) {
       TSystemFile *file;
       TString fname;
       TIter next(files);
       while ((file=(TSystemFile*)next())) {
          fname = file->GetName();
          if ( fname.Length()>2 ) {
               TString path;
               path = dirname + TString("/") + fname ;
               chainptr->Add(path);
          }
        }
    }
}

vector<TString> list_files(const char *dirname, string proc )
{
	vector<TString> file_names;
	TSystemDirectory dir(dirname, dirname);
	TList *files = dir.GetListOfFiles();
	if (files) {
		TSystemFile *file;
		string fname;
		TIter next(files);
		while ((file=(TSystemFile*)next())) {
			fname = file->GetName();
			if ( fname.find(proc) != std::string::npos ) file_names.push_back( fname );
		}
	}
	return file_names;
}


bool check_mCP_hit(int row, int col, int chan, vector<int> *pulseChan){
	if ( chan > 864 ) return false;
	else if ( ( chan - 1 ) % 4 == 0 && row != 0 && row != 17 && col != 0 && col != 11 
	  && ( count( pulseChan->begin(), pulseChan->end(), chan+6 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan+43+ 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan+37+ 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan+39+ 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan+2 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan+3 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan+1 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan+7 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan   + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
	  )
	  && ( count( pulseChan->begin(), pulseChan->end(), chan+6 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+43+ 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+37+ 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+39+ 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+2 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+3 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+1 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+7 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan   + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
	  )
	  && ( count( pulseChan->begin(), pulseChan->end(), chan+6 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+43+ 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+37+ 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+39+ 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+2 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+3 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+1 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+7 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan   + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
	  )
	){
		return true;
	}
	else if ( ( chan - 1 ) % 4 == 1 && row != 0 && row != 17 && col != 0 && col != 11 
	  && ( count( pulseChan->begin(), pulseChan->end(), chan+6 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan+5 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan-1 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan+1 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan+2 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan-35+ 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan-37+ 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan-31+ 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan   + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
	  )
	  && ( count( pulseChan->begin(), pulseChan->end(), chan+6 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+5 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-1 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+1 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+2 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-35+ 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-37+ 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-31+ 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan   + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
	  )
	  && ( count( pulseChan->begin(), pulseChan->end(), chan+6 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+5 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-1 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+1 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+2 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-35+ 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-37+ 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-31+ 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan   + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
	  )
	){
		return true;
	}
	else if ( ( chan - 1 ) % 4 == 2 && row != 0 && row != 17 && col != 0 && col != 11 
	  && ( count( pulseChan->begin(), pulseChan->end(), chan-2 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan+35+ 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan+37+ 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan+31+ 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan-6 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan-5 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan+1 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan-1 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan   + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
	  )
	  && ( count( pulseChan->begin(), pulseChan->end(), chan-2 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+35+ 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+37+ 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+31+ 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-6 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-5 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+1 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-1 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan   + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
	  )
	  && ( count( pulseChan->begin(), pulseChan->end(), chan-2 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+35+ 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+37+ 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+31+ 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-6 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-5 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan+1 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-1 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan   + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
	  )
	){
		return true;
	}
	else if ( ( chan - 1 ) % 4 == 3 && row != 0 && row != 17 && col != 0 && col != 11 
	  && ( count( pulseChan->begin(), pulseChan->end(), chan-2 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan-3 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan-1 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan-7 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan-6 + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan-43+ 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan-37+ 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan-39+ 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
		|| count( pulseChan->begin(), pulseChan->end(), chan   + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) ) 
	  )
	  && ( count( pulseChan->begin(), pulseChan->end(), chan-2 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-3 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-1 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-7 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-6 + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-43+ 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-37+ 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-39+ 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan   + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
	  )
	  && ( count( pulseChan->begin(), pulseChan->end(), chan-2 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-3 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-1 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-7 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-6 + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-43+ 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-37+ 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan-39+ 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
		|| count( pulseChan->begin(), pulseChan->end(), chan   + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
	  )
	){
		return true;
	}
	else return false;
}

bool mCP_hit(int row, int col, int chan, vector<int> *pulseChan){
	
	if ( chan <= 864 
	  && count( pulseChan->begin(), pulseChan->end(), chan + 216 * ( ( (chan-1)/216 +1 ) < 4 ? +1 :-( (chan-1)/216+1 +1 )%4 ) )
	  && count( pulseChan->begin(), pulseChan->end(), chan + 216 * ( ( (chan-1)/216 +2 ) < 4 ? +2 :-( (chan-1)/216+1 +2 )%4 ) )
	  && count( pulseChan->begin(), pulseChan->end(), chan + 216 * ( ( (chan-1)/216 +3 ) < 4 ? +3 :-( (chan-1)/216+1 +3 )%4 ) )
	){
		return true;
	}
	else return check_mCP_hit( row, col, chan, pulseChan ); 
}

vector<vector<float>> select_mCPs( vector<int> *pulseChan, vector<float> *pulseHeight, vector<float> *pulseTime, vector<float> *pulsenPE, vector<float> *pulseTimeCal, vector<int> *pulse_row, vector<int> *pulse_col ){
	vector<vector<float>> preSelPulses;
	for (unsigned int j=0; j<pulseChan->size(); j++){
		/*
		cout << "pulse chan: " << pulseChan->at(j) << endl;
		cout << " iter 1 ch: " << pulseChan->at(j) + 216 * ( ( (pulseChan->at(j)-1)/216 +1 ) < 4 ? +1 :-( (pulseChan->at(j)-1)/216+1 +1 )%4 ) << endl;
		cout << " iter 2 ch: " << pulseChan->at(j) + 216 * ( ( (pulseChan->at(j)-1)/216 +2 ) < 4 ? +2 :-( (pulseChan->at(j)-1)/216+1 +2 )%4 ) << endl;
		cout << " iter 3 ch: " << pulseChan->at(j) + 216 * ( ( (pulseChan->at(j)-1)/216 +3 ) < 4 ? +3 :-( (pulseChan->at(j)-1)/216+1 +3 )%4 ) << endl;
		if ( pulseChan->at(j) <= 864		//check 4 bars in a line
		  && count( pulseChan->begin(), pulseChan->end(), pulseChan->at(j) + 216 * ( ( (pulseChan->at(j)-1)/216 +1 ) < 4 ? +1 :-( (pulseChan->at(j)-1)/216+1 +1 )%4 ) )
		  && count( pulseChan->begin(), pulseChan->end(), pulseChan->at(j) + 216 * ( ( (pulseChan->at(j)-1)/216 +2 ) < 4 ? +2 :-( (pulseChan->at(j)-1)/216+1 +2 )%4 ) )
		  && count( pulseChan->begin(), pulseChan->end(), pulseChan->at(j) + 216 * ( ( (pulseChan->at(j)-1)/216 +3 ) < 4 ? +3 :-( (pulseChan->at(j)-1)/216+1 +3 )%4 ) )
		  && pulseTime->at(j)  <  tMax
		  && pulseTime->at(j) >  tMin
		){
		*/
		if ( mCP_hit(pulse_row->at(j) , pulse_col->at(j), pulseChan->at(j), pulseChan)
		  && pulseTime->at(j)  <  tMax
		  && pulseTime->at(j) >  tMin
		){
		 	preSelPulses.push_back({(float)pulseChan->at(j), pulseHeight->at(j), pulseTime->at(j), pulsenPE->at(j) * nPEcalib, pulseTimeCal->at(j)});
		}
	}

	//only retain first pulse in each channel
	std::sort( preSelPulses.begin(), preSelPulses.end(), sort_wrt_calTime );
	for (unsigned int j=0; j<preSelPulses.size(); j++){
		for (unsigned int k=j+1; k<preSelPulses.size(); k++){
			if ( preSelPulses.at(j)[0] == preSelPulses.at(k)[0] ){
				  //preSelPulses.at(j).at(3)+= preSelPulses.at(k).at(3);
				preSelPulses.erase( preSelPulses.begin() + k );
				k -=1;
			}
		}
	}
	
	//require dT between pulses < dt (15ns)
	for (unsigned int j=0; j<preSelPulses.size(); j++){
		for (unsigned int k=j+1; k<preSelPulses.size(); k++){
			if ( fabs(preSelPulses.at(j)[4] - preSelPulses.at(k)[4]) > dt_mCP ){
				  //preSelPulses.at(j).at(3)+= preSelPulses.at(k).at(3);
				preSelPulses.erase( preSelPulses.begin() + k );
				k -=1;
			}
		}
	}
	if ( preSelPulses.size() != 0 && preSelPulses.size() % 4 == 0 ) return preSelPulses;
	else{
		vector<vector<float>> nullo;
		return nullo;
	}
}


vector<vector<float>> select_mCPs_debug( vector<int> *pulseChan, vector<float> *pulseHeight, vector<float> *pulseTime, vector<float> *pulsenPE, vector<float> *pulseTimeCal, vector<int> *pulse_row, vector<int> *pulse_col ){
	vector<vector<float>> preSelPulses;
	for (unsigned int j=0; j<pulseChan->size(); j++){
		if ( mCP_hit(pulse_row->at(j) , pulse_col->at(j), pulseChan->at(j), pulseChan)
		  && pulseTime->at(j)  <  tMax
		  && pulseTime->at(j) >  tMin
		){
		 	preSelPulses.push_back({(float)pulseChan->at(j), pulseHeight->at(j), pulseTime->at(j), pulsenPE->at(j) * nPEcalib, pulseTimeCal->at(j)});
		}
	}

	//only retain first pulse in each channel
	std::sort( preSelPulses.begin(), preSelPulses.end(), sort_wrt_calTime );
	for (unsigned int j=0; j<preSelPulses.size(); j++){
		for (unsigned int k=j+1; k<preSelPulses.size(); k++){
			if ( preSelPulses.at(j)[0] == preSelPulses.at(k)[0] ){
				  //preSelPulses.at(j).at(3)+= preSelPulses.at(k).at(3);
				preSelPulses.erase( preSelPulses.begin() + k );
				k -=1;
			}
		}
	}

	if ( preSelPulses.size() != 0 && preSelPulses.size() % 4 == 0 ) return preSelPulses;
	else{
		vector<vector<float>> nullo;
		return nullo;
	}
}

vector<vector<float>> select_mCPs_line( vector<int> *pulseChan, vector<float> *pulseHeight, vector<float> *pulseTime, vector<float> *pulsenPE, vector<float> *pulseTimeCal ){
	vector<vector<float>> preSelPulses;
	for (unsigned int j=0; j<pulseChan->size(); j++){
		if ( pulseChan->at(j) <= 864		//check 4 bars in a line
		  && count( pulseChan->begin(), pulseChan->end(), pulseChan->at(j) + 216 * ( ( (pulseChan->at(j)-1)/216 +1 ) < 4 ? +1 :-( (pulseChan->at(j)-1)/216+1 +1 )%4 ) )
		  && count( pulseChan->begin(), pulseChan->end(), pulseChan->at(j) + 216 * ( ( (pulseChan->at(j)-1)/216 +2 ) < 4 ? +2 :-( (pulseChan->at(j)-1)/216+1 +2 )%4 ) )
		  && count( pulseChan->begin(), pulseChan->end(), pulseChan->at(j) + 216 * ( ( (pulseChan->at(j)-1)/216 +3 ) < 4 ? +3 :-( (pulseChan->at(j)-1)/216+1 +3 )%4 ) )
		  && pulseTime->at(j)  <  tMax
		  && pulseTime->at(j) >  tMin
		){
		 	preSelPulses.push_back({(float)pulseChan->at(j), pulseHeight->at(j), pulseTime->at(j), pulsenPE->at(j) * nPEcalib, pulseTimeCal->at(j)});
		}
	}

	//only retain first pulse in each channel
	std::sort( preSelPulses.begin(), preSelPulses.end(), sort_wrt_calTime );
	for (unsigned int j=0; j<preSelPulses.size(); j++){
		for (unsigned int k=j+1; k<preSelPulses.size(); k++){
			if ( preSelPulses.at(j)[0] == preSelPulses.at(k)[0] ){
				  //preSelPulses.at(j).at(3)+= preSelPulses.at(k).at(3);
				preSelPulses.erase( preSelPulses.begin() + k );
				k -=1;
			}
		}
	}

/*	
	//require dT between pulses < dt (15ns)
	for (unsigned int j=0; j<preSelPulses.size(); j++){
		for (unsigned int k=j+1; k<preSelPulses.size(); k++){
			if ( fabs(preSelPulses.at(j)[4] - preSelPulses.at(k)[4]) > dt_mCP ){
				  //preSelPulses.at(j).at(3)+= preSelPulses.at(k).at(3);
				preSelPulses.erase( preSelPulses.begin() + k );
				k -=1;
			}
		}
	}
*/
	if ( preSelPulses.size() != 0 && preSelPulses.size() % 4 == 0 ) return preSelPulses;
	else{
		vector<vector<float>> nullo;
		return nullo;
	}
}

unsigned int pulse_check1( vector<int> *pulseChan, vector<float> *pulseHeight, vector<float> *pulseTime, vector<float> *pulsenPE, vector<float> *pulseTimeCal ){
	vector<vector<float>> preSelPulses;
	for (unsigned int j=0; j<pulseChan->size(); j++){
		if ( pulseChan->at(j) <= 864		//check 4 bars in a line
		  && count( pulseChan->begin(), pulseChan->end(), pulseChan->at(j) + 216 * ( ( (pulseChan->at(j)-1)/216 +1 ) < 4 ? +1 :-( (pulseChan->at(j)-1)/216+1 +1 )%4 ) )
		  && count( pulseChan->begin(), pulseChan->end(), pulseChan->at(j) + 216 * ( ( (pulseChan->at(j)-1)/216 +2 ) < 4 ? +2 :-( (pulseChan->at(j)-1)/216+1 +2 )%4 ) )
		  && count( pulseChan->begin(), pulseChan->end(), pulseChan->at(j) + 216 * ( ( (pulseChan->at(j)-1)/216 +3 ) < 4 ? +3 :-( (pulseChan->at(j)-1)/216+1 +3 )%4 ) )
		  && pulseTime->at(j)  <  tMax
		  && pulseTime->at(j) >  tMin
		){
		 	preSelPulses.push_back({(float)pulseChan->at(j), pulseHeight->at(j), pulseTime->at(j), pulsenPE->at(j) * nPEcalib, pulseTimeCal->at(j)});
		}
	}

	//only retain first pulse in each channel
	std::sort( preSelPulses.begin(), preSelPulses.end(), sort_wrt_calTime );
	for (unsigned int j=0; j<preSelPulses.size(); j++){
		for (unsigned int k=j+1; k<preSelPulses.size(); k++){
			if ( preSelPulses.at(j)[0] == preSelPulses.at(k)[0] ){
				  //preSelPulses.at(j).at(3)+= preSelPulses.at(k).at(3);
				preSelPulses.erase( preSelPulses.begin() + k );
				k -=1;
			}
		}
	}

/*	
	//require dT between pulses < dt (15ns)
	for (unsigned int j=0; j<preSelPulses.size(); j++){
		for (unsigned int k=j+1; k<preSelPulses.size(); k++){
			if ( fabs(preSelPulses.at(j)[4] - preSelPulses.at(k)[4]) > dt_mCP ){
				  //preSelPulses.at(j).at(3)+= preSelPulses.at(k).at(3);
				preSelPulses.erase( preSelPulses.begin() + k );
				k -=1;
			}
		}
	}
*/
	vector<vector<float>> new_pulses;
	for (unsigned int j=0; j<pulseChan->size(); j++){
		if ( pulseChan->at(j) <= 864		//check 4 bars in a line
		  && pulseTime->at(j)  <  tMax
		  && pulseTime->at(j) >  tMin
		){
		 	new_pulses.push_back({(float)pulseChan->at(j), pulseHeight->at(j), pulseTime->at(j), pulsenPE->at(j) * nPEcalib, pulseTimeCal->at(j)});
		}
	}
	std::sort( new_pulses.begin(), new_pulses.end(), sort_wrt_calTime );
	for (unsigned int j=0; j<new_pulses.size(); j++){
		for (unsigned int k=j+1; k<new_pulses.size(); k++){
			if ( new_pulses.at(j)[0] == new_pulses.at(k)[0] ){
				  //new_pulses.at(j).at(3)+= new_pulses.at(k).at(3);
				new_pulses.erase( new_pulses.begin() + k );
				k -=1;
			}
		}
	}
	//require dT between pulses < dt (100ns)
	for (unsigned int j=0; j<new_pulses.size(); j++){
		for (unsigned int k=j+1; k<new_pulses.size(); k++){
			if ( fabs(new_pulses.at(j)[4] - new_pulses.at(k)[4]) > 100 ){
				  //new_pulses.at(j).at(3)+= new_pulses.at(k).at(3);
				new_pulses.erase( new_pulses.begin() + k );
				k -=1;
			}
		}
	}

	if ( preSelPulses.size() != 0 && preSelPulses.size() % 4 == 0 ) return new_pulses.size() ;
	else return 0;
}
