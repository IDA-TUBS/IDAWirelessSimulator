#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path

sys.path.append(os.path.abspath('./../../../'))
from pythonLib import GraphPlotter

p_file = os.path.dirname(os.path.abspath(__file__))


import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import argparse
import csv

# Import python lib
import os





def plot_efficency_diagram(data, pdffilename):


    #TODO multiply time with 1000
    current_palette = sns.color_palette()
    #sns.palplot(current_palette)

    
    data['shapingTime'] = data['shapingTime'].map(lambda x: x.lstrip('+-').rstrip('us'))
    data['shapingTime'] = data['shapingTime'].astype(int)

    #data['Efficency'] = data['Efficency'].astype(int) * 100

    colors = [current_palette[0], current_palette[1], current_palette[2], current_palette[3], current_palette[4], current_palette[5], current_palette[6]]
    plt.figure(figsize=(10,3.0))
    sns.set(style="darkgrid", palette="muted", font_scale=1)
    ax =sns.boxplot(x='shapingTime', y='sampleEfficiencyVector', hue='bitErrorRate', data=data, dodge=True,
            linewidth=1.0, palette=colors)
    #sns.barplot(x='fragment_size', y ='Lost_Samples', hue = 'bit_error_rate', ax=ax, data=data, xlabel="test")

    ax.set(yticklabels=[0,0.20,0.40,0.60,0.80,1], yticks=[0,0.20,0.40,0.60,0.80,1],ylim=(0,1.05))
    ax.set_xticklabels(ax.get_xticklabels(), rotation=45, horizontalalignment='right')
    ax.tick_params(labelsize=18)
    ax.set(xlim=(-0.5,9))
    ax.set_xlabel("shaping time (\u03BCs)", fontsize = 20)
    ax.set_ylabel("efficiency", fontsize = 20)

    for i,artist in enumerate(ax.artists):
        col = artist.get_facecolor()
        for j in range(i*6,i*6+6):
            line = ax.lines[j]
            line.set_mfc(col)
            line.set_mec(col)

    ax.legend(loc='center right', fontsize=17)
    leg = ax.get_legend()
    leg.set_title("frame error rate",prop={'size':15,'weight':'bold'})
    new_labels = ["0%","30%","50%"]
    for t, l in zip(leg.texts, new_labels): 
        t.set_text(l)



    #fig = plt.gcf()
    plt.plot()
    plt.tight_layout()

#    assert len(pdffilename) > 3 
#    assert pdffilename[-3:].lower() == 'pdf'
    plt.savefig(pdffilename)






if __name__ == '__main__':

    data = GraphPlotter.csvToPanda("output/violationRates.csv")
    
    if not os.path.exists("./figures"):
        os.makedirs("./figures/")
        
    plot_efficency_diagram(data, "./figures/shapingTimeToEfficiency.pdf")
    
    
    
    
    
    
    
    
    
    
    
