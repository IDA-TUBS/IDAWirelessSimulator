#!/bin/bash

# Call this script from this directory!
bash bin/runOmnetPrio
bash bin/scaveResults
python3 bin/exportData.py -s Prio
bash bin/runOmnetNoPrio
bash bin/scaveResults
python3 bin/exportData.py -s NoPrio
#plot results
python3 bin/plotData.py
