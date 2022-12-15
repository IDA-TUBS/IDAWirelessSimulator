#!/usr/bin/env python3
# -*- coding: utf-8 -*- 
import sys
import os
import re


import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.patches import Patch
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
    return series[0] #lambda x: x

def createList(series):
    return list(series)

def createListFromLists(series):
    return list(series)


def plotViolationRateTri(data, combined=True):
    # plot violation rate over shaping and arbitration time

    fragmentSize = data['fragmentSize'][0]
    sampleSize = data['sampleSize'][0]
    shapingTime = data['shapingTime'][0]

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

        data["arbitrationTime"] = (data["arbitrationTime"] * 9) / 2

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
        surf = ax.plot_trisurf(data['arbitrationTime'], data['frameErrorRate'], data['violationRate'],
                                antialiased = True,
                                cmap=cm.get_cmap('plasma'),
                                linewidth=0.2)


        # ax.set(xticklabels=x_vector_labels, xticks=x_vector_ticks)
        plt.xticks(rotation = 60, rotation_mode="anchor")

        y_vector_ticks  = [0, 20, 40, 60, 80]
        y_vector_labels  = ["0", "20", "40", "60", "80"]
        ax.set(yticklabels=y_vector_labels, yticks=y_vector_ticks)
        plt.yticks(rotation = -30, rotation_mode="anchor")

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

        # plt.show()
    else:
        # TODO draw a single plott per reader
        i = 0




def plotViolationRatesMulticast(data, suffix=''):

    fragmentSize = data['fragmentSize'][0]
    sampleSize = data['sampleSize'][0]
    shapingTime = data['shapingTime'][0]

    del data['fragmentSize']
    del data['sampleSize']
    del data['shapingTime']
    del data['type']
    # and other unnecessary data
    del data['attrvalue']
    del data['attrname']
    del data['name']
    del data["Unnamed: 0"]

    # calculate frame error rate
    s_frag = fragmentSize * 8
    s_frame = s_frag + (64 + 20 + 28 + 36) * 8

    tmp = set(data['module'])
    moduleNumbers = []
    moduleFERs = []
    for s in tmp:
        moduleNumbers.append(s[s.find("[")+1:s.find("]")])
    moduleNumbers.sort()

    for module in moduleNumbers:
        s = "bitErrorRate" + module
        moduleFERs.append("frameErrorRate" + module)
        ber = data[s]
        list_fer = []
        for x in ber:
            if (x not in list_fer):
                list_fer.append(round(1 - (1-x)**(s_frame),1) * 100)
        data[s] = round(1 - (1-data[s])**(s_frame),1) * 100

        data["arbitrationTime"] = data["arbitrationTime"] * 9

        renameMap = {s: "frameErrorRate"+module}
        data = data.rename(renameMap, axis=1, errors="raise")
    renameMap = {"value": "violationRate"}
    data = data.rename(renameMap, axis=1, errors="raise")

    del data['bitErrorRate']


    # simplify data by combining relevant data points (each run)
    column_map = {}
    cols = []
    for col in data.columns:        
        cols.append(col)
        if "frameErrorRate" in col:
            column_map[col] = keep
        elif "violationRate" in col:
            column_map[col] = createListFromLists


    #data = data.groupby(cols, as_index=False).agg(column_map)
    data = data.groupby(['run']).agg(column_map)

    data['fers'] = data[moduleFERs].values.tolist()
    for f in moduleFERs:
        del data[f]




    fig = None
    ax = None
    fig, ax = plt.subplots(figsize=(12.5, 2.75))

    pos = 0
    colors = ['darkblue', 'red', 'lightgreen', 'orange', 'yellow']

    xTicks = []
    xLabels = []
    for index, row in data.iterrows():
        fers = row['fers']
        # fers = fers[:len(fers)-1]

        violationRates = row['violationRate']

        pos0 = pos

        j = 0
        for rate in violationRates:
            bar = plt.bar(pos, rate, width = 0.6, color=colors[j], edgecolor='black')
            j = j + 1
            pos = pos +1

        pos = pos + 1

        if(len(fers) % 2 == 0):
            offset = 0.5
        else:
            offset = 1

        xTicks.append(pos0 + (pos-pos0)/2 - offset)
        xLabels.append(str(fers))
    
    ax.set_xticks(xTicks)
    ax.set_xticklabels(xLabels)
    ax.set_xlabel('Reader FER combinations (%)')

    ax.set_yticks([0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0])
    ax.set_yticklabels(['0','10','20','30','40','50','60','70','80','90','100'])
    ax.set_ylabel('Observed Deadline\nViolation Rate (%)')

    i = 0
    customSymbols = []
    customText = []
    for num in moduleNumbers[:len(moduleNumbers)-1]:
        customSymbols.append(Patch(facecolor=colors[i], edgecolor='black'))
        customText.append("reader " + str(i))
        i = i + 1
    ax.legend(customSymbols, customText, ncol=len(customSymbols), bbox_to_anchor=(0.01,0.99), loc='upper left')


    if(not os.path.exists("figures")):
        os.makedirs("figures")
    plt.savefig("figures/violationRates" + suffix + ".pdf" ,bbox_inches='tight')

    # plt.show()




