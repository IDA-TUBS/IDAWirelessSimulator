#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path

sys.path.append(os.path.abspath('./../../../'))
from pythonLib import GraphPlotter

p_file = os.path.dirname(os.path.abspath(__file__))

if __name__ == '__main__':
    
    data1 = GraphPlotter.csvToPanda("output/collisions_tsh_700.csv")
    dataReader1 = GraphPlotter.csvToPanda("output/collisionsReaders_tsh_700.csv")
    # GraphPlotter.plotTaCollisions(data1, dataReader1)

    data2 = GraphPlotter.csvToPanda("output/collisions_tsh_1000.csv")
    dataReader2 = GraphPlotter.csvToPanda("output/collisionsReaders_tsh_1000.csv")

    data3 = GraphPlotter.csvToPanda("output/collisions_tsh_1300.csv")
    dataReader3 = GraphPlotter.csvToPanda("output/collisionsReaders_tsh_1300.csv")

    # data = GraphPlotter.csvToPanda("output/collisions_tsh_3600.csv")
    # GraphPlotter.plotAppViolationRates(data)

    fig, ax = GraphPlotter.plotTaCollisionsCombined([data1, data2, data3],[dataReader1, dataReader2, dataReader3],nmax=7)
    GraphPlotter.plotUtilCombined([data1, data2, data3],[dataReader1, dataReader2, dataReader3], fig, ax, nmax=7)