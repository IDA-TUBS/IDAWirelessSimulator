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


    data['sampleAverageArbitrationTimeVector'] = data['sampleAverageArbitrationTimeVector']*1000*1000
    
    plt.figure(figsize=(12,3.0))
    sns.set(style="darkgrid", palette="muted", font_scale=1)
    ax =sns.boxplot(x='nbrOfApps', y='sampleAverageArbitrationTimeVector', data=data, dodge=True,  linewidth=1.0, color = "deepskyblue") #, color = "blue")
    ax.tick_params(labelsize=18)
    ax.axhline(1500, ls='--', color="red")
            
    _label = "$\mathregular{\overline{t}_{a,S}^{max}}$"
    ax.set(yticklabels=[0,500,1000,_label,2000,2500,3000], yticks=[0,500,1000,1500,2000,2500,3000],ylim=(-50,3200))
    ax.set_xlabel("number of sample links", fontsize = 20)
    ax.set_ylabel("average arbi-\ntration time \nper sample (\u03BCs)", fontsize = 18)
    

    plt.plot()
    plt.tight_layout()

    plt.savefig(pdffilename)




if __name__ == '__main__':

    data = GraphPlotter.csvToPanda("output/arbitrationTimes.csv")
    
    if not os.path.exists("./figures"):
        os.makedirs("./figures/")
        
    plot_arbitration_time_histogram(data, "./figures/averageArbitrationTimesPerSample.pdf");
    
    
