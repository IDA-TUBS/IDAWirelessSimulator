rm -r ./results

PATH_SIM="/home/alexb/Documents/Code/IDAWirelessSimulator/"
SIM_PATH="PaperWirelessPipelining/burstErrorsVaryingR/"
SIM_NAME="burstErrorsVaryingR.ini"

PATH_INET="/home/alexb/Documents/Code/ws_omnet/inet4.4/"

${PATH_SIM}idaWirelessSimulator -u Cmdenv \
-f ${PATH_SIM}simulations/${SIM_PATH}${SIM_NAME} \
-n \
:${PATH_INET}src\
:${PATH_SIM}simulations\
:${PATH_SIM}\


sh scaveResults.sh
python3 exportData.py
# python3 plotData.py

