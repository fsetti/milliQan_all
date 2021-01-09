#! /bin/bash
#start of processing code

geant_dir="/net/cms25/cms25r5/fsetti/milliQan_files/Geant_slabDetector/"
loc_dir="/data1/milliqan/Geant_output/"

function copy_Files {

	iter=1
	cd $geant_dir
	grid=($(ls -d m*))

	for grid_point in "${grid[@]}"
	do
		if [ $iter -ge $1 -a $iter -lt $2 ] 
		then
			cp -r $geant_dir$grid_point $loc_dir
		fi

		iter=$(($iter+1))
	done
}

copy_Files 200 350
