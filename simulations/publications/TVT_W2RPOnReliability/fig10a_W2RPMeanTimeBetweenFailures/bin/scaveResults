#!/bin/bash

# check if data exists and create folder if not
if [ ! -d ./output ]
then
	mkdir output
fi

opp_scavetool x ./results/*.vec -o ./output/vector_file.csv
opp_scavetool x ./results/*.sca -o ./output/histogram_file.csv
