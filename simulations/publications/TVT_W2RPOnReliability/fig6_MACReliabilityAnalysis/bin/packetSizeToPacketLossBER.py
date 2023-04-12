import sys
import os
import pandas as pd

nbr_retransmissions = 3
FER = [0.0,0.10,0.20,0.30,0.40,0.50,0.60,0.70,0.80,0.90,1]

df = pd.DataFrame(columns= ['fer', 'frame_drop_rate','sample_drop_rate'])

current_row = 0

for fer in FER:

     row_val = [None]*3

     row_val[0] = fer
     row_val[1] = pow(fer,1+nbr_retransmissions)
     row_val[2] = 1- pow(1-row_val[1],25)

     df.loc[current_row] = row_val
     current_row = current_row + 1



# Create folder if it does not exist
path = "./results"
exists = os.path.exists(path)
if not exists:
   os.makedirs(path)
   
df.to_csv("./results/fer_to_per.csv")





