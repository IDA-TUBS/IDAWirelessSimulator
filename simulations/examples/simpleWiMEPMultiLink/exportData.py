#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path

sys.path.append(os.path.abspath('./../../'))
from pythonLib import VectorParser

p_file = os.path.dirname(os.path.abspath(__file__))




vectorParser = VectorParser.VectorParser()

#Configuration
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

vectorNames = ["sampleLatencies"]
vectorSynonyms = ["sampleLatencies"]

vecFile = './output/vector_file.csv'

moduleNames = ["receivers"]
df = vectorParser.processVectors(parameterTypes, parameterSynonyms, vectorNames, vectorSynonyms, moduleNames, vecFile)
df.to_csv("./output/vectors.csv")
