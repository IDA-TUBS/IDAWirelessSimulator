import sys
import os
import pandas as pd
import math
import scipy.special


frame_size   = 148 + 800

print(" Values for low standard case")

for addressed_fer in [0.10,0.20,0.3,0.4,0.5,0.6,0.7,0.8,0.9]:
    
    upper_ber = 1
    lower_ber = 0
    current_ber = 0.5
    
    iteration_times = 100
    
    for k in range(0,iteration_times):
    
        current_ber = (upper_ber - lower_ber)/2 + lower_ber
    
        current_fer = 1 - pow(1-current_ber,8*frame_size) 
        
        if current_fer < addressed_fer:
            lower_ber = current_ber
        else:
            upper_ber = current_ber
            
    print("FER: " + str(int(addressed_fer*100)) + "; BER: " + str(current_ber))
 
 
