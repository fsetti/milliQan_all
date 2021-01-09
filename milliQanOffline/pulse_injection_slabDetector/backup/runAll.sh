#! /bin/bash

#compile executables
#./compileMix.sh
#./compile.sh make_tree_fullsim

#start of processing code
RANDOM=$(date +%s)
input_dir="/net/cms26/cms26r0/milliqan/milliqanOffline/mixTrees/fullSim/"
output_dir="/net/cms26/cms26r0/milliqan/milliqanOffline/DRS_fullSim/"
geant_dir="/net/cms25/cms25r5/fsetti/milliQan_files/Geant_fullDetector/"

cd $geant_dir
grid=($(ls -d m*))
cd -

grid_iter=1


function check_monitorFiles {
	grid_point_monitor=$1
	touch $output_dir/$grid_point_monitor/monitor_file.txt
	file_monitor=$2
	file_monitor="DRS""$2""_v21.root"
	if ! grep -Fxq "$file_monitor" $output_dir/$grid_point_monitor/monitor_file.txt
	then
		echo "$file_monitor" >> $output_dir/$grid_point_monitor/monitor_file.txt
		echo "adding file: ""$file_monitor"" to the monitor list."
		proceed=1
	else
		echo "file: ""$file_monitor" " already exists in " "$output_dir""/""$grid_point_monitor""."
		proceed=0
	fi
}


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
				check_monitorFiles $grid_point $file_name
				if [ $proceed -eq 1 ] 
				then
					echo "running on: "i"$file_name"
					#run_idx=$((1 + RANDOM % 12))
					#run=$(sed "${run_idx}q;d" zeroBias_runs.txt)
					#echo "Using zero Bias run: " $run
					#./mixEvents_fullSim.exe $run "$entry" "$file_name" "$grid_point" 1 0 -1  #&> /dev/null
					#python runMakeTree.py -d  -i "$input_dir""$grid_point""/""$file_name"".root" -g "$grid_point" -e ./make_tree_fullSim  #&> /dev/null #&
					#rm "$input_dir""$grid_point""/""$file_name"".root"
				fi
			
				iter=$(($iter+1))
			done
		fi
	
		grid_iter=$(($grid_iter+1))
	done
}


#set up loop to submit multiple jobs in parallel
increment=3

#iteration=0
#nJobs=128
#iteration=128
#nJobs=256
iteration=370
nJobs=389
#iteration=320
#nJobs=384
#while [ $iteration -le $nJobs ]
#do
#    next_iteration=$(($iteration+$increment))
#    run_make_tree $iteration $next_iteration &
#    iteration=$next_iteration
#done

run_make_tree 2 4
