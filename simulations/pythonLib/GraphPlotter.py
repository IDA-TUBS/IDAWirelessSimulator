#!/usr/bin/env python3
# -*- coding: utf-8 -*- 
import sys
import os
import re
import math
from collections import OrderedDict

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Patch
from matplotlib.lines import Line2D
from matplotlib import cm

import itertools


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



def plotViolationRateUnicast(data, over='sampleDeadline'):
    i = 0
    data = data.reset_index(drop=True)

    over = data[over]
    refs = []
    for x in over:
        factor = 1
        if 'ms' in x:
            factor = 1
        elif 'us' in x:
            factor = 1000
        seq_type= type(x)
        value = float(seq_type().join(filter(seq_type.isdigit, x)))
        refs.append(value / factor)
    violationRates = [x * 100 for x  in data['value']]
    violationRates = [x if x > 0.004 else 0 for x in violationRates ]

    vrDict = {refs[i]: violationRates[i] for i in range(len(refs))}
    vrDict = OrderedDict(sorted(vrDict.items()))
    refs = list(vrDict.keys())
    violationRates = list(vrDict.values())

    print(violationRates)
    
    fig, ax = plt.subplots(figsize=(8, 3))
    ax.set_ylim([0, max(violationRates)*1.1])
    ax.plot(refs, violationRates, clip_on=False, zorder=100)
    ax.scatter(refs, violationRates, clip_on=False, zorder=100)
    ax.set_xlabel('Sample Deadlines (ms)')

    ax.set_ylabel('Observed Deadline\nViolation Rate (%)')    

    plt.savefig("figures/violationRates.pdf"  ,bbox_inches='tight')
    plt.savefig("figures/violationRates.png"  ,bbox_inches='tight')


    fig, ax = plt.subplots(figsize=(4, 2))
    ax.plot(list(refs)[3:], list(violationRates)[3:])
    ax.scatter(list(refs)[3:], list(violationRates)[3:])
    # ax.set_yticks([0, 0.1, 0.2, 0.3, 0.4])

    plt.savefig("figures/violationRatesSmall.pdf"  ,bbox_inches='tight')
    plt.savefig("figures/violationRatesSmall.png"  ,bbox_inches='tight')
    # plt.show()


def plotLatencyAndSlack(data):    
    data = data.reset_index(drop=True)
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
    
    # del data["Unnamed: 0"]

    tmp = set(data['module'])
    moduleNumbers = []
    moduleFERs = []
    for s in tmp:
        moduleNumbers.append(s[s.find("[")+1:s.find("]")])
    moduleNumbers.append("")

    runs = []
    for run in data['run']:
        if run not in runs:
            runs.append(run)
    
    
    for run in runs:
        tmp = data[data['run'] == run]
        tmp = tmp.reset_index(drop=True)

        samplePeriod = tmp['samplePeriod'][0]
        deadline = tmp['sampleDeadline'][0]
        # print(deadline)

        for index, row in tmp.iterrows():
            s = row['vecvalue']
            if isinstance(s, str):
                tmp = s.replace('[','').replace(']','').replace('\n', ' ').replace('[','').strip().split(' ')
                tmp = [x for x in tmp if len(x) > 0]
            else:
                tmp = s
            latencies = list(map(float, tmp))
            latencies = [x * 1000 for x in latencies]

            tmp = latencies.copy()
            tmp.sort()
            topRef = 0.999
            val = tmp[round(len(tmp)*topRef)]
            print(str(topRef*100), "% with latency smaller than ", str(val))

            minLatency = math.floor(min(latencies))
            maxLatency = math.ceil(max(latencies))
            diff = maxLatency - minLatency
            steps = 5
            stepSize = round(diff/steps)

            yticks = [(minLatency + (stepSize * x)) for x in range(0,2*steps) if (minLatency + (stepSize * x)) <= maxLatency + stepSize]

            s = row['vectime']
            if isinstance(s, str):
                tmp = s.replace('[','').replace(']','').replace('\n', ' ').replace('[','').strip().split(' ')
                tmp = [x for x in tmp if len(x) > 0]
            else:
                tmp = s
            times = list(map(float, tmp))

            

            fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(8, 2), gridspec_kw={'width_ratios': [7, 1]})
            # fig.suptitle('Sample Latencies')

            ax1.plot(times, latencies)
            ax1.set_xlabel('Time (s)')
            ax1.set_ylabel('Latency (ms)')
            ax1.set_yticks(yticks)

            ax2.boxplot(latencies)
            ax2.set_xticklabels([" "])
            ax2.set_ylabel('Latency (ms)')
            ax2.set_yticks(yticks)

            plt.savefig("figures/latencies_Deadline" + deadline + "_reader" + str(index) + ".pdf" ,bbox_inches='tight')
            plt.savefig("figures/latencies_Deadline" + deadline + "_reader" + str(index) + ".png" ,bbox_inches='tight')

    


