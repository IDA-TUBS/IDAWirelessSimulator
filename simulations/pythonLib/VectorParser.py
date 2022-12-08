#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import numpy as np
import pandas as pd

class VectorParser(object):
    def __init__(self):
        print("Initialize Vector Parser")

    def parseNumber(self, s):
        try: return float(s)
        except: return True if s=="true" else False if s=="false" else s if s else None

    def parseNpArray(self, s):
        return np.fromstring(s, sep=' ') if s else None

    def processVectors(self, parTypes, parSynonyms, vecTypes, vecSynonyms, moduleNames, fileName, keepTime = False):

        #Configuration
        parameterTypes = parTypes
        vectorNames = vecTypes

        parameterSynonyms = parSynonyms
        vectorSynonyms = vecSynonyms

        # Load the omnetp data as a vector frame file
        dataFrame = pd.read_csv(fileName, converters = {
            'attrvalue': self.parseNumber,
            'binedges': self.parseNpArray,
            'binvalues': self.parseNpArray,
            'vectime': self.parseNpArray,
            'vecvalue': self.parseNpArray})

        #################################
        # process simulation parameters #
        #################################

        # Get all relevant lines with:
        #	- type column == 'param'
        #       - attrname in parameter_types
        parameterVector = dataFrame[(dataFrame.type == 'config') & (dataFrame.attrname.isin(parameterTypes))]
        # Shrink the file to the relevant columns: run, attrname and values - e.g.: | run1 | ber.n | 1e-5 | ... |
        parameterVector = parameterVector.loc[parameterVector.type=='config', ['run', 'attrname', 'attrvalue']]

        # Then bring reformat the csv table into the following form (run is the index)
        # | run  | param1 | param2 | param3 | ... |
        # | run1 | 1e-4   | 2000us | 1000B  | ... |
        parameterVector = parameterVector.pivot(index='run', columns='attrname', values='attrvalue')

        # Then select only the relevant vector rows from the initial data frame vector
        dataFrame = dataFrame[(dataFrame.type=="vector") & (dataFrame.name.isin(vectorNames))]

        # Merge the related parameters to the vector lines
        dataFrame = dataFrame.merge(parameterVector, left_on='run', right_index=True, how='outer') #TODO convert the index here! after merging...

        synonymMap = {parameterTypes[i]: parameterSynonyms[i] for i in range(len(parameterTypes))}
        dataFrame = dataFrame.rename(columns=synonymMap, errors="raise")

        dataFrame['vecvalue'] = dataFrame['vecvalue'].replace([np.nan], 0)

        # filter for specific modules
        if len(moduleNames) != 0:
            for moduleName in moduleNames:
                dataFrame = dataFrame[dataFrame['module'].str.contains(moduleName)]
        
        if(not keepTime):
            del dataFrame["vectime"]

        return dataFrame


