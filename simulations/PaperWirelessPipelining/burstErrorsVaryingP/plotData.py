#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path

import matplotlib.pyplot as plt
import dill

sys.path.append(os.path.abspath('./../../'))
from pythonLib import GraphPlotter
from pythonLib import burstErrors

p_file = os.path.dirname(os.path.abspath(__file__))






if __name__ == '__main__':

    # data = GraphPlotter.csvToPanda("output/sampleLatencies.csv")
    # with open("output/sampleLatencies.pkl", 'rb') as dillFile:
    #     data = dill.load(dillFile)        
    #     GraphPlotter.plotLatencyAndSlack(data)
    #     GraphPlotter.plotSlack(data)


    with open("output/violationRates.pkl", 'rb') as dillFile:
        data = dill.load(dillFile)        
        GraphPlotter.plotViolationRateTriParameters(data, ['sampleDeadline', 'gilbertElliotP'])  
    

    

    # data = GraphPlotter.csvToPanda("output/sampleLatencies.csv")
    # with open("output/violationRates.pkl", 'rb') as dillFile:
    #     data = dill.load(dillFile)        
    #     GraphPlotter.plotViolationRateUnicast(data, 'sampleDeadline')  
    

    


    # with open("output/errorTrace.pkl", 'rb') as dillFile:
    #     data = dill.load(dillFile)
    #     for index, row in data.iterrows():
    #         seq = row['vecvalue']

    #         if isinstance(seq, str):
    #             seq = [int(x) for x in seq.split(" ")]    
    #         else:
    #             seq = [int(x) for x in seq]
    #         burstErrors.analyseSequence(seq)

    #     with open("output/missingSamples.pkl", 'rb') as dillFile:
    #         data2 = dill.load(dillFile)  


    #         with open("output/sampleTxStartpoints.pkl", 'rb') as dillFile:
                      
    #             data3 = dill.load(dillFile)        
    #             GraphPlotter.processMissingSamples(data, data2, data3)  

    # plt.show()
    # quit()