#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path
import dill

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
parameterTypes = ["multiLink.sender.rtpsAppWriter[*].samplePeriod",
                    "multiLink.sender.rtpsAppWriter[*].sampleSize",
                    "multiLink.sender.rtps.writer[0].shaping",                    
                    "multiLink.sender.rtps.writer[0].fragmentSize",
                    "multiLink.sender.rtps.writer[0].deadline",
                    "multiLink.**.wlan[0].mac.ta",
                    "multiLink.**.wlan[0].mac.geBurstParamP",
                    "multiLink.**.wlan[0].mac.geBurstParamR"]
parameterSynonyms = ["samplePeriod",
                    "sampleSize",
                    "shapingTime",
                    "fragmentSize",
                    "sampleDeadline",
                    "arbitrationTime",
                    "gilbertElliotP",
                    "gilbertElliotR"]









vectorParser = VectorParser.VectorParser()

print("export latencies")

vectorNames = ["sampleLatencies"]
vectorSynonyms = ["sampleLatencies"]

vecFile = './output/vector_file.csv'

moduleNames = ["receivers"]
df = vectorParser.processVectors(parameterTypes, parameterSynonyms, vectorNames, vectorSynonyms, moduleNames, vecFile, keepTime=True)
with open("output/sampleLatencies.pkl", "wb") as dill_file:
    dill.dump(df, dill_file)
df.to_csv("./output/sampleLatencies" + args.suffix + ".csv")


print("export missing samples")

vectorNames = ["missingSamples"]
vectorSynonyms = ["missingSamples"]

vecFile = './output/vector_file.csv'

moduleNames = ["receivers"]
df = vectorParser.processVectors(parameterTypes, parameterSynonyms, vectorNames, vectorSynonyms, moduleNames, vecFile, keepTime=True)
with open("output/missingSamples.pkl", "wb") as dill_file:
    dill.dump(df, dill_file)
df.to_csv("./output/missingSamples" + args.suffix + ".csv")



print("sample transmission timestamps")

vectorNames = ["sampleTxStartpoints"]
vectorSynonyms = ["sampleTxStartpoints"]

vecFile = './output/vector_file.csv'

moduleNames = ["sender"]
df = vectorParser.processVectors(parameterTypes, parameterSynonyms, vectorNames, vectorSynonyms, moduleNames, vecFile, keepTime=True)
with open("output/sampleTxStartpoints.pkl", "wb") as dill_file:
    dill.dump(df, dill_file)
df.to_csv("./output/sampleTxStartpoints" + args.suffix + ".csv")



print("export error traces")

vectorNames = ["errorTrace"]
vectorSynonyms = ["errorTrace"]

vecFile = './output/vector_file.csv'

moduleNames = ["receivers"]
df = vectorParser.processVectors(parameterTypes, parameterSynonyms, vectorNames, vectorSynonyms, moduleNames, vecFile, keepTime=True)
with open("output/errorTrace.pkl", "wb") as dill_file:
    dill.dump(df, dill_file)
df.to_csv("./output/errorTrace" + args.suffix + ".csv")




scalarParser = ScalarParser.ScalarParser()

print("export violation rates")

scalarNames = ["deadlineViolationRate"]
scalarSynonyms = ["deadlineViolationRate"]

scaFile = './output/histogram_file.csv'

moduleNames = ["receivers"]
df = scalarParser.processScalars(parameterTypes, parameterSynonyms, scalarNames, scalarSynonyms, moduleNames, scaFile)
with open("output/violationRates.pkl", "wb") as dill_file:
    dill.dump(df, dill_file)
df.to_csv("./output/violationRates" + args.suffix + ".csv")