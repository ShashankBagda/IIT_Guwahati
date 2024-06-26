#*******************************************************************************
# Copyright (c) 2021-2025
# School of Electrical, Computer and Energy Engineering, Arizona State University
# Department of Electrical and Computer Engineering, University of Wisconsin-Madison
# PI: Prof. Yu Cao, Prof. Umit Y. Ogras, Prof. Jae-sun Seo, Prof. Chaitali Chakrabrati
# All rights reserved.
#
# This source code is part of SIAM - a framework to benchmark chiplet-based IMC 
# architectures with synaptic devices(e.g., SRAM and RRAM).
# Copyright of the model is maintained by the developers, and the model is distributed under
# the terms of the Creative Commons Attribution-NonCommercial 4.0 International Public License
# http://creativecommons.org/licenses/by-nc/4.0/legalcode.
# The source code is free and you can redistribute and/or modify it
# by providing that the following conditions are met:
#
#  1) Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#
#  2) Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Developer list:
#		Gokul Krishnan Email: gkrish19@asu.edu
#		Sumit K. Mandal Email: skmandal@wisc.edu
#
# Acknowledgements: Prof.Shimeng Yu and his research group for NeuroSim
#*******************************************************************************/

# -*- coding: utf-8 -*-
"""
Created on Fri Sep 24 09:57:35 2021

"""

import pandas as pd
import numpy as np
import math
import os
import shutil
import csv

#Take all below parameters as argument
# quantization_bit = 8
# bus_width = 32
# netname = 'ResNet110' #'VGG-19_45.3M'
# xbar_size = 256
# chiplet_size = 16
# num_chiplets = 25
type = 'Homogeneous_Design'
scale = 10
quantization_bit = 8
# num_writes = 1499648

def generate_traces_dram(bus_width, netname, chiplet_size, num_chiplets, scale):

    # directory_name = netname + '/' + type + '/' + str(num_chiplets) + '_Chiplets_' + str(chiplet_size) + '_Tiles/to_interconnect'
    #directory_name = '/home/gkrish19/SIAM_Integration/to_interconnect'
    directory_name = '/home/neethu/tcas/SIAM/to_interconnect'
    tiles_csv_file_name = directory_name + '/num_tiles_per_layer_chiplet.csv'
    num_tiles_each_layer = pd.read_csv(tiles_csv_file_name, header=None)
    num_tiles_each_layer = num_tiles_each_layer.to_numpy()
    num_tiles_each_layer = num_tiles_each_layer[:, 2]
    
    activation_csv_file_name = directory_name + '/ip_activation.csv'
    num_activations_per_layer = pd.read_csv(activation_csv_file_name, header=None)
    num_activations_per_layer = num_activations_per_layer.to_numpy()
    num_actv_current_layer = num_activations_per_layer[:,0]
    
    chiplet_breakup_file_name = directory_name + '/chiplet_breakup.csv'
    data = pd.read_csv(chiplet_breakup_file_name, header=None)
    data = data.to_numpy()
    tile_begin_array = data[:, 0]
    tile_end_array = data[:, 1]
    num_chiplets_this_layer = data[:, 2]
    
    # Load the csv file into a pandas dataframe
    network_csv_file_name = '/home/neethu/tcas/SIAM/SIAM/NetWork.csv'
    df = pd.read_csv(network_csv_file_name, header=None)
    weights_layer = df.iloc[:, 3] * df.iloc[:, 4] * df.iloc[:, 5] * quantization_bit 
        
    dir_name = '/home/neethu/tcas/SIAM/Interconnect/' +  netname + '_DRAM_traces' + '/' + type + '_' + str(num_chiplets) + '_cnt_size_' +  str(chiplet_size) + '_scale_' + str(scale) + '_bus_width_' + str(bus_width)
            
    
    if (os.path.isdir(dir_name)):
        shutil.rmtree(dir_name)
    
    os.makedirs(dir_name)
    # os.chdir(dir_name);
    num_chiplets_used = len(tile_begin_array)
    # num_chiplets_used = sum(num_chiplets_this_layer)
    nop_mesh_size = np.zeros([num_chiplets_used, 1])

    chiplet_idx = 0
    # read table data from csv file
    with open('/home/neethu/tcas/SIAM/to_interconnect/num_tiles_per_layer_chiplet.csv', 'r') as f:
        table_data = list(csv.reader(f))
        num_tiles_each_layer = [row[2] for row in table_data]

    # define parameters
    timestamp = 1

    # initialize variables
    scale = 10
     
    for chiplet_idx in range(0, num_chiplets_used):
        print("DRAM trace generation in Chiplet_",chiplet_idx)
        begin_layer = tile_begin_array[chiplet_idx] 
        end_layer = tile_end_array[chiplet_idx] + 1
        
        # num_tiles_this_chiplet = sum(num_tiles_each_layer[begin_layer:end_layer])
        num_tiles_this_chiplet = sum(int(val) for val in num_tiles_each_layer)
        nop_mesh_size[chiplet_idx] = math.ceil(math.sqrt(num_chiplets))
        
        os.chdir(dir_name);
        chiplet_dir_name = 'Chiplet_' + str(chiplet_idx)
        os.mkdir(chiplet_dir_name)
              
        for layer_idx in range (begin_layer, end_layer):
            
            # print("DRAM trace generation in Chiplet_", chiplet_idx, " for layer_", layer_idx)
            
            trace = np.array([[0,0,0]])
            timestamp = 1
            
            ip_activation_this_layer = num_activations_per_layer[layer_idx]
            num_packets_this_layer = math.ceil(ip_activation_this_layer/(bus_width*scale))

            for packet_idx in range(0, num_packets_this_layer):
                # print (' DRAM num_packets_this_layer: ', packet_idx , ' of ' , num_packets_this_layer )
                # for src_chiplet_idx in range(0,num_chiplets_used):                    
                if packet_idx <= math.ceil(num_packets_this_layer/2):
                    dest_chiplet_idx = num_chiplets - 2
                else:
                    dest_chiplet_idx = num_chiplets - 1
                trace = np.append(trace, [[chiplet_idx, dest_chiplet_idx, timestamp]], axis=0)
                    # print('layer_idx :', layer_idx , 'packet_idx :', packet_idx , 'src_tile_idx :', src_chiplet_idx , 'dest_tile_idx:', dest_chiplet_idx, 'timestamp :',timestamp)
                    
                timestamp = timestamp + 1
            
            filename = 'trace_file_dram_layer_' + str(layer_idx) + '.txt'
            # filename = 'trace_file_chiplet_' + str(layer_idx) + '.txt'
            
            
            trace = np.delete(trace, 0, 0)
            
            # if (chiplet_idx != num_chiplets):
                # trace = np.delete(trace, 0, 0)
            os.chdir(chiplet_dir_name)
            np.savetxt(filename, trace, fmt='%i')
            os.chdir("..")
    
    # os.chdir(dir_name)

    np.savetxt('nop_mesh_size_dram.csv', nop_mesh_size, fmt='%i')
    os.chdir("../..")
    # os.chdir("..")
