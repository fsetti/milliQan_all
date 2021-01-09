#! /bin/bash

RANDOM=89
search_dir="/homes/fsetti/milliQan_files/Geant/raw_slimTree"
input_dir="/net/cms26/cms26r0/milliqan/milliqanOffline/mixTrees/franny/"
iter=323
for entry in "$search_dir"/output_9*
#for entry in "$search_dir"/output_1.*
do
	file_name="4layers_""$iter"
	run_idx=$((1 + RANDOM % 12))
	run=$(sed "${run_idx}q;d" zeroBias_runs.txt)
	file_num="$(($RANDOM % 77 +1))"
	echo '	Input File: '"$entry"'  '
	echo '                                              '
	echo '----------------   --- --- ---  --------------'
	echo '----------------    ---- ----   --------------'
	echo '----------------      ----      --------------'
	echo '	Running mixEvents!                   '
	echo '	Zero Bias Run: '"$run"'              '
	echo '----------------      ----      --------------'
	echo '----------------    ---- ----   --------------'
	echo '----------------   --- --- ---  --------------'
	echo '                                              '

	./mixEvents.exe $run "$entry" "$file_name" $file_num 0 -1 $RANDOM &> /dev/null
#	./mixEvents.exe $run "$entry" "test" $file_num 0 -1 $RANDOM # &> /dev/null

	echo '                                              '
	echo '----------------   --- --- ---  --------------'
	echo '----------------    ---- ----   --------------'
	echo '----------------      ----      --------------'
	echo '-----------   Running make_tree!  -----------'
	echo '----------------      ----      --------------'
	echo '----------------    ---- ----   --------------'
	echo '----------------   --- --- ---  --------------'
	echo '                                              '

	python runMakeTree.py -d  -i "$input_dir""$file_name"".root" -e ./make_tree_4layers_Franny &> /dev/null 
#	python runMakeTree.py -d  -i "$input_dir""test.root" -e ./make_tree_4layers_Franny # &> /dev/null 

	iter=$(($iter+1)) 
done
