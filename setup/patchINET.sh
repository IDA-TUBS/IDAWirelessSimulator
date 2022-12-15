#!/bin/bash

REF=$(pwd)
PATH_FILE=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
# Load global variables
if [[ -f ${PATH_FILE}/"../profile" ]]
then
    source ${PATH_FILE}/../profile
else
    echo "First execute setup.sh"
    exit 1
fi



cd $PATH_FILE
source ${PATH_FILE}/../profile

INET_PATH=${PATH_INET}
MOBILITY_PATH='src/inet/mobility/base/'
PATCH_FILE='mobilityBase.patch'

cd $INET_PATH$MOBILITY_PATH
echo 'Copy patch file into INET Workspace: '$(pwd)
cp $PATH_FILE/$PATCH_FILE $PATCH_FILE
echo 'Patching File: MobilityBase.cc'
patch < $PATCH_FILE
echo 'Patch successful'
echo 'Removing Patch File'
rm $PATCH_FILE

cd $REF