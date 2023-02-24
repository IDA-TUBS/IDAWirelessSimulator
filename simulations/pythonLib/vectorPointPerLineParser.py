import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

import os.path

def parse_if_number(s):
    try: return float(s)
    except: return True if s=="true" else False if s=="false" else s if s else None

def parse_ndarray(s):
    return np.fromstring(s, sep=' ') if s else None


#############################################################################################################
#############################################################################################################
#############################################################################################################
#############################################################################################################
################################ Work on the Vecotrs ########################################################


def process_data_with_panda(par_types, par_synonyms, vec_types, vec_synonyms, path_to_file):

    #Configuration
    parameter_types = par_types
    vector_names = vec_types


    #parameter_types_synonyms
    parameter_types_synonyms = par_synonyms
    vector_names_synonyms = vec_synonyms

    # Load the omnetp data as a vector frame file
    data_frame_vector = pd.read_csv(path_to_file, converters = {
       'attrvalue': parse_if_number,
       'binedges': parse_ndarray,
       'binvalues': parse_ndarray,
       'vectime': parse_ndarray,
       'vecvalue': parse_ndarray})

    # Get all relevant lines with:
    #	- type column == 'config'
    #       - attrname in parameter_types
    param_vector_dataframe = data_frame_vector[(data_frame_vector.type == 'config') & (data_frame_vector.attrname.isin(parameter_types))]

    # Shrink the file to the relevant columns: run, attrname and values - e.g.: | run1 | ber.n | 1e-5 | ... |
    param_vector_dataframe = param_vector_dataframe.loc[param_vector_dataframe.type=='config', ['run', 'attrname', 'attrvalue']]


    # Then bring reformat the csv table into the following form (run is the index)
    # | run  | param1 | param2 | param3 | ... |
    # | run1 | 1e-4   | 2000us | 1000B  | ... |
    param_vector_dataframe = param_vector_dataframe.pivot(index='run', columns='attrname', values='attrvalue')
    print(param_vector_dataframe)
    # Then select only the relevant vector rows from the initial data frame vector
    vector_only_data_frame = data_frame_vector[(data_frame_vector.type=="vector") & (data_frame_vector.name.isin(vector_names))] #XXX if histograms should also be included, do it here!

    # !!! Plausibility Check: Check if a Vector type cannot be found !!!
    for i in range(0,len(vector_names)):
        #for index, row in data_frame_vector.iterrows():
        #    print(row)
        if not data_frame_vector.name.isin([vector_names[i]]).any():
            print("WARNING: Value: \"" + vector_names[i] + "\" does not match any vector in the data")
            #exit()

    # Merge the related parameters to the vector lines
    vector_only_data_frame = vector_only_data_frame.merge(param_vector_dataframe, left_on='run', right_index=True, how='outer') #TODO convert the index here! after merging...

    # Write back intermediate result to csv file
    #vector_only_data_frame.to_csv("./output/relevant_parametriced_vector_lines.csv") #XXX Lange Datensaetze duerfen nicht abgeschnitten werden!


    #########################################################################
    #########################################################################
    ################ No write the relevant output ###########################
    #########################################################################
    #########################################################################

    # Create the column lines vector for the resulting file
    csv_results_file_column_names = parameter_types + vector_names + ["timestamp"] # Concatenate the relevant vectors

    # Create a new csv file where each lines is one data-point
    output_data_field = pd.DataFrame(columns= parameter_types_synonyms + vector_names_synonyms + ["timestamp"])
	
    # Now run through the lines of the current data frame where each line represents a vector output
    # Next init the running variables
    current_row = 0
    current_vector_index = 0
    number_of_rows = vector_only_data_frame.shape[0];
    for index, row in vector_only_data_frame.iterrows():

        print("Reading vector row: " + str(current_vector_index+1) + "/" + str(number_of_rows))
        current_vector_index = current_vector_index + 1

        # First get the fixed parameters
        parameter_values  = [None]*len(parameter_types)    # e.g. fragment_size, bit_error_rate, ...
        for i in range(0,len(parameter_types)):
            parameter_values[i] = row[parameter_types[i]]
       
        # Get the current vector
        if not (row['type'] == 'vector'):
            print("warning no vector")
            continue

        # Get the current Vector, Vectortime and Vectorname
        current_value_vector = row['vecvalue']
        current_value_vector_time = row['vectime']

        vector_name = row['name']
        

        # Calculate fix values of each vector related data point line
        insertion_array = [None]*len(csv_results_file_column_names)
        insertion_array[0:len(parameter_values)] = parameter_values

        print("Vector Name: " + str(vector_name))
        print("Lines: " + str(len(current_value_vector)))

        # Iterate through the vector
        for i in range(0, len(current_value_vector)):

            if i%1000 == 0 :
                print(str(i) + "/" + str(len(current_value_vector)))

            # Csv index to place the data
            value_index = csv_results_file_column_names.index(vector_name)
            time_index = csv_results_file_column_names.index("timestamp")

            # Get the current data value and insert it at the right place
            insertion_array[value_index] = current_value_vector[i]
            insertion_array[time_index] = current_value_vector_time[i]

            # Insert the output_data as a new row into the data frame
            output_data_field.loc[current_row] = insertion_array

            # Increase the current row counter
            current_row = current_row + 1

            
    # Write back the file
    output_data_field.to_csv("./output/omnet_output_vector_data.csv")







