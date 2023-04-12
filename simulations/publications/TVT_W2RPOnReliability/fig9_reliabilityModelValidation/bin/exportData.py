#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path

sys.path.append(os.path.abspath('./../../../'))
from pythonLib import VectorParser
from pythonLib import ScalarParser

p_file = os.path.dirname(os.path.abspath(__file__))

# General Configuration
parameterTypes = [  "multipleSingleLink.numberApps",
                    "multipleSingleLink.**.wlan[0].mac.ta"]
parameterSynonyms = ["nbrOfApps",
                    "CW"]



vectorNames = ["sampleAverageArbitrationTimeVector"]
vectorSynonyms = ["sampleAverageArbitrationTimeVector"]

vecFile = './output/vector_file.csv'

moduleNames = ["camera1"]

vectorParser = VectorParser.VectorParser()
dataFrame = vectorParser.parseVectorsToPointPerLineData(parameterTypes, parameterSynonyms, vectorNames, vectorSynonyms, moduleNames, vecFile)

dataFrame.to_csv("./output/arbitrationTimes.csv")

