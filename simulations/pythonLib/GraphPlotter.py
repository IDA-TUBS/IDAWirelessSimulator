#!/usr/bin/env python3
# -*- coding: utf-8 -*- 
import sys
import os

import matplotlib.pyplot as plt
import pandas as pd
from matplotlib import cm


def csvToPanda(filename):
    """
    Should return tidy panda data
    """
    print("Read panda data from csv file")
    data_frame = pd.read_csv(filename)
    return data_frame


def combineViolationRates(series):
    return sum(series) / len(series)

def keep(series):
    series.reset_index(inplace=True, drop=True)
    print(series, len(series))
    return series[0] #lambda x: x


def plotViolationRateTri(data, combined=True):
    # plot violation rate over shaping and arbitration time

    print(data)

    fragmentSize = data['fragmentSize'][0]
    sampleSize = data['sampleSize'][0]
    shapingTime = data['shapingTime'][0]

    print(sampleSize, fragmentSize)

    # remove type, shapingTime, sample size and fragment size from data
    del data['fragmentSize']
    del data['sampleSize']
    del data['shapingTime']
    del data['type']
    # and other unnecessary data
    del data['attrvalue']
    del data['attrname']
    del data['name']
    fig = None
    ax = None

    if(combined):        
        del data['module']

        # aggregate lines with the same attributes (shaping and arbitration time)
        # into a single line with the combined deadline violation rate: sum(dvr_i)/num readers

        # calculate frame error rate
        s_frag = fragmentSize * 8
        s_frame = s_frag + (64 + 20 + 28 + 36) * 8
        ber = data["bitErrorRate"]
        list_fer = []
        for x in ber:
            if (x not in list_fer):
                list_fer.append(round(1 - (1-x)**(s_frame),1) * 100)
        data["bitErrorRate"] = round(1 - (1-data["bitErrorRate"])**(s_frame),1) * 100

        data["arbitrationTime"] = data["arbitrationTime"] * 9

        renameMap = {"value": "violationRate", "bitErrorRate": "frameErrorRate"}
        data = data.rename(renameMap, axis=1, errors="raise")

        column_map = {}
        cols = []
        for col in data.columns:
            if col == "Unnamed: 0":
                continue            
            cols.append(col)
            if col == "violationRate":
                column_map[col] = combineViolationRates
            else:
                column_map[col] = keep

        #data = data.groupby(cols, as_index=False).agg(column_map)
        data = data.groupby(['run']).agg(column_map)        

        # data.to_csv("./output/tmp.csv")

        fig = plt.figure(figsize=(8,3))
        ax = plt.axes(projection='3d')
        surf = ax.plot_trisurf(data['arbitrationTime'], data['frameErrorRate'], data['violationRate'], cmap=cm.get_cmap('plasma'), linewidth=0.2)

        z_vector_ticks  = [0.0, 0.2, 0.4, 0.6, 0.8, 1.0]
        z_vector_labels  = ["0", "20", "40", "60", "80", "100"]
        ax.set(zticklabels=z_vector_labels, zticks=z_vector_ticks)

        ax.set_xlabel('average arbitration time (\u03BCs)')
        ax.set_ylabel('frame error rate (%)')
        ax.set_zlabel('observed deadline\nviolation rate (%)')

        plt.gca().invert_xaxis()
        plt.yticks(rotation = -30, rotation_mode="anchor")

        if(not os.path.exists("figures")):
            os.makedirs("figures")
        plt.savefig("figures/violationRate.pdf" ,bbox_inches='tight')

        plt.show()
    else:
        # TODO
        i = 0