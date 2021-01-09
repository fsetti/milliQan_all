#! /bin/bash
geant_dir="/net/cms27/cms27r0/schmitz/mcpSignal_fullsim/hadoop/cms/store/user/ryan/beammcp_fullSim_v1/mcp_v8_v1_save2m_full_fullSim_v1/dataStore/"
geant_grid=($(ls $geant_dir))
out_dir="/net/cms25/cms25r5/fsetti/milliQan_files/Geant_fullDetector_bkg/"
mkdir -p $out_dir

#empty_dirs=($(find $out_dir -type d -empty))
#for mass in "${geant_grid[@]}"
#do
#	charge_grid=($(ls $geant_dir/$mass))
#	for charge in "${charge_grid[@]}"
#	do
#		full_out_dir=$out_dir$mass"_"$charge
#		for empty_mass in "${empty_dirs[@]}"
#		do
#			if [ "$empty_mass" == "$full_out_dir"  ]
#			then
#				mkdir -p $full_out_dir
#				iter=1
#				proc_grid=($(ls $geant_dir/$mass/$charge))
#				for proc in "${proc_grid[@]}"
#				do
#					fileNames=($(ls $geant_dir/$mass/$charge/$proc))
#					for file in "${fileNames[@]}"
#					do
#						./make_ntuple $full_out_dir $iter $geant_dir/$mass/$charge/$proc/$file
#						iter=$(($iter+1))
#					done
#				done
#			fi
#		done
#	done
#done

grid_point=1
for mass in "${geant_grid[@]}"
do
	if [ "$mass" == "m_10p0" ] 
	then 
		charge_grid=($(ls $geant_dir/$mass))
		for charge in "${charge_grid[@]}"
		do
		#if [ $grid_point -gt 350 -a $grid_point -lt 390 ]
		#then
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
					./make_ntuple $full_out_dir $iter $weight $geant_dir/$mass/$charge/$proc/$file
					iter=$(($iter+1))
				done
			done
		#fi
		grid_point=$(($grid_point+1))
		done
	fi
done