def plotSlack(data):  
    # iterating the columns

    data = data.reset_index(drop=True)
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
    
    # del data["Unnamed: 0"]

    tmp = set(data['module'])
    moduleNumbers = []
    moduleFERs = []
    for s in tmp:
        moduleNumbers.append(s[s.find("[")+1:s.find("]")])
    moduleNumbers.append("")

    runs = []
    for run in data['run']:
        if run not in runs:
            runs.append(run)
    
    slackDict = {}
    
    for run in runs:
        tmp = data[data['run'] == run]
        tmp = tmp.reset_index(drop=True)

        # print(tmp)

        samplePeriod = tmp['samplePeriod'][0]
        deadline = tmp['sampleDeadline'][0]

        if "ms" in deadline:
            d_tmp = deadline.replace("ms","")
            d_tmp = float(d_tmp)            
        elif "us" in deadline:
            d_tmp = deadline.replace("us","")
            d_tmp = float(d_tmp) / 1000
        deadline = d_tmp

        if "ms" in samplePeriod:
            p_tmp = samplePeriod.replace("ms","")
            p_tmp = float(p_tmp)
        elif "us" in samplePeriod:
            p_tmp = samplePeriod.replace("us","")
            p_tmp = float(p_tmp) / 1000
        samplePeriod = p_tmp

        # print("--------------------\n", deadline)


        

        for index, row in tmp.iterrows():
            s = row['vecvalue']
            if isinstance(s, str):
                tmp = s.replace('[','').replace(']','').replace('\n', ' ').replace('[','').strip().split(' ')
                tmp = [x for x in tmp if len(x) > 0]
            else:
                tmp = s
            latencies = list(map(float, tmp))
            latencies = [x * 1000 for x in latencies]

            slackUsage = [lat - samplePeriod if lat > samplePeriod else 0 for lat in latencies]

            index_max = max(range(len(slackUsage)), key=slackUsage.__getitem__)
            trace = slackUsage[index_max: index_max+10]

            i_min = min(range(len(trace)), key=trace.__getitem__)
            # print(i_min)
            trace = trace[:i_min+1]
            # print(index_max, slackUsage[index_max])
            # print("\ttrace: ", trace)
            # print(slackUsage)

            slackDict[deadline] = trace

    sorted_dict = dict(sorted(slackDict.items()))
    # print("\n\n")
    # print(sorted_dict)

    fig, ax = plt.subplots(figsize=(8, 2))
    color = iter(plt.cm.plasma(np.linspace(0, 1, len(sorted_dict.keys()))))

    customLines = []
    for d, trace in sorted_dict.items():
        c = next(color)
        print(d, trace)  
        trace = [math.ceil(x/1.4) for x in trace]

        ax.plot([x for x in range(0,len(trace))], trace, color=c)
        ax.scatter([x for x in range(0,len(trace))], trace, color=c)

        customLines.append(Line2D([0], [0], color=c, lw=2))

    ax.legend(customLines, [str(x) + 'ms' for x in sorted_dict.keys()], ncol=2, bbox_to_anchor=(0.99,0.99), loc='upper right')
    ax.set_xlabel('Sample')
    ax.set_ylabel('Slack\nUsage (slots)')
        
    plt.savefig("figures/slack_decay.pdf" ,bbox_inches='tight')
    plt.savefig("figures/slack_decay.png" ,bbox_inches='tight')
    # plt.show()




