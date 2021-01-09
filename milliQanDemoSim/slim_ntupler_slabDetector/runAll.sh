#! /bin/bash

geant_dir="/net/cms27/cms27r0/schmitz/mcpSignal_slab/dataStore/"
geant_grid=($(ls $geant_dir))
out_dir="/net/cms25/cms25r5/fsetti/milliQan_files/Geant_slabDetector/"

grid_point=1

function run_ntupler {
	for mass in "${geant_grid[@]}"
	do
		charge_grid=($(ls $geant_dir/$mass))
		for charge in "${charge_grid[@]}"
		do
			if [ $grid_point -ge $1 -a $grid_point -lt $2 ]
			then
				full_out_dir=$out_dir$mass"_"$charge
				mkdir -p $full_out_dir
				iter=1
				proc_grid=($(ls $geant_dir/$mass/$charge))
				for proc in "${proc_grid[@]}"
				do
					fileNames=($(ls $geant_dir/$mass/$charge/$proc))
					for file in "${fileNames[@]}"
					do
						weight=($(python get_weights.py $mass $charge $proc $file))
						#echo "for file: " $file "  in " $geant_dir"/"$mass"/"$charge"/"$proc 
						#echo "weight: " $weight
						./make_ntuple $full_out_dir $iter $weight $geant_dir/$mass/$charge/$proc/$file #&> /dev/null
						iter=$(($iter+1))
					done
				done
			fi		

			grid_point=$(($grid_point+1))
		done
	done
}

increment=70
iteration=0
nJobs=480
while [ $iteration -le $nJobs ]
do
	next_iteration=$(($iteration+$increment))
	run_ntupler $iteration $next_iteration &
	iteration=$next_iteration
done

#run_ntupler  200 202
