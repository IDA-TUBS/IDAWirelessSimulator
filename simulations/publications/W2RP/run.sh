# Call this script out of this directory!
bash bin/runOmnet
bash bin/scaveResults
python3 bin/exportData.py
python3 bin/plotData.py
