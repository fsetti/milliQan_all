#! /bin/bash
#start of processing code
DRS_dir="/net/cms26/cms26r0/milliqan/milliqanOffline/DRS_fullSim/"
GEANT_dir="/net/cms25/cms25r5/fsetti/milliQan_files/Geant_fullDetector/"


function init_monitorFiles {

	cd $DRS_dir
	grid=($(ls -d m*))

	for grid_point in "${grid[@]}"
	do
		if [ "$grid_point" == "m_0p14_q_0p001" ] then
		cd $grid_point	
		touch monitor_file.txt
		file_list=($(ls DRS4layers_output*))

		for file in "${file_list[@]}"
		do
			echo "$file \n" >> monitor_file.txt
		done

		cd -
		fi
	done
}

init_monitorFiles
