#!/usr/bin/env python3
# -*- coding: utf-8 -*- 
import matplotlib.pyplot as plt
import math

if __name__ == '__main__':
    fers = [x for x in range(0,101)]

    P_f = 100
    periods = [x / 10.0 for x in range(4, 21, )]
    print(periods)
    # periods = [0.8]    

    S_s = 412344
    S_f = 11454
    N_f = S_s/S_f
    n = 2

    results = []

    for p in periods:
        # print("----------")
        # print("period: " + str(p))
        N_tx_max = int(math.floor(P_f/p))
        # print(N_tx_max)
    
        fer_max = 0
        for fer in fers:
            # print("---\nfer: " + str(fer/100))
            N_missing_approx = int(math.ceil(N_f * (fer/100)))
            # print(N_missing_approx)

            # N_retr_needed_i = N_missing_approx * sum([(fer/100)**i for i in range(0, 500)])
            # print(N_retr_needed_i)

            if(fer != 1):
                N_retr_needed_i = (math.ceil(N_f*(fer/100)) / (1-fer/100))
            else:
                continue

            if ((N_f + n*N_retr_needed_i) < N_tx_max) and (fer > fer_max):
                fer_max = fer
            
            if ((N_f + n*N_retr_needed_i) > N_tx_max):
                break

        

        print(str(p) + " - max FER: " + (str(fer_max)))
        results.append(fer_max)
    
    fig, ax = plt.subplots(figsize=(6, 2))
    plt.scatter(periods,results, color="blue")
    plt.plot(periods, results, color="blue")

    # plt.ylabel('maximum\nviolation-free FER (%)')
    ax.set_ylabel("maximum\nviolation-free FER (%)", color="blue")
    plt.xlabel('load injection period (ms)')


    # plt.show()

    




    data_rate = 400
    res_vec=[]
    S_a = 64
    for p in periods:
        # print("----------")
        # print("period: " + str(p))
        N_tx_max = int(math.floor(P_f/p))
        # print(N_tx_max)
        
        ta_max = p*1000 - ((((S_f+64+20+28+36)*8)/(data_rate*math.pow(2,20))) * 1000000) - n * ((((S_a+20+28+8)*8)/(data_rate*math.pow(2,20))) * 1000000)
        
        res_vec.append(ta_max)
        
    
    ax2=ax.twinx()
    # fig, ax2 = plt.subplots(figsize=(6, 2))
    ax2.set_ylabel("maximum average\narbitration time (us)",color="red")
    plt.scatter(periods, res_vec, color='red')
    plt.plot(periods, res_vec, color='red')
    
    ax.set_yticks([x*10 for x in range (2,9)])
    ax.set_yticklabels(['20', '', '40', '', '60', '', '80'])
    
    ax2.set_yticks([x*250 for x in range (0,8)])
    # ax2.set_yticklabels(['0', '', '500', '', '1000', '', '1500'])
    
    
    # plt.show()
    
    fig.savefig("figures/limits_ta_fer_n_" + str(n) +".pdf", bbox_inches='tight')