#!/usr/bin/env python3

import os
import sys

import random
import matplotlib.pyplot as plt



def buildSequence(length, p ,r):
    goodState = True
    sequence = []
    for i in range(0,length):
        rnd = random.uniform(0,1)    

        if goodState:
            if rnd > p:
                # > p --> no error
                sequence.append(0)
            else:
                sequence.append(1)
                goodState = False
        elif not goodState:
            if rnd > r:
                # > r --> still an error
                sequence.append(1)
            else:
                sequence.append(0)
                goodState = True


    return sequence

def calcFER(sequence):
    return sequence.count(1) / len(sequence)

def determineBurstCharacteristics(sequence):
    burstLengths = []
    distances = []

    length = 0
    distance = 0
    prevState = -1
    for s in sequence: 
        if s == 0:
            distance = distance + 1

            if prevState == 1:
                burstLengths.append(length)
                length = 0

            prevState = 0
        elif s == 1:
            length = length + 1

            if prevState == 0:
                distances.append(distance)
                distance = 0
            
            prevState = 1

    return burstLengths, distances




def analyseSequence(sequence):
    burstLengths, distances = determineBurstCharacteristics(sequence)

    print(calcFER(sequence))
    print("average burst length", str(sum(burstLengths)/len(burstLengths)))

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(8, 4))
    fig.suptitle('Packet Burst Error Characteristics')

    ax1.hist(burstLengths, 100)
    ax1.set_xlabel('Burst Length')
    ax1.set_ylabel('Frequency')
    ax2.hist(distances, 100)
    ax2.set_xlabel('Inter Burst Distance')

    if(not os.path.exists("figures")):
        os.makedirs("figures")
    plt.savefig("figures/burstCharacteristics.pdf" ,bbox_inches='tight')
    plt.savefig("figures/burstCharacteristics.png" ,bbox_inches='tight')
    plt.show()





if __name__ == '__main__':
    sequences = []

    p = 0.18
    r = 0.40
  
    sequence = buildSequence(100000, p, r)

    burstLengths, distances = determineBurstCharacteristics(sequence)

    print(calcFER(sequence))
    print("average burst length", str(sum(burstLengths)/len(burstLengths)))

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(8, 4))
    fig.suptitle('Packet Burst Error Characteristics')

    ax1.hist(burstLengths, 100)
    ax1.set_xlabel('Burst Length')
    ax1.set_ylabel('Frequency')
    ax2.hist(distances, 100)
    ax2.set_xlabel('Inter Burst Distance')
    # ax2.set_ylabel('Frequency')

    # plt.savefig("burstCharacteristics_p" + str(p) + "_r" + str(r) + ".pdf" ,bbox_inches='tight')
    # plt.savefig("burstCharacteristics_p" + str(p) + "_r" + str(r) + ".png" ,bbox_inches='tight')
    plt.show()

    

