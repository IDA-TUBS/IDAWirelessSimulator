#!/usr/bin/env python3 

import sys
import os.path

import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd

sys.path.append(os.path.abspath('./../../../'))
from pythonLib import GraphPlotter


def plot_violation_rate_selected_ta(data,pdffilename):

    print("--- plot violation rates ---")

    fragment_size = 800
    dds_protocol_overhead = 120
    frame_size = fragment_size + dds_protocol_overhead

    # Filter for valid values
    data = data[~data.sampleViolationRateVector.isnull()]

    # Filter arbitration times
    t_a_1 = 178 # ~ 800us --> undisturbed
    t_a_2 = 235 # ~1058us --> disturbed
    data['arbitrationTime'] = data['arbitrationTime'].astype(int)
    data = data.loc[(data['arbitrationTime'] == t_a_1) | (data['arbitrationTime'] == t_a_2)]
    
    # Convert violation rates to % values
    data['sampleViolationRateVector'] = data['sampleViolationRateVector'] * 100

    # Convert bit error rate to frame error rate
    data['bitErrorRate'] = (1 - pow(1-data['bitErrorRate'],8*frame_size))*100

    # Debug
    data = data.sort_values(["arbitrationTime", "bitErrorRate"], ascending=[True,True])
    for index, row in data.iterrows():
        print(str(row['bitErrorRate']) + "   " + str(row['arbitrationTime']) + "   " + str(row['sampleViolationRateVector']))

    # Colors
    current_palette = sns.color_palette()
    colors = [current_palette[2], current_palette[3]]

    plt.figure(figsize=(10,3.0))
    sns.set(style="darkgrid", palette="muted", font_scale=1)
    ax = sns.pointplot(data=data, x="bitErrorRate", y="sampleViolationRateVector",hue="arbitrationTime", palette=colors)

    ax.set(yticklabels=[0,20,40,60,80,100], yticks=[0,20,40,60,80,100],ylim=(-5,105))
    ax.set(xticklabels=[0,10,20,30,40,50,60,70,80,90] , xticks=[0,1,2,3,4,5,6,7,8,9], xlim=(-0.5,9.5))
    
    ax.tick_params(labelsize=18)
    ax.set_xlabel("frame error rate (%)", fontsize = 17)
    ax.set_ylabel("deadline\nviolation rate (%)", fontsize = 17)

    plt.legend(loc='upper left', fontsize=17)
    leg = ax.get_legend()
    leg.set_title("     average\narbitration time",prop={'size':16,'weight':'bold'})

    new_labels = ["800 (\u03bcs)","1058(\u03bcs)"]
    for t, l in zip(leg.texts, new_labels): 
        t.set_text(l)

    plt.plot()
    plt.tight_layout()
    plt.savefig(pdffilename)
    
    
    
if __name__ == '__main__':

    data = GraphPlotter.csvToPanda("output/violationRates.csv")
    
    if not os.path.exists("./figures"):
        os.makedirs("./figures/")
        
    plot_violation_rate_selected_ta(data,"./figures/63_eval_DDS_with_MAC_unbounded_interference.pdf")
    
    
    
    
    
    
