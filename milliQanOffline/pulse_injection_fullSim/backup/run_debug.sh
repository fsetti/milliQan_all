#! /bin/bash

#compile executables
#./compileMix.sh
#./compile_debug.sh make_tree_fullsim_debug

#start of processing code
RANDOM=$(date +%s)
input_dir="/net/cms26/cms26r0/milliqan/milliqanOffline/mixTrees/fullSim/"
output_dir="/net/cms26/cms26r0/milliqan/milliqanOffline/DRS_fullSim/"
geant_dir="/net/cms25/cms25r5/fsetti/milliQan_files/Geant_fullDetector/"

cd $geant_dir
grid=($(ls -d m*))
cd -

grid_iter=1

function run_make_tree {
	for grid_point in "${grid[@]}"
	do
	
		if [ $grid_iter -ge $1 -a $grid_iter -lt $2 ]
		then
			mkdir -p $input_dir$grid_point
			mkdir -p $output_dir$grid_point
			iter=1
			for entry in "$geant_dir""$grid_point"/output*
			do
				file_name="4layers_output_""$iter"
				run_idx=$((1 + RANDOM % 12))
				run=$(sed "${run_idx}q;d" zeroBias_runs.txt)
				#echo "Using zero Bias run: " $run
				#./mixEvents_fullSim.exe $run "$entry" "$file_name" "$grid_point" 1 0 -1  #&> /dev/null
				python runMakeTree.py -d  -i "$input_dir""$grid_point""/""$file_name"".root" -g "$grid_point" -e ./make_tree_fullSim_debug  #&> /dev/null #&
				#rm "$input_dir""$grid_point""/""$file_name"".root"
				exit			
				iter=$(($iter+1))
			done
		fi
	
		grid_iter=$(($grid_iter+1))
	done
}

#set up loop to submit multiple jobs in parallel
increment=15

#iteration=0
#nJobs=128
#iteration=128
#nJobs=256
#iteration=256
#nJobs=384
#while [ $iteration -le $nJobs ]
#do
#    next_iteration=$(($iteration+$increment))
#    run_make_tree $iteration $next_iteration &
#    iteration=$next_iteration
#done

run_make_tree 3 4 
