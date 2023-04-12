#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import sys
import os.path
import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import argparse
import csv
import numpy as np
sys.path.append(os.path.abspath('./../../../'))
from pythonLib import GraphPlotter

p_file = os.path.dirname(os.path.abspath(__file__))



def plot_mtbf_data(data,pdffilename):


    print(" ---- mean time between failure -----")
    
    frame_size = 690+148
    period = 0.1

    data = data.sort_values(["bit_error_rate", "arbitr_time"], ascending = (True, True))
    data = data[~data.violation_rate.isnull()]
    # Data Adaption:
    data['bit_error_rate'] = (1 - pow(1-data['bit_error_rate'],8*frame_size))*100
    data = data.loc[(data['bit_error_rate'] != -10)]
    data = data[(data	["bit_error_rate"]<=65)]
    
    
    data = data.sort_values(["bit_error_rate", "arbitr_time"], ascending = (True, True))
    for index, row in data.iterrows():
        print(str(row['bit_error_rate']) + "   " + str(row['arbitr_time']) + "   " + str(row['violation_rate']))
        
    bound = 6245
    data['violation_rate'] = period/(data['violation_rate']/100)
    data['violation_rate'].replace(np.inf, bound, inplace=True)
    data['violation_rate'] = np.where(data['violation_rate']>=bound , bound, data['violation_rate'])
    
    
    data = data.sort_values(["bit_error_rate", "arbitr_time"], ascending = (True, True))
    for index, row in data.iterrows():
        print(str(row['bit_error_rate']) + "   " + str(row['arbitr_time']) + "   " + str(row['violation_rate']))
   
   
    fig = plt.figure(figsize=(10,3))
    ax = fig.gca(projection='3d')
    surf = ax.plot_trisurf(data['arbitr_time'],data['bit_error_rate'],  data['violation_rate'],cmap="viridis_r" , linewidth=0.2) #cm.viridis
    label_fontsizes = 12
    axis_label_fontsize = 13

    # x labels
    x_vector_ticks = [11,33,56,78,100,122,145,167,189,211,233,256,278,300,322,345,366,389,411,433]
    _label_taamax = "\n1300,$\mathregular{\overline{t}^{max}_{a,S}}$          \n $\mathregular{\leq 1450us}$    "
    _label_taamax = "$\mathregular{\overline{t}_{a,S} \leq \overline{t}^{max}_{a,S}}$          "
    x_vector_labels = ["100","","300","","50","","70","","90","","1100","","1300","","1500","","1700","","1900",""]
    vector_min = min(x_vector_ticks)
    vector_max = max(x_vector_ticks)
    ax.set_xticklabels(ax.get_xticklabels(), fontsize = label_fontsizes-2,rotation=20,verticalalignment = 'center')
    ax.set(xticklabels=x_vector_labels, xticks=x_vector_ticks,xlim=(vector_max+5,vector_min-5))  
    plt.xlabel('', horizontalalignment='left', position=(120,25))
    ax.set_xlabel("", fontsize = 11)
    
    # y labels
    y_vector_ticks  = [0,10,20,30,35.14,40,50,60]
    y_vector_labels  = [0,10,20,30,"",40,50,60]
    ax.set(yticklabels=y_vector_labels, yticks=y_vector_ticks,ylim=(-5,65)) 
    ax.set_ylabel("", fontsize = 11)


    # z labels
    _label = "   $\mathregular{\geq5556}$"
    ax.set(zticklabels=[_label,5000,4000,3000,2000,1000,0.1], zticks=[bound,5000,4000,3000,2000,1000,0.1],zlim=(bound,-50))
    ax.set_zticklabels(ax.get_zticklabels(), rotation=-6, fontsize = label_fontsizes-2)
    ax.set_zlabel("", fontsize = 11)
    
    
    
    ax.set_xlabel("average arbitration time (\u03BCs)", rotation=0, fontsize = 9)
    ax.set_ylabel("frame error rate (%)", rotation=0, fontsize = 9)
    ax.set_zlabel("observed MTBF (s)", rotation=0, fontsize = 9)

    
    plt.tight_layout()
    plt.savefig(pdffilename)
    return





if __name__ == '__main__':

    data = GraphPlotter.csvToPanda("output/violationRates.csv")
    
    if not os.path.exists("./figures"):
        os.makedirs("./figures/")
        
    
    plot_mtbf_data(data, "./figures/fig10b_W2RPMeanTimeBetweenFailures.pdf")
    
    
    
    
    
