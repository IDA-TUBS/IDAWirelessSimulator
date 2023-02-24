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
parameterTypes = ["multipleMultiLink.numberApps"]
parameterSynonyms = ["numberApps"]








vectorParser = VectorParser.VectorParser()


scalarParser = ScalarParser.ScalarParser()

scalarNames = ["numSent", "numCollision", "averageTa", "appViolationRate"]
scalarSynonyms = ["numSent", "numCollision", "averageTa", "appViolationRate"]

scaFile = './output/histogram_file.csv'

moduleNames = ["sender"]
df = scalarParser.processScalars(parameterTypes, parameterSynonyms, scalarNames, scalarSynonyms, moduleNames, scaFile)
df.to_csv("./output/collisions" + args.suffix + ".csv")




scalarNames = ["numSent", "numCollision", "numReceived", "averageTa", "appViolationRate"]
scalarSynonyms = ["numSent", "numCollision", "averageTa", "appViolationRate"]

scaFile = './output/histogram_file.csv'

moduleNames = ["receivers"]
df = scalarParser.processScalars(parameterTypes, parameterSynonyms, scalarNames, scalarSynonyms, moduleNames, scaFile)
df.to_csv("./output/collisionsReaders" + args.suffix + ".csv")




scalarNames = [ "deadlineViolationRate"]
scalarSynonyms = ["deadlineViolationRate"]

scaFile = './output/histogram_file.csv'

moduleNames = ["reader"]
df = scalarParser.processScalars(parameterTypes, parameterSynonyms, scalarNames, scalarSynonyms, moduleNames, scaFile)
df.to_csv("./output/readerViolationRates" + args.suffix + ".csv")