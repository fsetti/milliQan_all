#! /bin/bash
geant_dir="/net/cms27/cms27r0/schmitz/mcpSignal_fullsim/hadoop/cms/store/user/ryan/beammcp_fullSim_v1/mcp_v8_v1_save2m_full_fullSim_v1/dataStore"
geant_grid=($(ls $geant_dir))
out_dir="/homes/fsetti/milliQan_files/Geant_fullDetector/"
grid_point=1
cd $geant_dir
charge_grid=($(ls -d q_*))
cd -

for charge in "${charge_grid[@]}"
do
	full_out_dir=$out_dir"_"$charge
	mkdir -p $full_out_dir
	iter=1
	proc_grid=($(ls $geant_dir/$charge))
	for proc in "${proc_grid[@]}"
	do
		fileNames=($(ls $geant_dir/$charge/$proc))
		for file in "${fileNames[@]}"
		do
			./make_ntuple $full_out_dir $iter $geant_dir/$charge/$proc/$file
			iter=$(($iter+1))
		done
	done
done
