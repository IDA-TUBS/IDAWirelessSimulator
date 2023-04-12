#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path

sys.path.append(os.path.abspath('./../../../'))
from pythonLib import VectorParser
from pythonLib import ScalarParser

p_file = os.path.dirname(os.path.abspath(__file__))

# General Configuration
parameterTypes = [  "singleLink.**.wlan[0].mac.bitErrorRate",
                    "singleLink.**.wlan[0].mac.ta"]
parameterSynonyms = ["bit_error_rate",
                    "arbitr_time"]

vectorNames = ["sampleViolationRateVector"]
vectorSynonyms = ["violation_rate"]

vecFile = './output/vector_file.csv'

moduleNames = ["vehicle"]

vectorParser = VectorParser.VectorParser()
dataFrame = vectorParser.parseVectorsToPointPerLineData(parameterTypes, parameterSynonyms, vectorNames, vectorSynonyms, moduleNames, vecFile)

dataFrame.to_csv("./output/violationRates.csv")

