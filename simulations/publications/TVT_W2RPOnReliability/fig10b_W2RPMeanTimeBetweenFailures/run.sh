# Call this script m this directory!
bash bin/runOmnet
bash bin/scaveResults
python3 bin/exportData.py
python3 bin/plotData.py
bash bin/runOmnet86400
bash bin/scaveResults
python3 bin/exportData.py
python3 bin/plotData.py