def processMissingSamples(dateError, dataMissing, dataStart=None):
    dateError = dateError.reset_index(drop=True)
    dataMissing = dataMissing.reset_index(drop=True)

    for index, row in dataMissing.iterrows():
        deadline = row['sampleDeadline']
        missingSamples = row['vecvalue']
        print(index, row['sampleDeadline'], len(row['vecvalue']))

        if deadline == '66ms':
            print(missingSamples)
            vectimes = row['vectime']
            intervals = []
            for t in vectimes:
                # intervals.append((t-0.2, t-0.1, t, t+0.005))
                intervals.append((t-0.123, t-0.066, t, t+0.005))
            
            # print(intervals)

            errorData = dateError.loc[dateError['sampleDeadline'] == deadline]
            errorTrace = errorData['vecvalue'].tolist()
            errorTs = errorData['vectime'].tolist()

            j = 0
            for interval in intervals:
                # print(interval)
                indices = [i for i,t in enumerate(errorTs[0]) if t > interval[0] and t < interval[3]]
                # print(indices, "\n-------------\n")
                transmissions = errorTrace[0][indices[0]:indices[-1]]
                ts = errorTs[0][indices[0]:indices[-1]]

                fig, ax = plt.subplots(figsize=(8, 1))
                # ax.scatter(ts, transmissions)
                ax.plot(ts, transmissions)
                plt.axvline(x = interval[1], color = 'green', label = '111', linewidth=2)
                plt.axvline(x = interval[2], color = 'red', label = '111', linewidth=2)

                if dataStart is not None:
                    startData = dataStart.loc[dataStart['sampleDeadline'] == deadline]
                    startSNs = startData['vecvalue'].tolist()
                    startTs = startData['vectime'].tolist()
                    # print(startTs)
                    sample = missingSamples[j]
                    # indices = [i for i,sn in enumerate(startSNs[0]) if sn == sample]
                    # print(sample, indices)
                    plt.axvline(x = startTs[0][int(sample)+0] if startTs[0][int(sample)+0] > interval[1] else interval[1], color = 'red', linestyle=':', label = '111', linewidth=2) # TODO why +1 for 66ms deadlines

                    indicesSample = [i for i,t in enumerate(errorTs[0]) if t > startTs[0][int(sample)] and t < interval[2]]
                    transmissionsSample = errorTrace[0][indicesSample[0]:indicesSample[-1]]

                    # burstsSample = []
                    # for key, iter in itertools.groupby(transmissionsSample):
                    #     burstsSample.append((key, len(list(iter))))
                    # print('\n-------------------', len(transmissionsSample), burstsSample,'\n----')

                ax.set_xlabel('t')

                ax.set_yticks([0,1])
                ax.set_yticklabels(["success", "error"])   

                if(not os.path.exists("figures/erroneousSamples")):
                    os.makedirs("figures/erroneousSamples")

                plt.savefig("figures/erroneousSamples/trace" + str(j) + ".pdf"  ,bbox_inches='tight')
                plt.savefig("figures/erroneousSamples/trace" + str(j) + ".png"  ,bbox_inches='tight')


                # also analyse trace for burst error length
                bursts = []
                for key, iter in itertools.groupby(transmissions):
                    bursts.append((key, len(list(iter))))

                # print(len(transmissions), bursts)
                # plt.show()

                j = j + 1
    # plt.show()
    # quit()