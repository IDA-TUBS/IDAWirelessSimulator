rm -r ./results

#PATH_SIM="/home/jonas/Documents/omnetpp_workspace/idaWirelessSimulator/"
PATH_SIM="./../../../"
SIM_PATH="publications/W2RP/"
SIM_NAME="W2RP.ini"

PATH_INET="./../../../../inet4.4/"
#PATH_INET="/home/jonas/Documents/omnetpp_workspace/inet4.4/"

${PATH_SIM}idaWirelessSimulator -u Cmdenv \
-f ${PATH_SIM}simulations/${SIM_PATH}${SIM_NAME} \
-n \
:${PATH_INET}src\
:${PATH_SIM}simulations\
:${PATH_SIM}\


sh scaveResults.sh
python3 exportData.py
python3 plotData.py
