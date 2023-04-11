#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path
import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import argparse
import csv

sys.path.append(os.path.abspath('./../../../'))
from pythonLib import GraphPlotter
p_file = os.path.dirname(os.path.abspath(__file__))


def plot_arbitration_time_histogram(data, pdffilename):

    # Colors
    current_palette = sns.color_palette()
    
    # Convert data to ms
    data['packetArbitrationTimesVec'] = data['packetArbitrationTimesVec'] * 1000
    data['averageArbitrationTimeVector'] = data['averageArbitrationTimeVector'] * 1000
    
    taa = data.loc[data['averageArbitrationTimeVector'].idxmax()]['averageArbitrationTimeVector']
    _binwidth = 0.3
    box_x = 2*taa
    box_y = 100 / (box_x/_binwidth)
    print(box_y)
    

    colors = [current_palette[0], current_palette[1], current_palette[2], current_palette[3], current_palette[4], current_palette[5], current_palette[6]]
    plt.figure(figsize=(10,3.0))
    sns.set(style="darkgrid", palette="muted", font_scale=1)
    ax =sns.histplot(data, x='packetArbitrationTimesVec', binwidth = _binwidth,stat='percent', color = current_palette[0])

    
    x_heigh = ((2*taa)/10)   + 0.235    # TODO Adapt
    print(x_heigh)
    y_heigh = box_y/10
    linewidth = 3

    plt.axvline(taa, 0, y_heigh, color = "crimson",linewidth = linewidth)
    plt.axvline(box_x, 0, y_heigh, color = "dimgray",linewidth = linewidth)
    plt.axhline(box_y, 0, x_heigh,  color = "dimgray",linewidth = linewidth)

    ax.tick_params(labelsize=18)

    taa= int(taa*100)/100
    taa2=int(taa*2*100)/100

    ax.set(yticklabels=[0,2,4,6,8,10], yticks=[0,2,4,6,8,10])
    ax.set(xticklabels=[0,1,"$\overline{t}_a=$" + str(taa),3,"$2\cdot\overline{t}_a=$" + str(taa2),5,6], xticks=[0,1,taa,3,2*taa,5,6])
    ax.set_xlabel("Frame arbitration times at MAC layer (ms)", fontsize = 20)
    ax.set_ylabel("Percent", fontsize = 20)

    ax.set(xlim=(0,6.3))

    plt.plot()
    plt.tight_layout()
    plt.savefig(pdffilename)




if __name__ == '__main__':

    data = GraphPlotter.csvToPanda("output/arbitrationTimes.csv")
    
    if not os.path.exists("./figures"):
        os.makedirs("./figures/")
        
    plot_arbitration_time_histogram(data, "./figures/fig3_arbitration_times.pdf");
    
    
