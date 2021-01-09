#! /bin/bash
#start of processing code
postProc_dir="/net/cms25/cms25r5/fsetti/milliQan_files/Geant_slabDetector/"

function init_monitorFiles {

	cd $postProc_dir
	grid=($(ls -d m*))

	for grid_point in "${grid[@]}"
	do
		cd $grid_point	
		rm monitor_file.txt
		touch monitor_file.txt
		file_list=($(ls output*))
	
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

init_monitorFiles
