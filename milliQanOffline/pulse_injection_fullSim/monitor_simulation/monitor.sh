#! /bin/bash
#start of processing code
DRS_dir="/net/cms26/cms26r0/milliqan/milliqanOffline/DRS_fullSim/"
GEANT_dir="/net/cms25/cms25r5/fsetti/milliQan_files/Geant_fullDetector/"


function init_monitorFiles {

	cd $DRS_dir
	grid=($(ls -d m*))

	for grid_point in "${grid[@]}"
	do
		cd $grid_point	
		rm monitor_file.txt
		touch monitor_file.txt
		file_list=($(ls DRS4layers_output*))
	
		for file in "${file_list[@]}"
		do
			if ! grep -Fxq "$file" monitor_file.txt
			then
				echo "$file" >> monitor_file.txt
			fi
		done
	
		cd ..
	done
}


function check_monitorFiles {

	cd $DRS_dir
	grid_point=$1
	cd $grid_point	
	touch monitor_file.txt
	file=$2
	if ! grep -Fxq "$file" monitor_file.txt
	then
		echo "$file" >> monitor_file.txt
		proceed=1
	else
		proceed=0
	fi
	cd -
}

init_monitorFiles
