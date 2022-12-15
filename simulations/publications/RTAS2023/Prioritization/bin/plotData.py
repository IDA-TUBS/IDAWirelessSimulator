#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path

sys.path.append(os.path.abspath('./../../../'))
from pythonLib import GraphPlotter

p_file = os.path.dirname(os.path.abspath(__file__))

if __name__ == '__main__':

    data = GraphPlotter.csvToPanda("output/violationRatesPrio.csv")
    GraphPlotter.plotViolationRatesMulticast(data, 'Prio')

    data = GraphPlotter.csvToPanda("output/sampleLatenciesPrio.csv")
    GraphPlotter.plotLatenciesMulticast(data, 'Prio')




    data = GraphPlotter.csvToPanda("output/violationRatesNoPrio.csv")
    GraphPlotter.plotViolationRatesMulticast(data, 'NoPrio')

    data = GraphPlotter.csvToPanda("output/sampleLatenciesNoPrio.csv")
    GraphPlotter.plotLatenciesMulticast(data, 'NoPrio')