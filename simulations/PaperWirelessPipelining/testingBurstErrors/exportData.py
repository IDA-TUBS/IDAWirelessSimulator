#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path

import argparse

sys.path.append(os.path.abspath('./../../'))
from pythonLib import VectorParser
from pythonLib import ScalarParser

p_file = os.path.dirname(os.path.abspath(__file__))




parser = argparse.ArgumentParser(
        prog = 'ExportData',
        description = 'Export OMNeT++ simulation data')

parser.add_argument('-s',
                    action = 'store',
                    default = '',
                    dest = 'suffix',
                    help = 'fill suffix under which to store the exported data')
args = parser.parse_args()  


# General Configuration
parameterTypes = ["multiLink.sender.rtpsAppWriter[*].sampleSize",
                    "multiLink.sender.rtps.writer[0].shaping",                    
                    "multiLink.sender.rtps.writer[0].fragmentSize",
                    "multiLink.sender.rtps.writer[0].deadline",
                    "multiLink.**.wlan[0].mac.ta",
                    "multiLink.**.wlan[0].mac.geBurstParamP",
                    "multiLink.**.wlan[0].mac.geBurstParamR"]
parameterSynonyms = ["sampleSize",
                    "shapingTime",
                    "fragmentSize",
                    "sampleDeadline",
                    "arbitrationTime",
                    "gilbertElliotP",
                    "gilbertElliotP"]









vectorParser = VectorParser.VectorParser()

vectorNames = ["sampleLatencies"]
vectorSynonyms = ["sampleLatencies"]

vecFile = './output/vector_file.csv'

moduleNames = ["receivers"]
df = vectorParser.processVectors(parameterTypes, parameterSynonyms, vectorNames, vectorSynonyms, moduleNames, vecFile)
df.to_csv("./output/sampleLatencies" + args.suffix + ".csv")



# vectorNames = ["errorTrace"]
# vectorSynonyms = ["errorTrace"]

# vecFile = './output/vector_file.csv'

# moduleNames = ["receivers"]
# df = vectorParser.processVectors(parameterTypes, parameterSynonyms, vectorNames, vectorSynonyms, moduleNames, vecFile)
# df.to_csv("./output/errorTrace" + args.suffix + ".csv")




scalarParser = ScalarParser.ScalarParser()

scalarNames = ["deadlineViolationRate"]
scalarSynonyms = ["deadlineViolationRate"]

scaFile = './output/histogram_file.csv'

moduleNames = ["receivers"]
df = scalarParser.processVectors(parameterTypes, parameterSynonyms, scalarNames, scalarSynonyms, moduleNames, scaFile)
df.to_csv("./output/violationRates" + args.suffix + ".csv")