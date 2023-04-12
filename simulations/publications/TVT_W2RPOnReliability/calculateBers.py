import sys
import os
import pandas as pd
import math
import scipy.special


low_ber_frame_size   = 148 + 690
high_ber_frame_size  = 148 + 625
dds_frame_size       = 120 + 667
iteration_times = 1000

print(" Values for low ber case")
for i in range(1,11):

    addressed_fer = i*0.1
    
    if i == 10:
        addressed_fer = 0.247
    
    lower_ber = 0.0
    upper_ber = 1.0
    
    current_ber = 0.5
    
    for k in range(0,iteration_times):
    
        current_ber = (upper_ber - lower_ber)/2 + lower_ber
    
        current_fer = 1 - pow(1-current_ber,8*low_ber_frame_size) 
        
        if current_fer < addressed_fer:
            lower_ber = current_ber
        else:
            upper_ber = current_ber
            
    print("FER: " + str(int(addressed_fer*100)) + "; BER: " + str(current_ber))
 
 
fer_low_0035 = 1 - pow(1-0.000035,8*low_ber_frame_size) 
print("low fer 000035: " + str(fer_low_0035))
 
print()   
print(" Values for high ber case")
for i in range(1,10):

    addressed_fer = i*0.1
    
    if i == 10:
       addressed_fer = 0.3825
        
    lower_ber = 0.0
    upper_ber = 1.0
    
    current_ber = 0.5
    
    for k in range(0,iteration_times):
    
        current_ber = (upper_ber - lower_ber)/2 + lower_ber
    
        current_fer = 1 - pow(1-current_ber,8*high_ber_frame_size) 
        
        if current_fer < addressed_fer:
            lower_ber = current_ber
        else:
            upper_ber = current_ber
            
    print("FER: " + str(int(addressed_fer*100)) + "; BER: " + str(current_ber))
    
fer_low_007 = 1 - pow(1-0.00007,8*high_ber_frame_size) 
print("high fer 0.00007: " + str(fer_low_007))
    
    
    
    
print()   
print(" Values for DDS ber case")
for i in range(1,10):

    addressed_fer = i*0.1
    
    lower_ber = 0.0
    upper_ber = 1.0
    
    current_ber = 0.5
    
    for k in range(0,iteration_times):
    
        current_ber = (upper_ber - lower_ber)/2 + lower_ber
    
        current_fer = 1 - pow(1-current_ber,8*dds_frame_size) 
        
        if current_fer < addressed_fer:
            lower_ber = current_ber
        else:
            upper_ber = current_ber
            
    print("FER: " + str(int(addressed_fer*100)) + "; BER: " + str(current_ber))
    
    
fer_low_0035 = 1 - pow(1-0.000035,8*dds_frame_size) 
print("DDS low fer 000035: " + str(fer_low_0035))
    
    
fer_low_007 = 1 - pow(1-0.00007,8*dds_frame_size) 
print("DDS low fer 0.00007: " + str(fer_low_007))
    
    
    

