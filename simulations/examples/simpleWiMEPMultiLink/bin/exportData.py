#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path

import argparse

sys.path.append(os.path.abspath('./../../'))
from pythonLib import VectorParser
from pythonLib import ScalarParser

p_file = os.path.dirname(os.path.abspath(__file__))






# General Configuration
parameterTypes = ["multiLink.sender.rtpsAppWriter[*].sampleSize",
                    "multiLink.sender.rtps.writer[0].shaping",                    
                    "multiLink.sender.rtps.writer[0].fragmentSize",
                    "multiLink.**.wlan[0].mac.bitErrorRate",
                    "multiLink.**.wlan[0].mac.ta"]
parameterSynonyms = ["sampleSize",
                    "shapingTime",
                    "fragmentSize",
                    "bitErrorRate",
                    "arbitrationTime"]








vectorParser = VectorParser.VectorParser()

vectorNames = ["sampleLatencies"]
vectorSynonyms = ["sampleLatencies"]

vecFile = './output/vector_file.csv'

moduleNames = ["receivers"]
df = vectorParser.processVectors(parameterTypes, parameterSynonyms, vectorNames, vectorSynonyms, moduleNames, vecFile)
df.to_csv("./output/sampleLatencies.csv")



scalarParser = ScalarParser.ScalarParser()

scalarNames = ["deadlineViolationRate"]
scalarSynonyms = ["deadlineViolationRate"]

scaFile = './output/histogram_file.csv'

moduleNames = ["receivers"]
df = scalarParser.processScalars(parameterTypes, parameterSynonyms, scalarNames, scalarSynonyms, moduleNames, scaFile)
df.to_csv("./output/violationRates.csv")