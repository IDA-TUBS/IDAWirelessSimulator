#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path

sys.path.append(os.path.abspath('./../../../'))
from pythonLib import GraphPlotter

p_file = os.path.dirname(os.path.abspath(__file__))

if __name__ == '__main__':

    data = GraphPlotter.csvToPanda("output/violationRates.csv")
    
    
    if not os.path.exists("./figures"):
        os.makedirs("./figures/")
        
        
    GraphPlotter.plotViolationRateTriPointPerLineData(data, 800+148, combined=True)
