#! /bin/bash

#compile executables
#./compileMix.sh
#./compile.sh make_tree_fullSim

#start of processing code
RANDOM=$(date +%s)
input_dir="/net/cms26/cms26r0/milliqan/milliqanOffline/mixTrees/fullSim/"
output_dir="/net/cms26/cms26r0/milliqan/milliqanOffline/DRS_fullSim/"

input_dir_tmp="/data1/milliqan/mixTrees/"
output_dir_tmp="/data1/milliqan/DRS_fullSim/"
geant_dir="/data1/milliqan/Geant_output/"

cd $geant_dir
grid=($(ls -d m*))
cd -

cms_server=4
n_server=10

function check_monitorFiles {
	grid_point_monitor=$1
	touch $output_dir/$grid_point_monitor/monitor_file.txt
	file_monitor=$2
	file_monitor="DRS""$2""_v21.root"
	if ! grep -Fxq "$file_monitor" $output_dir/$grid_point_monitor/monitor_file.txt
	then
		echo "$file_monitor" >> $output_dir/$grid_point_monitor/monitor_file.txt
		#echo "adding file: ""$file_monitor"" to the monitor list."
		proceed=1
	else
		#echo "file: ""$file_monitor" " already exists in " "$output_dir""/""$grid_point_monitor""."
		proceed=0
	fi
}


function run_make_tree {
	grid_iter=1
	for grid_point in "${grid[@]}"
	do	
		if [ $grid_iter -ge $1 -a $grid_iter -lt $2 ]
		then

			mkdir -p $input_dir$grid_point
			mkdir -p $output_dir$grid_point

			mkdir -p $input_dir_tmp$grid_point
			mkdir -p $output_dir_tmp$grid_point
			iter=1
			for entry in "$geant_dir""$grid_point"/output*
			do
				file_name="4layers_output_""$iter"
				#if [ $(( $iter % $n_server )) -eq $cms_server ] 
				#then
					check_monitorFiles $grid_point $file_name
					if [ $proceed -eq 1 ] 
					then
						#echo "running on: ""$file_name" " in ""$grid_point""."
						run_idx=$((1 + RANDOM % 12))
						run=$(sed "${run_idx}q;d" zeroBias_runs.txt)
						./mixEvents_fullSim.exe $run "$entry" "$file_name" "$grid_point" 1 0 -1  &> /dev/null
						python runMakeTree.py -d  -i "$input_dir_tmp""$grid_point""/""$file_name"".root" -g "$grid_point" -e ./make_tree_fullSim  &> /dev/null #&

						rm "$input_dir_tmp""$grid_point""/""$file_name"".root"
						mv "$output_dir_tmp""$grid_point""/DRS""$file_name""_v21.root" "$output_dir""$grid_point""/DRS""$file_name""_v21.root"
					fi
				#fi
				iter=$(($iter+1))
			done
		fi
	
		grid_iter=$(($grid_iter+1))
	done
}


#set up loop to submit multiple jobs in parallel
increment=5
#iteration=1

#jobs already run 0-32
#mil10 is running 0-56, for milX < mil10 they are running jobs 0-64 (may overload cores)
#iteration=8
#nJobs=128

#nJobs=160	#submit 2 parallel jobs
#nJobs=176	#submit 3 parallel jobs
#nJobs=192	#submit 4 parallel jobs
#nJobs=208	#submit 5 parallel jobs
#nJobs=224	#submit 6 parallel jobs
#nJobs=240	#submit 7 parallel jobs
#iteration=128
#nJobs=256

#iteration=256
#nJobs=288	#submit 2 parallel jobs
#nJobs=304	#submit 3 parallel jobs
#nJobs=320	#submit 4 parallel jobs
#nJobs=336	#submit 5 parallel jobs
#nJobs=352	#submit 6 parallel jobs
#nJobs=368	#submit 7 parallel jobs
iteration=1
nJobs=400

while [ $iteration -lt $nJobs ]
do
    next_iteration=$(($iteration+$increment))
    run_make_tree $iteration $next_iteration &
    iteration=$next_iteration
done

#run_make_tree 1 2
