#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path

sys.path.append(os.path.abspath('./../../../'))
from pythonLib import VectorParser
from pythonLib import ScalarParser

p_file = os.path.dirname(os.path.abspath(__file__))

# General Configuration
parameterTypes = [  "singleLink.camera.rtpsAppWriter[*].sampleSize",
                    "singleLink.camera.rtps.writer[0].shaping",                    
                    "singleLink.camera.rtps.writer[0].fragmentSize",
                    "singleLink.**.wlan[0].mac.bitErrorRate",
                    "singleLink.**.wlan[0].mac.ta"]
parameterSynonyms = ["sampleSize",
                    "shapingTime",
                    "fragmentSize",
                    "bitErrorRate",
                    "arbitrationTime"]



vectorNames = ["sampleViolationRateVector"]
vectorSynonyms = ["sampleViolationRateVector"]

vecFile = './output/vector_file.csv'

moduleNames = ["vehicle"]

vectorParser = VectorParser.VectorParser()
dataFrame = vectorParser.parseVectorsToPointPerLineData(parameterTypes, parameterSynonyms, vectorNames, vectorSynonyms, moduleNames, vecFile)

dataFrame.to_csv("./output/violationRates.csv")

