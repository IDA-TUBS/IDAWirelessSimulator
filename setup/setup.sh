#!/bin/bash

read -p "Enter path to inet4 package: " -i "$HOME/" -e PATH_INET
rm ../profile

template=`cat profileTemplate`
echo "${template//<PATH_INET>//$PATH_INET}" > ../profile