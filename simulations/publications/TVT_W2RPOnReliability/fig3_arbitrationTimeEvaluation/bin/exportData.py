#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path

sys.path.append(os.path.abspath('./../../../'))
from pythonLib import VectorParser
from pythonLib import ScalarParser

p_file = os.path.dirname(os.path.abspath(__file__))

# General Configuration
parameterTypes = [  "multipleSingleLink.camera1.rtps.writer[0].appID",
                    "multipleSingleLink.**.wlan[0].mac.ta"]
parameterSynonyms = ["appID",
                    "CW"]



vectorNames = ["packetArbitrationTimesVec","averageArbitrationTimeVector"]
vectorSynonyms = ["packetArbitrationTimesVec","averageArbitrationTimeVector"]

vecFile = './output/vector_file.csv'

moduleNames = ["camera1"]

vectorParser = VectorParser.VectorParser()
dataFrame = vectorParser.parseVectorsToPointPerLineData(parameterTypes, parameterSynonyms, vectorNames, vectorSynonyms, moduleNames, vecFile)

dataFrame.to_csv("./output/arbitrationTimes.csv")

