import re
import sys
import pandas as pd


mass 	= sys.argv[1]
charge	= sys.argv[2]
proc	= sys.argv[3]
nFile	= sys.argv[4]
try:
	nFile	= re.search(r"\d+(\.\d+)?", nFile)
	nFile	= nFile.group(0)
except:
	nFile	= '1'

#absolute madness
check_mass	= re.search(r"\d+(\p\d+)?", mass)
check_mass	= str(check_mass.group(0))
prefix = 'lowmass'
if float(check_mass.replace("p",".")) >= 7. :
	prefix = 'v8_v1_save2m_full'

inFile = '/net/cms27/cms27r0/schmitz/milliqanTrajectories/hadoop/cms/store/user/ryan/milliqan/milliq_mcgen/mcp_txt/'+prefix+'/'+str(mass)+'/'+str(charge)+'/'+str(proc)+'/output_'+str(nFile)+'.txt'

def main():
	weights = pd.read_csv( inFile ,sep=' ',header=None,usecols=[10],dtype=str,index_col=False)

	weight = float(weights.iloc[0])
	print weight

if __name__ == "__main__":
	main()