def plotLatenciesMulticast(data, suffix=''):
    fragmentSize = data['fragmentSize'][0]
    sampleSize = data['sampleSize'][0]
    shapingTime = data['shapingTime'][0]

    del data['fragmentSize']
    del data['sampleSize']
    del data['shapingTime']
    del data['type']
    # and other unnecessary data
    del data['attrvalue']
    del data['attrname']
    del data['name']
    del data["Unnamed: 0"]

    # calculate frame error rate
    s_frag = fragmentSize * 8
    s_frame = s_frag + (64 + 20 + 28 + 36) * 8

    tmp = set(data['module'])
    moduleNumbers = []
    moduleFERs = []
    for s in tmp:
        moduleNumbers.append(s[s.find("[")+1:s.find("]")])
    # moduleNumbers = [int(x) for x in moduleNumbers]
    moduleNumbers.sort()

    for module in moduleNumbers:
        s = "bitErrorRate" + str(module)
        moduleFERs.append("frameErrorRate" + module)
        ber = data[s]
        list_fer = []
        for x in ber:
            if (x not in list_fer):
                list_fer.append(round(1 - (1-x)**(s_frame),1) * 100)
        data[s] = round(1 - (1-data[s])**(s_frame),1) * 100

        data["arbitrationTime"] = data["arbitrationTime"] * 9

        renameMap = {s: "frameErrorRate"+module}
        data = data.rename(renameMap, axis=1, errors="raise")
    renameMap = {"vecvalue": "latencies"}
    data = data.rename(renameMap, axis=1, errors="raise")

    del data['bitErrorRate']

    latenciesCol = []
    slist = data[['latencies']].values.tolist()
    for s in slist:    
        tmp = s[0]
        tmp = tmp.replace('[','').replace(']','').replace('\n', ' ').replace('[','').strip().split(' ')
        tmp = [x for x in tmp if len(x) > 0]
        latencies = list(map(float, tmp))
        latenciesCol.append(latencies)

    data['latencies'] = latenciesCol

    # simplify data by combining relevant data points (each run)
    column_map = {}
    cols = []
    for col in data.columns:        
        cols.append(col)
        if "frameErrorRate" in col:
            column_map[col] = keep
        elif "latencies" in col:
            column_map[col] = createList


    #data = data.groupby(cols, as_index=False).agg(column_map)
    data = data.groupby(['run']).agg(column_map)

    

    data['fers'] = data[moduleFERs].values.tolist()
    for f in moduleFERs:
        del data[f]


    fig = None
    ax = None
    fig, ax = plt.subplots(figsize=(12.5, 2.75))

    pos = 0
    colors = ['darkblue', 'red', 'lightgreen', 'orange', 'yellow']

    xTicks = []
    xLabels = []
    for index, row in data.iterrows():
        fers = row['fers']
        # fers = fers[:len(fers)-1]

        readerLatencies = row['latencies']

        pos0 = pos
        bp = plt.boxplot(readerLatencies, positions = [pos + i for i in range(0,len(readerLatencies))], showfliers=False, patch_artist=True)
        for patch, color in zip(bp['boxes'], colors):
            patch.set_facecolor(color)

        pos = pos + len(readerLatencies) + 1

        if(len(fers) % 2 == 0):
            offset = 0.5
        else:
            offset = 1

        xTicks.append(pos0 + (pos-pos0)/2 - offset)
        xLabels.append(str(fers))
    
    ax.set_xticks(xTicks)
    ax.set_xticklabels(xLabels)
    ax.set_xlabel('Reader FER combinations (%)')

    ax.set_ylabel('Sample Latency (s)')

    i = 0
    customSymbols = []
    customText = []
    for num in moduleNumbers[:len(moduleNumbers)-1]:
        customSymbols.append(Patch(facecolor=colors[i], edgecolor='black'))
        customText.append("reader " + str(i))
        i = i + 1
    ax.legend(customSymbols, customText, ncol=len(customSymbols), bbox_to_anchor=(0.01,0.99), loc='upper left')


    if(not os.path.exists("figures")):
        os.makedirs("figures")
    plt.savefig("figures/latencies" + suffix + ".pdf" ,bbox_inches='tight')




def plotViolationRateTriPointPerLineData(data, frameSize, combined=True):
    # plot violation rate over shaping and arbitration time

    
    slot_time = 9

    data = data[~data.sampleViolationRateVector.isnull()]

    # Data Adaption:
    #data['bit_error_rate'] = np.log10(data['bit_error_rate'])
    data['bitErrorRate'] = (1 - pow(1-data['bitErrorRate'],8*frameSize))*100
    data = data.loc[(data['bitErrorRate'] != -10)]
    data['arbitrationTime'] = data['arbitrationTime'] * slot_time / 2
    
    
    data = data.sort_values(["bitErrorRate", "arbitrationTime"], ascending = (True, True))
    for index, row in data.iterrows():
        print(str(row['bitErrorRate']) + "   " + str(row['arbitrationTime']) + "   " + str(row['sampleViolationRateVector']))

    fig = plt.figure(figsize=(10,3))
    ax = plt.axes(projection='3d')
    surf = ax.plot_trisurf(data['arbitrationTime'],data['bitErrorRate'],  data['sampleViolationRateVector'],cmap="viridis" , linewidth=0.2)
    



    # ax.set(xticklabels=x_vector_labels, xticks=x_vector_ticks)
    plt.xticks(rotation = 60, rotation_mode="anchor")

    y_vector_ticks  = [0, 20, 40, 60, 80]
    y_vector_labels  = ["0", "20", "40", "60", "80"]
    ax.set(yticklabels=y_vector_labels, yticks=y_vector_ticks)
    plt.yticks(rotation = -30, rotation_mode="anchor")

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





def plotLatencies(data):
    i = 0
