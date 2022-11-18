#!/bin/bash

REF=$(pwd)
INET_PATH="/home/alexb/Documents/Code/ws_omnet/inet4.4/"
MOBILITY_PATH='src/inet/mobility/base/'
PATCH_FILE='mobilityBase.patch'

cd $INET_PATH$MOBILITY_PATH
echo 'Copy patch file into INET Workspace: '$(pwd)
cp $REF/$PATCH_FILE $PATCH_FILE
echo 'Patching File: MobilityBase.cc'
patch < $PATCH_FILE
echo 'Patch successful'
echo 'Removing Patch File'
rm $PATCH_FILE

cd $REF