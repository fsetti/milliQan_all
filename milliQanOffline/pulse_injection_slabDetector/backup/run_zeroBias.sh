#! /bin/bash

files_per_output=49
#for i in {1..6}
for i in {7..12}
#for i in {1..1}
do
	fileNumber=1
	run=$(sed "${i}q;d" zeroBias_runs.txt)
	mixRunDir="/net/cms26/cms26r0/milliqan/UX5/Run""$run""_software/"
	nFiles=($(ls $mixRunDir | wc -l ))
	while [ $fileNumber -lt $nFiles ]
	do
		batch=$(($fileNumber / $files_per_output + 1 ))
		echo '-------------   --- --- ---  --------------'
		echo '-------------    ---- ----   --------------'
		echo '-------------      ----      --------------'
		echo '           Mixing Zero Bias Run: '"$run"'  '
		echo '               Batch # : '"$batch"'        '
		echo '-------------      ----      --------------'
		echo '-------------    ---- ----   --------------'
		echo '-------------   --- --- ---  --------------'
		echo '                                           '

		if [ $(($fileNumber + $files_per_output + $files_per_output)) -lt $nFiles ]
		then
			./mixZeroBias_fullSim.exe $run "batch""$batch" $fileNumber $files_per_output &> /dev/null &
		else
			if [ $(($fileNumber + $files_per_output )) -lt $nFiles ]
			then
				./mixZeroBias_fullSim.exe $run "batch""$batch" $fileNumber $files_per_output
			else
				./mixZeroBias_fullSim.exe $run "batch""$batch" $fileNumber $files_per_output &> /dev/null &
			fi
		fi

		fileNumber=$(($fileNumber+$files_per_output))
	done
	sleep 10m
done
