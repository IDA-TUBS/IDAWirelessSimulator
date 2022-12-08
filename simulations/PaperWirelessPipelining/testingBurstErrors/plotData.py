#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path

sys.path.append(os.path.abspath('./../../'))
from pythonLib import GraphPlotter
from pythonLib import burstErrors

p_file = os.path.dirname(os.path.abspath(__file__))




if __name__ == '__main__':

    data = GraphPlotter.csvToPanda("output/sampleLatencies.csv")

    GraphPlotter.plotLatencyAndSlack(data)
 
    # seq = [int(x) for x in errorSequence.split(" ")]    
    # burstErrors.analyseSequence(seq)