#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import numpy as np
import pandas as pd


def parse_if_number(s):
    try: return float(s)
    except: return True if s=="true" else False if s=="false" else s if s else None

def parse_ndarray(s):
    return np.fromstring(s, sep=' ') if s else None
    
    
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

        dataFrame = dataFrame[dataFrame['module'].notna()]
        dataFrame['vecvalue'] = dataFrame['vecvalue'].replace([np.nan], 0)
        print(dataFrame)

        # filter for specific modules
        if len(moduleNames) != 0:
            for moduleName in moduleNames:
                print(dataFrame)
                dataFrame = dataFrame[dataFrame['module'].str.contains(moduleName)]
        
        if(not keepTime):
            del dataFrame["vectime"]

        return dataFrame





    def parseVectorsToPointPerLineData(self, parTypes, parSynonyms, vecTypes, vecSynonyms, moduleNames, fileName):
    
        # TODO run und moduleName wird noch benötigt
    
    
        #Configuration
        parameterTypes = parTypes
        vectorNames = vecTypes


        #parameter_types_synonyms
        parameterTypesSynonyms = parSynonyms
        vectorNamesSynonyms = vecSynonyms

        # Load the omnetp data as a vector frame file
        dataFrameVector = pd.read_csv(fileName, converters = {
            'attrvalue': parse_if_number,
            'binedges': parse_ndarray,
            'binvalues': parse_ndarray,
            'vectime': parse_ndarray,
            'vecvalue': parse_ndarray})

        # Get all relevant lines with:
        #	- type column == 'config'
        #       - attrname in parameterTypes
        paramVectorDataFrame = dataFrameVector[(dataFrameVector.type == 'config') & (dataFrameVector.attrname.isin(parameterTypes))]

        # Shrink the file to the relevant columns: run, attrname and values - e.g.: | run1 | ber.n | 1e-5 | ... |
        paramVectorDataFrame = paramVectorDataFrame.loc[paramVectorDataFrame.type=='config', ['run', 'attrname', 'attrvalue']]


        # Then bring reformat the csv table into the following form (run is the index)
        # | run  | param1 | param2 | param3 | ... |
        # | run1 | 1e-4   | 2000us | 1000B  | ... |
        paramVectorDataFrame = paramVectorDataFrame.pivot(index='run', columns='attrname', values='attrvalue')
    
        # Then select only the relevant vector rows from the initial data frame vector
        vectorOnlyDataFrame = dataFrameVector[(dataFrameVector.type=="vector") & (dataFrameVector.name.isin(vectorNames))] #XXX if histograms should also be included, do it here!

        # !!! Plausibility Check: Check if a Vector type cannot be found !!!
        for i in range(0,len(vectorNames)):
            #for index, row in dataFrameVector.iterrows():
            #    print(row)
            if not dataFrameVector.name.isin([vectorNames[i]]).any():
                print("WARNING: Value: \"" + vectorNames[i] + "\" does not match any vector in the data")
                #exit()

        # Merge the related parameters to the vector lines
        vectorOnlyDataFrame = vectorOnlyDataFrame.merge(paramVectorDataFrame, left_on='run', right_index=True, how='outer') #TODO convert the index here! after merging...

        # Write back intermediate result to csv file
        #vectorOnlyDataFrame.to_csv("./output/relevant_parametriced_vector_lines.csv") #XXX Lange Datensaetze duerfen nicht abgeschnitten werden!


    	#########################################################################
    	#########################################################################
    	################ No write the relevant output ###########################
    	#########################################################################
    	#########################################################################

        # Create the column lines vector for the resulting file
        csvResultsFileColumnNames = parameterTypes + vectorNames + ["timestamp"] # Concatenate the relevant vectors

        # Create a new csv file where each lines is one data-point
        outputDataField = pd.DataFrame(columns= parameterTypesSynonyms + vectorNamesSynonyms + ["timestamp"])
	
        # Now run through the lines of the current data frame where each line represents a vector output
        # Next init the running variables
        currentRow = 0
        currentVectorIndex = 0
        numberOfRows = vectorOnlyDataFrame.shape[0];
        for index, row in vectorOnlyDataFrame.iterrows():
        
            if (i+1)%10000 == 0:
                print("Reading vector row: " + str(currentVectorIndex+1) + "/" + str(numberOfRows))
                
            currentVectorIndex = currentVectorIndex + 1

            # First get the fixed parameters
            parameterValues  = [None]*len(parameterTypes)    # e.g. fragment_size, bit_error_rate, ...
            for i in range(0,len(parameterTypes)):
                parameterValues[i] = row[parameterTypes[i]]
       
            # Get the current vector
            if not (row['type'] == 'vector'):
                print("warning no vector")
                continue

            # Get the current Vector, Vectortime and Vectorname
            currentValueVector = row['vecvalue']
            currentValueVectorTime = row['vectime']

            vectorName = row['name']
        

            # Calculate fix values of each vector related data point line
            insertionArray = [None]*len(csvResultsFileColumnNames)
            insertionArray[0:len(parameterValues)] = parameterValues

            print("Vector Name: " + str(vectorName))
            print("Lines: " + str(len(currentValueVector)))

            # Iterate through the vector
            for i in range(0, len(currentValueVector)):

                if i%1000 == 0 :
                    print(str(i) + "/" + str(len(currentValueVector)))

                # Csv index to place the data
                valueIndex = csvResultsFileColumnNames.index(vectorName)
                timeIndex = csvResultsFileColumnNames.index("timestamp")

                # Get the current data value and insert it at the right place
                insertionArray[valueIndex] = currentValueVector[i]
                insertionArray[timeIndex] = currentValueVectorTime[i]

                # Insert the output_data as a new row into the data frame
                outputDataField.loc[currentRow] = insertionArray

                # Increase the current row counter
                currentRow = currentRow + 1

            
        # Write back the file and return
        #outputDataField.to_csv("./output/omnet_output_vector_data.csv")
        return outputDataField

    
    
    
    
    
    
    
    
    
    
