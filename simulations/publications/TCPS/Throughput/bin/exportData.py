#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path

import argparse

sys.path.append(os.path.abspath('./../../../'))
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
                    "multiLink.sender.wlan[0].mac.bitErrorRate",
                    "multiLink.receivers[0].wlan[0].mac.bitErrorRate",
                    "multiLink.receivers[1].wlan[0].mac.bitErrorRate",
                    "multiLink.receivers[2].wlan[0].mac.bitErrorRate",
                    "multiLink.receivers[3].wlan[0].mac.bitErrorRate",
                    "multiLink.receivers[4].wlan[0].mac.bitErrorRate",
                    "multiLink.**.wlan[0].mac.ta"]
parameterSynonyms = ["sampleSize",
                    "shapingTime",
                    "fragmentSize",
                    "bitErrorRate",
                    "bitErrorRate0",
                    "bitErrorRate1",
                    "bitErrorRate2",
                    "bitErrorRate3",
                    "bitErrorRate4",
                    "arbitrationTime"]








vectorParser = VectorParser.VectorParser()

vectorNames = ["sampleLatencies"]
vectorSynonyms = ["sampleLatencies"]

vecFile = './output/vector_file.csv'

moduleNames = ["receivers"]
df = vectorParser.processVectors(parameterTypes, parameterSynonyms, vectorNames, vectorSynonyms, moduleNames, vecFile)
df.to_csv("./output/sampleLatencies" + args.suffix + ".csv")



scalarParser = ScalarParser.ScalarParser()

scalarNames = ["deadlineViolationRate"]
scalarSynonyms = ["deadlineViolationRate"]

scaFile = './output/histogram_file.csv'

moduleNames = ["receivers"]
df = scalarParser.processScalars(parameterTypes, parameterSynonyms, scalarNames, scalarSynonyms, moduleNames, scaFile)
df.to_csv("./output/violationRates" + args.suffix + ".csv")