#!/bin/bash

REF=$(pwd)

PATH_FILE=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $PATH_FILE

read -p "Enter path to inet4 package: " -i "$HOME/" -e PATH_INET
rm ../profile

template=`cat profileTemplate`
echo "${template//<PATH_INET>/$PATH_INET}" > ../profile

cd $REF