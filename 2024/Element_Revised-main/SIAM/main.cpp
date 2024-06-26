/*******************************************************************************
* Copyright (c) 2021-2025
* School of Electrical, Computer and Energy Engineering, Arizona State University
* Department of Electrical and Computer Engineering, University of Wisconsin-Madison
* PI: Prof. Yu Cao, Prof. Umit Y. Ogras, Prof. Jae-sun Seo, Prof. Chaitali Chakrabrati
* All rights reserved.
*
* This source code is part of SIAM - a framework to benchmark chiplet-based IMC 
* architectures with synaptic devices(e.g., SRAM and RRAM).
* Copyright of the model is maintained by the developers, and the model is distributed under
* the terms of the Creative Commons Attribution-NonCommercial 4.0 International Public License
* http://creativecommons.org/licenses/by-nc/4.0/legalcode.
* The source code is free and you can redistribute and/or modify it
* by providing that the following conditions are met:
*
*  1) Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*
*  2) Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation
*     and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Developer list:
*		Gokul Krishnan Email: gkrish19@asu.edu
*		Sumit K. Mandal Email: skmandal@wisc.edu

* Acknowledgements: Prof.Shimeng Yu and his research group for NeuroSim
********************************************************************************/

#include <cstdio>
#include <random>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <chrono>
#include <algorithm>
#include "constant.h"
#include "formula.h"
#include "Param.h"
#include "Tile.h"
#include "Chip.h"
#include "ProcessingUnit.h"
#include "SubArray.h"
#include "Definition.h"

using namespace std;

vector<vector<double> > getNetStructure(const string &inputfile);
vector<vector<double>> slicing(vector<vector<double>>& arr, int X, int Y);

int main(int argc, char * argv[]) {

	auto start = chrono::high_resolution_clock::now();

	gen.seed(0);

	vector<vector<double> > netStructure;
	netStructure = getNetStructure(argv[2]); //nocs (argv[1]);

	/* Create SubArray object and link the required global objects (not initialization) */
	inputParameter.temperature = param->temp;   // Temperature (K)
	inputParameter.processNode = param->technode;    // Technology node
	tech.Initialize(inputParameter.processNode, inputParameter.deviceRoadmap, inputParameter.transistorType);

	// Declarations Common to both Cases	
	double maxPESizeNM, maxTileSizeCM, numPENM, desiredNumTileNM, desiredPESizeNM, desiredNumTileCM, desiredTileSizeCM, desiredPESizeCM;
	// double desiredNumTileNM, desiredPESizeNM, desiredNumTileCM, desiredTileSizeCM, desiredTileSizeCM_x, desiredTileSizeCM_y, desiredPESizeCM, desiredPESizeCM_x, desiredPESizeCM_y;
	int numTileRow, numTileCol;
	int numArrayWriteParallel; //nocs
	vector<vector<double> > numTileEachLayer, utilizationEachLayer, speedUpEachLayer, tileLocaEachLayer;
	// vector<vector<double> > utilizationEachLayer;
	// vector<vector<double> > speedUpEachLayer;
	// vector<vector<double> > tileLocaEachLayer;
	vector<int> markNM;
	vector<int> pipelineSpeedUp;
	double totalNumTile = 0, used_totalNumTile = 0;
	double avg_utilization_chiplet=0;
	double realMappedMemory = 0;
	double numComputation = 0;
	double chipHeight, chipWidth, chipArea, chipAreaIC, chipAreaADC, chipAreaAccum, chipAreaOther, chipArealocalbuffer,chipAreaglobalbuffer, chipAreaOtheralone, chipAreaglobalIC, chipAreatileIC, chipAreasubarrayalone, chipAreaWG, chipAreaArray; //nocs last 2 added
	chipAreasubarrayalone=0;
	double CMTileheight = 0;
	double CMTilewidth = 0;
	double NMTileheight = 0;
	double NMTilewidth = 0;
	vector<double> chipAreaResults, chipAreaResults_common;
	double chipReadLatency = 0;
	double chipReadDynamicEnergy = 0;
	double chipLeakageEnergy = 0;
	double chipLeakage = 0;
	double chipbufferLatency = 0;
	double chipbufferReadDynamicEnergy = 0;
	//double chipAreasubarrayalone =0;
	double chipicLatency = 0;
	double chipicReadDynamicEnergy = 0;
	double global_iclatency = 0;
	double chipLatencyADC = 0;
	double chipLatencyAccum = 0;
	double chipLatencyOther = 0;
	double chipEnergyADC = 0;
	double chipEnergyAccum = 0;
	double chipEnergyOther = 0;
	double layerReadLatency = 0;
	double avg_layerReadLatency = 0;
	int count = 0;
	double layerReadDynamicEnergy = 0;
	double tileLeakage = 0;
	double layerbufferLatency = 0;
	double layerbufferDynamicEnergy = 0;
	double layericLatency = 0;
	double layericDynamicEnergy = 0;
	double tile_total =0;
	double coreLatencyOther_only= 0;
	double coreEnergyOther_only = 0;
	double coreLatencyADC = 0;
	double coreLatencyAccum = 0;
	double coreLatencyOther = 0;
	double coreEnergyADC = 0;
	double coreEnergyAccum = 0;
	double coreEnergyOther = 0;
	double Global_accum_est_lat=0;
	double Global_accum_est_energy=0;
	double max_glob_acc_lat = 0;
	double max_glob_acc_energy = 0;
	double global_routinglatency_1= 0;
	double local_routinglatency_1= 0;
	double global_bufferlatency_1= 0;
	double local_bufferlatency_1= 0;
	double coreLatencyOther_only_1 =0;
	double global_bufferenergy_1= 0;
	double coreEnergyOther_only_1 =0;
	double local_bufferenergy_1 = 0;
	double global_routingenergy_1 = 0;
	double local_routingenergy_1 = 0;
	double global_routinglatency, test, local_bufferlatency, local_bufferenergy, local_routinglatency, local_routingenergy, global_routingenergy, global_bufferlatency, global_bufferenergy = 0;
	double numTileOtherLayer = 0;
	double layerLeakageEnergy = 0;	
	//nocs
	double chipReadLatencyAG = 0;
	double chipReadDynamicEnergyAG = 0;
	double chipReadLatencyWG = 0;
	double chipReadDynamicEnergyWG = 0;
	double chipWriteLatencyWU = 0;
	double chipWriteDynamicEnergyWU = 0;
	
	double chipReadLatencyPeakFW = 0;
	double chipReadDynamicEnergyPeakFW = 0;
	double chipReadLatencyPeakAG = 0;
	double chipReadDynamicEnergyPeakAG = 0;
	double chipReadLatencyPeakWG = 0;
	double chipReadDynamicEnergyPeakWG = 0;
	double chipWriteLatencyPeakWU = 0;	
	double chipWriteDynamicEnergyPeakWU = 0;


	double chipDRAMLatency = 0;
	double chipDRAMDynamicEnergy = 0;

	double layerReadLatencyAG = 0;
	double layerReadDynamicEnergyAG = 0;
	double layerReadLatencyWG = 0;
	double layerReadDynamicEnergyWG = 0;
	double layerWriteLatencyWU = 0;
	double layerWriteDynamicEnergyWU = 0;
	
	double layerReadLatencyPeakFW = 0;
	double layerReadDynamicEnergyPeakFW = 0;
	double layerReadLatencyPeakAG = 0;
	double layerReadDynamicEnergyPeakAG = 0;
	double layerReadLatencyPeakWG = 0;
	double layerReadDynamicEnergyPeakWG = 0;
	double layerWriteLatencyPeakWU = 0;
	double layerWriteDynamicEnergyPeakWU = 0;
	
	double layerDRAMLatency = 0;
	double layerDRAMDynamicEnergy = 0;

	//*****************************************************
	// Accumulator and Buffer at Chiplet Level Variables
	vector<int> l;
	double area=0, latency=0, energy=0;
	//*****************************************************
	
	//*****************************************************

	// define weight/input/memory precision from wrapper
		param->synapseBit = atoi(argv[3]); // nocs atoi(argv[2]);              // precision of synapse weight
		param->numBitInput = atoi(argv[4]); // nocs atoi(argv[3]);             // precision of input neural activation
		if (param->cellBit > param->synapseBit) {
			cout << "ERROR!: Memory precision is even higher than synapse precision, please modify 'cellBit' in Param.cpp!" << endl;
			param->cellBit = param->synapseBit;
		}
		param->numColPerSynapse = ceil((double)param->synapseBit/(double)param->cellBit);
		param->numRowPerSynapse = 1;
	if (param->mode)
	{
		//******************************************************** This is the Chiplet Chip Mode*********************************************************************
		//***********************************************************************************************************************************************************
		cout<<"*********************Starting Chiplet Partition*********************"<<endl;
		int chip_size, chip_cnt, residue_area_cal;
		chip_size = param->size_chiplet; 			// Gives the number of RRAM arrays in the each chiplet. We assume all the chiplets are same size
		chip_cnt = 	param->cnt_chiplet; 	
		int start = 0, end = 0, num_chip=0;
		int num_chip_final = 0;
		vector<int> start_array, end_array, split;
		vector<vector<int>> chiplet_structure;
		double numtileEachLayerRow = 0;
		double numTileEachLayerCol = 0;
		int i = 0;
		double desiredTileSizeCM;
		double numTileTotal, matrixTotalCM, total_weights_per_chiplet, per_layer_tile_cnt;
		desiredTileSizeCM = param->row_multiplier*param->numRowSubArray;
		// desiredTileSizeCM_y = param->col_multiplier*param->numColSubArray;
		int numRowPerSynapse, numColPerSynapse;
		numRowPerSynapse = param->numRowPerSynapse;
		numColPerSynapse = param->numColPerSynapse;
		int excess = 1, max_excess = 1;
		int set = 0;
		cout<<"The size of the network is: "<<netStructure.size()<<endl;
		while (i<netStructure.size())
		{
			// cout<<"Layer we are partition testing is: "<<i<<endl;

			// numTileTotal += ceil(((double) netStructure[i][2]*(double) netStructure[i][3]*(double) netStructure[i][4]*(double) numRowPerSynapse)/(double) tileSize_x) * ceil(netStructure[i][5]*numColPerSynapse/(double) tileSize_y);
			// matrixTotalCM += netStructure[i][2]*netStructure[i][3]*netStructure[i][4]*numRowPerSynapse*netStructure[i][5]*numColPerSynapse;
			if (netStructure[i][0] < netStructure[i][3])
			{
				numtileEachLayerRow = ceil((double) netStructure[i][2]*(double) (netStructure[i][0])*(double) netStructure[i][1]*(double) numRowPerSynapse/desiredTileSizeCM);
				double numtileEachLayerCol = ceil((double) netStructure[i][5]*(double) numColPerSynapse/(double) desiredTileSizeCM);
				numTileTotal += numtileEachLayerRow * numtileEachLayerCol;
				per_layer_tile_cnt = numtileEachLayerRow * numtileEachLayerCol;
			}

			else
			{
				numtileEachLayerRow = ceil((double) netStructure[i][2]*(double) netStructure[i][3]*(double) netStructure[i][4]*(double) numRowPerSynapse/desiredTileSizeCM);
				double numtileEachLayerCol = ceil((double) netStructure[i][5]*(double) numColPerSynapse/(double) desiredTileSizeCM);
				numTileTotal += numtileEachLayerRow * numtileEachLayerCol;
				per_layer_tile_cnt = numtileEachLayerRow * numtileEachLayerCol;
			}
			
			// numTileTotal += numtileEachLayerRow * numtileEachLayerCol; Need to see why this is failing with compile error !!

			if (numTileTotal>chip_size)
			{
				if (per_layer_tile_cnt <= chip_size)
				{
					if (i==0)
					{
						cout<<"We are setting the first chiplet such that the whole first layer is inside it."<<endl;
						numTileTotal = 0;
						matrixTotalCM = 0;
						start_array.push_back(0);
						end_array.push_back(0);
						split.push_back(1);
						start = i+1;
						cout<<"Layer "<<i<<" is in the chiplet "<< num_chip << endl;
						num_chip+=1;
						num_chip_final +=1;

					}
					
					else
					{
						// cout<<"NumtileTotal when we partition is "<< numTileTotal<<endl;
						end = i-1;		
						numTileTotal = 0;
						matrixTotalCM = 0;
						start_array.push_back(start);
						end_array.push_back(end);
						split.push_back(1);
						cout<<"Layers "<<start<<" to "<<end<<" is in the chiplet "<< num_chip << endl;
						start = i;
						num_chip+=1;
						num_chip_final+=1;
					}
				}

				else
				{
					cout<<"Warning: The layer is too big for a single chiplet. Need to break layer between different chiplets for homogeneous tile mapping"<<endl;
					// cout<<"NumtileTotal when we partition is "<< numTileTotal<<endl;
					if (numTileTotal>per_layer_tile_cnt)
					{
						end = i-1;		
						numTileTotal = 0;
						matrixTotalCM = 0;
						start_array.push_back(start);
						end_array.push_back(end);
						split.push_back(1);
						cout<<"Layers "<<start<<" to "<<end<<" is in the chiplet "<< num_chip << endl;
						num_chip+=1;
						num_chip_final+=1;

						start+=1;
						end+=1;
						start_array.push_back(start);
						end_array.push_back(end);
						excess = ceil(per_layer_tile_cnt/chip_size);
						if (excess>max_excess)
						{
							max_excess = excess;
						}
						l.push_back(i);
						split.push_back(excess);
						cout<<"Layers "<<start<<" to "<<end<<" is in the chiplet "<< num_chip << endl;
						num_chip+=1;
						num_chip_final+=excess;
						if (i == netStructure.size()-1)
						{
							cout<<"Last layer"<<endl;
							set=1;
						}
						i+=1;
						start = i;
					}
					else
					{
						cout<<"There is no residue layer from previous itteration to be partitioned."<<endl;
						end=i;
						numTileTotal = 0;
						matrixTotalCM = 0;
						start_array.push_back(start);
						end_array.push_back(end);
						excess = ceil(per_layer_tile_cnt/chip_size);
						if (excess>max_excess)
						{
							max_excess = excess;
						}
						split.push_back(excess);
						l.push_back(i);
						cout<<"Layers "<<start<<" to "<<end<<" is in the chiplet "<< num_chip << endl;
						num_chip+=1;
						num_chip_final+=excess;
						if (i == netStructure.size()-1)
						{
							cout<<"Last layer"<<endl;
							set=1;							
						}
						i+=1;
						start = i;
					}											
				}		
			}
			else if (i == netStructure.size()-1)
			{
				end = i;
				start_array.push_back(start);
				end_array.push_back(end);
				split.push_back(1);
				cout<<"Last Layer"<<endl;
				cout<<"Layers "<<start<<" to "<<end<<" is in the chiplet "<< num_chip << endl;
				i+=1;
				//Num Chip starts from 0 so no need to add it here
			}
			else
			{
				// cout<<"NumtileTotal when we partition is "<<numTileTotal<<endl;
				cout<<"Layer "<<i<<" is in the chiplet "<< num_chip << endl;
				i+=1;
			}			
		}

		if (set)
		{
			num_chip = num_chip - 1;
			num_chip_final = num_chip_final-1;
		}
		
		chiplet_structure.push_back(start_array);
		chiplet_structure.push_back(end_array);
		vector<vector<double> > netStructure_chiplet;
		cout<<"**************************************************************"<<endl;
		cout<<"Number of chiplets being used to map this DNN is:"<<(num_chip+1)<<endl;
		cout<<"**************************************************************"<<endl;

		if (param->fixed_cnt_chiplet)
		{
			if ((num_chip_final+1) > param->cnt_chiplet)
			{	
				cout<<"Error: The current configuration results in a large number of chiplets, beyond the limit set by the user"<<endl;
				cout<<"Error: We need "<<num_chip_final+1<<" chiplets to map this network. Please change the param file."<<endl;
				exit(1);
			}
		}

		std::ofstream myfile_dump;
			myfile_dump.open ("./to_interconnect/chiplet_breakup.csv"); //Dumps file for the chiplet breakup for BookSim.
		for(int i=0; i<(num_chip+1); i++) 
		{
			// myfile_dump << "Chiplet_"+ std::to_string(i+1) <<"," <<"Start"<<","<<chiplet_structure[0][i] <<","<<"End"<<","<< chiplet_structure[1][i] << endl;
			myfile_dump << chiplet_structure[0][i] <<","<< chiplet_structure[1][i] <<","<<split[i]<< endl;
		}
		myfile_dump.close();

		for(int i=0; i<(num_chip+1); i++)
		{	
			totalNumTile, numComputation, chipHeight, chipWidth, chipArea, chipAreaIC, chipAreaADC, chipAreaAccum, chipAreaOther, chipArealocalbuffer,chipAreaglobalbuffer, chipAreaOtheralone, chipAreaglobalIC, chipAreatileIC, chipAreasubarrayalone = 0;
			chipAreasubarrayalone, CMTileheight, CMTilewidth, NMTileheight, NMTilewidth, used_totalNumTile=0;
			chipReadLatency, chipReadDynamicEnergy, chipLeakageEnergy, chipLeakage, chipbufferLatency, chipbufferReadDynamicEnergy, chipicLatency, chipicReadDynamicEnergy, global_iclatency, chipLatencyADC = 0;
			chipLatencyAccum, chipLatencyOther, chipEnergyADC, chipEnergyAccum, chipEnergyOther, layerReadLatency, avg_layerReadLatency, count, layerReadDynamicEnergy, tileLeakage, layerbufferLatency, layerbufferDynamicEnergy, layericLatency = 0;
			layericDynamicEnergy, tile_total, coreLatencyOther_only, coreEnergyOther_only, coreLatencyADC, coreLatencyAccum, coreLatencyOther, coreEnergyADC, coreEnergyAccum, coreEnergyOther, Global_accum_est_lat = 0;
			Global_accum_est_energy, max_glob_acc_lat, max_glob_acc_energy, global_routinglatency_1, local_routinglatency_1, global_bufferlatency_1, local_bufferlatency_1, coreLatencyOther_only_1 =0;
			global_bufferenergy_1, coreEnergyOther_only_1, local_bufferenergy_1, global_routingenergy_1, local_routingenergy_1, numTileOtherLayer, layerLeakageEnergy = 0;
			global_routinglatency, test, local_bufferlatency, local_bufferenergy, local_routinglatency, local_routingenergy, global_routingenergy, global_bufferlatency, global_bufferenergy = 0;
			// avg_utilization_chiplet = 0;
			realMappedMemory = 0;
			start = chiplet_structure[0][i]; // within a chiplet start and end layer number
			end = chiplet_structure[1][i];
			
			cout<<"**************************************************************"<<endl;
			cout<<"Layers in Chiplet "<<i<<" are "<<start<<" to "<<end<<endl;
			cout<<"**************************************************************"<<endl;
			netStructure_chiplet = slicing(netStructure, start, end); // structure of layer shape in each chiplet (basically from NetWork.csv)
			// netStructure_chiplet = netStructure[std::slice(start, end-start+1, 1)][std::slice(netStructure[1].begin(), netStructure[1].end(), 1)];

			markNM = ChipDesignInitialize(inputParameter, tech, cell, false, netStructure_chiplet, &maxPESizeNM, &maxTileSizeCM, &numPENM, param->mode);	
			pipelineSpeedUp = ChipDesignInitialize(inputParameter, tech, cell, true, netStructure, &maxPESizeNM, &maxTileSizeCM, &numPENM, param->mode);


			numTileEachLayer = ChipFloorPlan(true, false, false, netStructure_chiplet, markNM,
							maxPESizeNM, maxTileSizeCM, numPENM, pipelineSpeedUp,
							&desiredNumTileNM, &desiredPESizeNM, &desiredNumTileCM, &desiredTileSizeCM, &desiredPESizeCM, &numTileRow, &numTileCol);

			utilizationEachLayer = ChipFloorPlan(false, true, false, netStructure_chiplet, markNM,
							maxPESizeNM, maxTileSizeCM, numPENM, pipelineSpeedUp,
							&desiredNumTileNM, &desiredPESizeNM, &desiredNumTileCM, &desiredTileSizeCM, &desiredPESizeCM, &numTileRow, &numTileCol);

			speedUpEachLayer = ChipFloorPlan(false, false, true, netStructure_chiplet, markNM,
							maxPESizeNM, maxTileSizeCM, numPENM, pipelineSpeedUp,
							&desiredNumTileNM, &desiredPESizeNM, &desiredNumTileCM, &desiredTileSizeCM, &desiredPESizeCM, &numTileRow, &numTileCol);

			tileLocaEachLayer = ChipFloorPlan(false, false, false, netStructure_chiplet, markNM,
							maxPESizeNM, maxTileSizeCM, numPENM, pipelineSpeedUp,
							&desiredNumTileNM, &desiredPESizeNM, &desiredNumTileCM, &desiredTileSizeCM, &desiredPESizeCM, &numTileRow, &numTileCol);

			cout << "------------------------------ FloorPlan --------------------------------" <<  endl;
			cout << endl;
			//cout << "Tile and PE size are optimized to maximize memory utilization ( = memory mapped by synapse / total memory on chip)" << endl;
			cout << endl;
			if (!param->novelMapping) {
				cout << "Desired Conventional Mapped Tile Storage Size: " << desiredTileSizeCM << "x" << desiredTileSizeCM << endl;
				cout << "Desired Conventional PE Storage Size: " << desiredPESizeCM << "x" << desiredPESizeCM << endl;
			} else {
				cout << "Desired Conventional Mapped Tile Storage Size: " << desiredTileSizeCM << "x" << desiredTileSizeCM << endl;
				cout << "Desired Conventional PE Storage Size: " << desiredPESizeCM << "x" << desiredPESizeCM << endl;
				cout << "Desired Novel Mapped Tile Storage Size: " << numPENM << "x" << desiredPESizeNM << "x" << desiredPESizeNM << endl;
			}
			cout << "User-defined SubArray Size: " << param->numRowSubArray << "x" << param->numColSubArray << endl;
			cout << endl;
			cout << "----------------- # of tile used for each layer -----------------" <<  endl;

			std::ofstream myfile;
			if (i==0)
			{
				myfile.open ("./to_interconnect/num_tiles_per_layer_chiplet.csv");
			}

			else
			{
				myfile.open ("./to_interconnect/num_tiles_per_layer_chiplet.csv", std::ios_base::app);
			}
			// myfile.open ("./to_interconnect/num_tiles_per_layer_chiplet_"+ std::to_string(i+1) + ".csv"); //Dumps file for the number of tile per layer for the interconnect simulator.
			for (int j=0; j<netStructure_chiplet.size(); j++) 
			{
				cout <<"Chiplet_"+ std::to_string(i+1)<<","<< "layer " << j+start+1 << ", tiles required is :" << numTileEachLayer[0][j] * numTileEachLayer[1][j] << endl;
				//myfile << i+1 <<","<< numTileEachLayer[0][i] * numTileEachLayer[1][i] << endl;
				myfile <<"Chiplet_"+ std::to_string(i+1)<<","<<"layer "<< j+start+1<<","<<numTileEachLayer[0][j] * numTileEachLayer[1][j] << ","<< split[i]<<endl;
				// if (param->chiplet_structure)
				if ((param->chiplet_structure))
				{
					totalNumTile = chip_size;
					cout<<"********************All the chiplets are of same size. Homogeneous structure utilized********************"<<endl;
					if (((numTileEachLayer[0][j] * numTileEachLayer[1][j]) <= chip_size))
					{
						cout<<"********************Layer has been Partitioned Across Multiple Chiplets that are homogeneous********************"<<endl;
					}
				}

				else
				{
					totalNumTile += numTileEachLayer[0][j] * numTileEachLayer[1][j];					
				}
				// used_totalNumTile += numTileEachLayer[0][j] * numTileEachLayer[1][j] * utilizationEachLayer[j][0];
				used_totalNumTile += numTileEachLayer[0][j] * numTileEachLayer[1][j];
			}

			myfile.close();
			cout << endl;
			/*
			cout << "----------------- Speed-up of each layer ------------------" <<  endl;
			for (int i=0; i<netStructure_chiplet.size(); i++) {
				cout << "layer" << i+1 << ": " << speedUpEachLayer[0][i] << ", " << speedUpEachLayer[1][i] << endl;
			}*/
			cout << endl;
			cout << "----------------- Utilization of each layer ------------------" <<  endl;
			
			for (int j=0; j<netStructure_chiplet.size(); j++) 
			{
				cout << "layer " << j+start+1 << ": " << utilizationEachLayer[j][0] << endl;
				// cout << "numTileEachLayer[0][j]" <<": "<<numTileEachLayer[0][j]<< "\t numTileEachLayer[1][j]" <<": "<<numTileEachLayer[1][j]<<endl;
				// cout << "numTileEachLayer[0][j] * numTileEachLayer[1][j] : "<< numTileEachLayer[0][j] * numTileEachLayer[1][j] <<endl;
				realMappedMemory += numTileEachLayer[0][j] * numTileEachLayer[1][j] * utilizationEachLayer[j][0];
				// if (realMappedMemory > totalNumTile)
				// {
				// 	realMappedMemory = realMappedMemory/split[j];
				// }
			}
			cout<<"The total number of tiles required are : "<<used_totalNumTile<<endl;
			cout<<"The number of chiplets used to map this layer is : "<<split[i]<<endl;
			cout<<"The total number of tiles/chiplet are : "<<(totalNumTile)<<endl;
			// cout<<"The real Mapped memory is : "<<numTileEachLayer[0][i] * numTileEachLayer[1][i] * utilizationEachLayer[i][0]<<endl;
			cout << "Memory Utilization of Whole Chip: " << (realMappedMemory/(totalNumTile*split[i]))*100 << " % " << endl;
			cout << endl;
			// if (param->fixed_cnt_chiplet)
			// {
			// 	avg_utilization_chiplet+= (realMappedMemory/(totalNumTile))*100;
			// }
			// else
			// {
			// 	avg_utilization_chiplet+= (realMappedMemory/(totalNumTile*split[i]))*100;
			// }
			avg_utilization_chiplet+= (realMappedMemory/(totalNumTile*split[i]))*100;
			
			cout << "---------------------------- FloorPlan Done ------------------------------" <<  endl;
			cout << endl;
			cout << endl;
			cout << endl;	

			for (int j=0; j<netStructure_chiplet.size(); j++) {
				numComputation += 2*(netStructure_chiplet[j][0] * netStructure_chiplet[j][1] * netStructure_chiplet[j][2] * netStructure_chiplet[j][3] * netStructure_chiplet[j][4] * netStructure_chiplet[j][5] * netStructure_chiplet[j][7]);
			}
			// cout<<"The total number of computations are : "<<numComputation<<endl;
			//nocs
			if (param->trainingEstimation) 
			{
				numComputation *= 3;  // forward, computation of activation gradient, weight gradient
				numComputation *= param->batchSize * param->numIteration;  // count for one epoch
			}
			cout<<"The total number of computations including training are : "<<numComputation<<endl;
			
			ChipInitialize(inputParameter, tech, cell, netStructure_chiplet, markNM, numTileEachLayer,
							numPENM, desiredNumTileNM, desiredPESizeNM, desiredNumTileCM, desiredTileSizeCM, desiredPESizeCM, numTileRow, numTileCol, &numArrayWriteParallel);

			if (param->chiplet_structure)
			// if ((param->chiplet_structure) & & ((numTileEachLayer[0][j] * numTileEachLayer[1][j]) <= chip_size))
			{
				desiredNumTileCM = chip_size;
				numTileRow = ceil((double)sqrt((double)(desiredNumTileCM)));
				numTileCol = ceil((double)((desiredNumTileCM))/(double)(numTileRow));
				cout<<"********************All the chiplets are of same size. Homogeneous structure utilized********************"<<endl;
			}

			else
			{
				desiredNumTileCM = desiredNumTileCM;
				cout<<"********************All the chiplets are of Different size. Heterogeneous structure utilized********************"<<endl;
			}

			// Work Around for Pointer Issue
			if ((param->chiplet_structure) & (i!=0))
			{
				chipAreaResults = ChipCalculateArea(inputParameter, tech, cell, desiredNumTileNM, numPENM, desiredPESizeNM, desiredNumTileCM, desiredTileSizeCM, desiredPESizeCM, numTileRow,
							&chipHeight, &chipWidth, &CMTileheight, &CMTilewidth, &NMTileheight, &NMTilewidth, split[i]);
				// chipAreaResults = chipAreaResults_common;
			}

			else
			{
				chipAreaResults = ChipCalculateArea(inputParameter, tech, cell, desiredNumTileNM, numPENM, desiredPESizeNM, desiredNumTileCM, desiredTileSizeCM, desiredPESizeCM, numTileRow,
							&chipHeight, &chipWidth, &CMTileheight, &CMTilewidth, &NMTileheight, &NMTilewidth, split[i]);
				chipAreaResults_common = chipAreaResults;
			}
			// Need to Figure it out why it is different in some cases

			chipArea = chipAreaResults[0];
			chipAreaIC = chipAreaResults[1];
			chipAreaADC = chipAreaResults[2];
			chipAreaAccum = chipAreaResults[3];
			chipAreaOther = chipAreaResults[4];
			chipAreaWG = chipAreaResults[5];
			chipArealocalbuffer = chipAreaResults[6];
			chipAreaglobalbuffer = chipAreaResults[7];
			chipAreaOtheralone = chipAreaResults[8];
			chipAreaglobalIC = chipAreaResults[9];
			chipAreatileIC = chipAreaResults[10];
			chipAreasubarrayalone=chipAreaResults[11];
			chipAreaArray = chipAreaResults[12]; 

			if (param->fixed_cnt_chiplet)
			{				
				if ((i==num_chip) & ((num_chip_final+1) < param->cnt_chiplet))
				{
					cout<<"This is the residual area addition for the remaining unused tiles."<<endl;
					residue_area_cal = param->cnt_chiplet - (num_chip_final+1);
					chipArea += chipAreaResults[0] * residue_area_cal;
					chipAreaIC += chipAreaResults[1]* residue_area_cal;
					chipAreaADC += chipAreaResults[2]* residue_area_cal;
					chipAreaAccum += chipAreaResults[3]* residue_area_cal;
					chipAreaOther += chipAreaResults[4]* residue_area_cal;
					chipAreaWG += chipAreaResults[5]* residue_area_cal;
					chipArealocalbuffer += chipAreaResults[6]* residue_area_cal;
					chipAreaglobalbuffer += chipAreaResults[7]* residue_area_cal;
					chipAreaOtheralone = chipAreaResults[8]* residue_area_cal;
					chipAreaglobalIC += chipAreaResults[9]* residue_area_cal;
					chipAreatileIC += chipAreaResults[10]* residue_area_cal;
					chipAreasubarrayalone +=chipAreaResults[11]* residue_area_cal;
				}
			}

			std::ofstream myarea;
			// myarea.open ("./Final_Results/area_chiplet_"+ std::to_string(i+1) + ".csv");
			if (i==0)
			{
				myarea.open ("./Final_Results/area_chiplet.csv");
			}

			else
			{
				myarea.open ("./Final_Results/area_chiplet.csv", std::ios_base::app);
			}		
			
			myarea<<"Chiplet_"+ std::to_string(i+1)<<","<<"Chip Area "<<","<<chipAreaResults[0] * 1e12<<","<<" um^2"<<endl;
			myarea<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total Within Tile Routing Area "<<","<<chipAreaResults[1] * 1e12<<","<<" um^2"<<endl;
			myarea<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total ADC (or S/As and precharger for SRAM) Area "<<","<<chipAreaResults[2] * 1e12<<","<<" um^2"<<endl;
			myarea<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total Accumulation Area"<<","<<chipAreaResults[3] * 1e12<<","<<" um^2"<<endl;
			myarea<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total Other Peripheries Area"<<","<<chipAreaResults[4] * 1e12<<","<<" um^2"<<endl;
			myarea<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total Buffer Area"<<","<<(chipAreaResults[6] + chipAreaResults[7]) * 1e12<<","<<" um^2"<<endl;
			myarea<<"Chiplet_"+ std::to_string(i+1)<<","<<"Subarray Area"<<","<<chipAreaResults[11] * 1e12<<","<<"um^2"<<endl;
			myarea<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total WG Calculation Area "<<","<<chipAreaResults[5] * 1e12<<","<<" um^2"<<endl;

			cout << endl;
			myarea.close();

			cout << "-------------------------------------- Hardware Performance --------------------------------------" <<  endl;
			
			int multiplier = 0;
			multiplier = split[i];
			// cout << " multiplier for i = " << i << " is : " << multiplier << endl;
					
			ofstream breakdownfile;
			string breakdownfile_name = "./Element_Results_Each_Epoch/Element_Breakdown_Epoch_";
			breakdownfile_name.append(argv[1]);
			breakdownfile_name.append(".csv");
			breakdownfile.open (breakdownfile_name, ios::app);
			if (breakdownfile.is_open()) {
				// firstly save the area results to file
				breakdownfile << "Total Area(m^2), Total CIM (FW+AG) Area (m^2), Routing Area(m^2), ADC Area(m^2), Accumulation Area(m^2), Other Logic&Storage Area(m^2), Weight Gradient Area(m^2),"<< endl;
				breakdownfile << chipAreaResults[0] << "," << chipAreaArray << "," << chipAreaResults[1] << "," << chipAreaResults[2] << "," << chipAreaResults[3] << "," << chipAreaResults[4] << "," << chipAreaResults[5] << endl;
				breakdownfile << "layer_number, latency_FW(s), latency_AG(s), latency_WG(s), latency_WU(s), energy_FW(J), energy_AG(J), energy_WG(J), energy_WU(J),";
				breakdownfile << "Peak_latency_FW(s), Peak_latency_AG(s), Peak_latency_WG(s), Peak_latency_WU(s), Peak_energy_FW(J), Peak_energy_AG(J), Peak_energy_WG(J), Peak_energy_WU(J),";
				breakdownfile << ", , ADC_latency(s), Accumulation_latency(s), Synaptic Array w/o ADC_latency(s), Buffer_latency(s), IC_latency(s), Weight_gradient_latency(s), Weight_update(s), DRAM_latency(s), ";
				breakdownfile << "ADC_energy(J), Accumulation_energy(J), Synaptic Array w/o ADC_energy(J), Buffer_energy(J), IC_energy(J), Weight_gradient_energy(J), Weight_update_energy(J)" << endl;
			} else {
				cout << "1. Error: the breakdown file cannot be opened!" << endl;
			}
			
			// Estimation of latency, area, energy etc. in detail for each layer in a chiplet begins here

			for (int i=0; i<netStructure_chiplet.size(); i++) 
			{
				cout<<"The weight matrix(s) we are loading is(are): "<<argv[4*(i+start)+5] << " ,  "<< argv[4*(i+start)+6] <<endl;
				cout << endl;
				cout << "-------------------- Estimation of Layer " << i+start+1 << " ----------------------" << endl;
				
				param->activityRowReadWG = atof(argv[4*i+8]);
	            param->activityRowWriteWG = atof(argv[4*i+8]);
	            param->activityColWriteWG = atof(argv[4*i+8]);
				
				ChipCalculatePerformance(inputParameter, tech, cell, i, argv[4*(i+start)+5], argv[4*(i+start)+6], argv[4*(i+start)+7], netStructure_chiplet[i][6],
						netStructure_chiplet, markNM, numTileEachLayer, utilizationEachLayer, speedUpEachLayer, tileLocaEachLayer,
						numPENM, desiredPESizeNM, desiredTileSizeCM, desiredPESizeCM, CMTileheight, CMTilewidth, NMTileheight, NMTilewidth, numArrayWriteParallel,
						&layerReadLatency, &layerReadDynamicEnergy, &tileLeakage, &layerReadLatencyAG, &layerReadDynamicEnergyAG, &layerReadLatencyWG, &layerReadDynamicEnergyWG, 
						&layerWriteLatencyWU, &layerWriteDynamicEnergyWU, &layerbufferLatency, &layerbufferDynamicEnergy, &layericLatency, &layericDynamicEnergy,
						&coreLatencyADC, &coreLatencyAccum, &coreLatencyOther, &coreEnergyADC, &coreEnergyAccum, &coreEnergyOther, &layerDRAMLatency, &layerDRAMDynamicEnergy,
						&layerReadLatencyPeakFW, &layerReadDynamicEnergyPeakFW, &layerReadLatencyPeakAG, &layerReadDynamicEnergyPeakAG,
						&layerReadLatencyPeakWG, &layerReadDynamicEnergyPeakWG, &layerWriteLatencyPeakWU, &layerWriteDynamicEnergyPeakWU, &global_routinglatency, &test, &local_bufferlatency, &local_bufferenergy
						, &local_routinglatency, &local_routingenergy, &global_routingenergy, &global_bufferlatency, &global_bufferenergy, 1);
	
				double numTileOtherLayer = 0;
				double layerLeakageEnergy = 0;	

				for (int j=0; j<netStructure_chiplet.size(); j++) {
					if (j != i) {
						numTileOtherLayer += numTileEachLayer[0][j] * numTileEachLayer[1][j];
					}
				}
				layerLeakageEnergy = numTileOtherLayer*layerReadLatency*tileLeakage*netStructure_chiplet[i][7];

				chipReadLatency += layerReadLatency*netStructure_chiplet[i][7]/multiplier;
				avg_layerReadLatency += layerReadLatency*netStructure_chiplet[i][7];
				chipReadDynamicEnergy += layerReadDynamicEnergy*netStructure_chiplet[i][7];
				chipLeakageEnergy += layerLeakageEnergy*netStructure_chiplet[i][7];
				chipLeakage += tileLeakage*numTileEachLayer[0][i] * numTileEachLayer[1][i]*netStructure_chiplet[i][7];
				chipbufferLatency += layerbufferLatency*netStructure_chiplet[i][7];
				chipbufferReadDynamicEnergy += layerbufferDynamicEnergy*netStructure_chiplet[i][7];
				chipicLatency += layericLatency*netStructure_chiplet[i][7];
				chipicReadDynamicEnergy += layericDynamicEnergy*netStructure_chiplet[i][7];
				
				chipReadLatencyAG += layerReadLatencyAG*netStructure_chiplet[i][7];
				chipReadDynamicEnergyAG += layerReadDynamicEnergyAG*netStructure_chiplet[i][7];
				chipReadLatencyWG += layerReadLatencyWG*netStructure_chiplet[i][7];
				chipReadDynamicEnergyWG += layerReadDynamicEnergyWG*netStructure_chiplet[i][7];
				chipWriteLatencyWU += layerWriteLatencyWU*netStructure_chiplet[i][7];
				// cout <<" nk main.cpp layerWriteLatencyWU = "<<layerWriteLatencyWU<< " and     chipWriteLatencyWU = " <<chipWriteLatencyWU<<endl;
				chipWriteDynamicEnergyWU += layerWriteDynamicEnergyWU*netStructure_chiplet[i][7];
				// cout <<" nk main.cpp layerWriteDynamicEnergyWU = "<<layerWriteDynamicEnergyWU<<  " and     chipWriteLatencyWU = " <<chipWriteLatencyWU<<endl;


				chipReadLatencyPeakFW += layerReadLatencyPeakFW;
				// cout <<" nk main.cpp layerReadLatencyPeakFW = "<<layerReadLatencyPeakFW<<  " and     chipReadLatencyPeakFW = " <<chipReadLatencyPeakFW<<endl;
				chipReadDynamicEnergyPeakFW += layerReadDynamicEnergyPeakFW;
				// cout <<" nk main.cpp layerReadDynamicEnergyPeakFW = "<<layerReadDynamicEnergyPeakFW<<  " and     chipReadDynamicEnergyPeakFW = " <<chipReadDynamicEnergyPeakFW<<endl;
				chipReadLatencyPeakAG += layerReadLatencyPeakAG;
				chipReadDynamicEnergyPeakAG += layerReadDynamicEnergyPeakAG;
				chipReadLatencyPeakWG += layerReadLatencyPeakWG;
				chipReadDynamicEnergyPeakWG += layerReadDynamicEnergyPeakWG;
				chipWriteLatencyPeakWU += layerWriteLatencyPeakWU;
				chipWriteDynamicEnergyPeakWU += layerWriteDynamicEnergyPeakWU;			

				global_routinglatency_1+=global_routinglatency*netStructure_chiplet[i][7];
				local_routinglatency_1+=local_routinglatency*netStructure_chiplet[i][7];
				global_bufferlatency_1+=global_bufferlatency*netStructure_chiplet[i][7];
				local_bufferlatency_1+=local_bufferlatency*netStructure_chiplet[i][7];
				coreLatencyOther_only_1+=coreLatencyOther_only*netStructure_chiplet[i][7];

				global_bufferenergy_1 += global_bufferenergy*netStructure_chiplet[i][7];
				coreEnergyOther_only_1 +=coreEnergyOther_only*netStructure_chiplet[i][7];
				local_bufferenergy_1 +=local_bufferenergy*netStructure_chiplet[i][7];
				global_routingenergy_1 +=global_routingenergy*netStructure_chiplet[i][7];
				local_routingenergy_1 +=local_routingenergy*netStructure_chiplet[i][7];
				chipLatencyADC += coreLatencyADC*netStructure_chiplet[i][7]/multiplier;
				chipLatencyAccum += coreLatencyAccum*netStructure_chiplet[i][7]/multiplier;
				//max_glob_acc_lat = MAX(max_glob_acc_lat, Global_accum_est_lat);

				//cout<<"\n The new global accum latency to support the chip max addition is: "<< Global_accum_est_lat*1e9<< "ns" <<endl;
				//cout<<"\n The new maximum global accum latency to support the chip max addition is: "<< max_glob_acc_lat*1e9<< "ns" <<endl;

				//chipLatencyAccum += max_glob_acc_lat;
				chipLatencyOther += coreLatencyOther*netStructure_chiplet[i][7]/multiplier;
				chipEnergyADC += coreEnergyADC*netStructure_chiplet[i][7];
				chipEnergyAccum += coreEnergyAccum*netStructure_chiplet[i][7];
				//max_glob_acc_energy = MAX(max_glob_acc_energy, Global_accum_est_energy);
				//cout<<"\n The new global accum energy to support the chip max addition is: "<< Global_accum_est_energy*1e12<< "pJ" <<endl;
				//cout<<"\n The new maximum global accum energy to support the chip max addition is: "<< max_glob_acc_energy*1e12<< "pJ" <<endl;
				//chipEnergyAccum += max_glob_acc_energy;

				chipEnergyOther += coreEnergyOther*netStructure_chiplet[i][7];
				global_iclatency += global_bufferlatency*netStructure_chiplet[i][7];

							// =========================================================================================
				cout << "readLatency : " << layerReadLatency*1e9 <<   "ns"<< endl;
				cout << "buffer_latency : " << layerbufferLatency*1e9 <<  "ns"<<endl;
				cout << "Routing_latency : " << layericLatency*1e9 <<  "ns"<<endl;
				cout << "AG_readLatency : " << layerReadLatencyAG*1e9 <<   "ns"<<endl;
				cout << "WG_readLatency : " << layerReadLatencyWG*1e9 <<  "ns"<<endl;
				cout << "WU_writeLatency : " << abs(layerWriteLatencyWU)*1e9 <<  "ns"<<endl;
				cout << "DRAM_data_tranfer_Latency : " << layerDRAMLatency*1e9 <<  "ns"<<endl;
				if (coreLatencyADC > 0)
				{
					cout << "ADC_readLatency : " << coreLatencyADC*1e9 <<  "ns"<< endl;
					cout << "Accum_readLatency : " << coreLatencyAccum*1e9 <<  "ns"<<endl;
					cout << "Other_readLatency : " << coreLatencyOther*1e9 <<   "ns"<<endl;
				}
				else
				{
					cout << "ADC_readLatency : " << chipLatencyADC *1e9 <<  "ns"<< endl;
					cout << "Accum_readLatency : " <<chipLatencyAccum *1e9 <<  "ns"<<endl;
					cout << "Other_readLatency : " << chipLatencyOther*1e9 <<  "ns"<<endl;
				}
				cout << "readDynamicEnergy : " << layerReadDynamicEnergy*1e12 <<   "pJ"<<endl;
				cout << "buffer_readDynamicEnergy : " << layerbufferDynamicEnergy*1e12 << "pJ"<< endl;
				cout << "Routing_readDynamicEnergy : " << layericDynamicEnergy*1e12 <<  "pJ"<<endl;
				cout << "readDynamicEnergy_AG : " << layerReadDynamicEnergyAG*1e12 <<  "pJ"<<endl;
				cout << "readDynamicEnergy_WG : " << layerReadDynamicEnergyWG*1e12 << "pJ"<< endl;
				cout << "writeDynamicEnergy_WU : " << abs(layerWriteDynamicEnergyWU)*1e12  << "pJ"<<endl;
				cout << "DRAM_data_tranfer_Energy : " << layerDRAMDynamicEnergy*1e12 << "pJ"<<endl;
				if (coreEnergyADC > 0)
				{
					cout << "ADC_readEnergy : " << coreEnergyADC*1e12 << "pJ"<<endl;
					cout << "Accum_readEnergy : " << coreEnergyAccum*1e12 <<  "pJ"<<endl;
					cout << "Other_readEnergy : " << coreEnergyOther*1e12 <<  "pJ"<<endl;
				}
				else
				{
					cout << "ADC_readEnergy : " << chipEnergyADC*1e12 <<  "pJ"<<endl;
					cout << "Accum_readEnergy : " << chipEnergyAccum*1e12 <<  "pJ"<<endl;
					cout << "Other_readEnergy : " << chipEnergyOther*1e12 << "pJ"<<endl;
				}
			//  ========================================================================================

				// cout << "layer" << i+start+1 << "'s readLatency is: " << layerReadLatency*1e9 << " ns" << endl;
				// cout << "layer" << i+start+1 << "'s buffer latency is: " << layerbufferLatency*1e9 << " ns" << endl;
				// cout << "layer" << i+start+1 << "'s Routing latency is: " << layericLatency*1e9 << " ns" << endl;
				// cout << "layer" << i+start+1 << "'s readLatency of Activation Gradient is: " << layerReadLatencyAG*1e9 << " ns" << endl;
				// cout << "layer" << i+start+1 << "'s readLatency of Weight Gradient is: " << layerReadLatencyWG*1e9 << " ns" << endl;
				// cout << "layer" << i+start+1 << "'s writeLatency of Weight Update is: " << layerWriteLatencyWU*1e9 << " ns" << endl;
				// cout << "DRAM data transfer Latency is : " << layerDRAMLatency*1e9 << " ns" << endl;
				// cout << "ADC (or S/As and precharger for SRAM) readLatency is : " << coreLatencyADC*1e9 << " ns , " <<chipLatencyADC *1e9 << " ns"<< endl;
				// cout << "Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readLatency is : " << coreLatencyAccum*1e9 << " ns , " <<chipLatencyAccum *1e9 << " ns" << endl;
				// cout << "Other Peripheries (e.g. decoders, mux, switchmatrix, buffers, IC, pooling and activation units) readLatency is : " << coreLatencyOther*1e9 << " ns , " <<chipLatencyOther*1e9 << " ns"<< endl;

				// // cout << "layer" << i+start+1 << "'s leakagePower is: " << numTileEachLayer[0][i] * numTileEachLayer[1][i] * tileLeakage*1e6 << " uW" << endl;
				// // cout << "layer" << i+start+1 << "'s leakageEnergy is: " << layerLeakageEnergy*1e12 << " pJ" << endl;
				// cout << "layer" << i+start+1 << "'s readDynamicEnergy is: " << layerReadDynamicEnergy*1e12 << " pJ" << endl;
				// cout << "layer" << i+start+1 << "'s buffer readDynamicEnergy is: " << layerbufferDynamicEnergy*1e12 << " pJ" << endl;
				// cout << "layer" << i+start+1 << "'s Routing readDynamicEnergy is: " << layericDynamicEnergy*1e12 << " pJ" << endl;
				// // cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				// // cout << "layer" << i+start+1 << "'s readLatency of Forward is: " << layerReadLatency*1e9 << "ns" << endl;
				// // cout << "layer" << i+start+1 << "'s readDynamicEnergy of Forward is: " << layerReadDynamicEnergy*1e12 << "pJ" << endl;
				// cout << "layer" << i+start+1 << "'s readDynamicEnergy of Activation Gradient is: " << layerReadDynamicEnergyAG*1e12 << " pJ" << endl;
				// cout << "layer" << i+start+1 << "'s readDynamicEnergy of Weight Gradient is: " << layerReadDynamicEnergyWG*1e12 << " pJ" << endl;
				// cout << "layer" << i+start+1 << "'s writeDynamicEnergy of Weight Update is: " << layerWriteDynamicEnergyWU*1e12 << " pJ" << endl;
				// cout << "DRAM data transfer Energy is : " << layerDRAMDynamicEnergy*1e12 << "pJ" << endl;
				// cout << "ADC (or S/As and precharger for SRAM) readEnergy is : " << coreEnergyADC*1e12 << " pJ , "<< chipEnergyADC*1e12 << " pJ" << endl;
				// cout << "Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readEnergy is : " << coreEnergyAccum*1e12 << " pJ , "<< chipEnergyAccum*1e12 << " pJ" << endl;
				// cout << "Other Peripheries (e.g. decoders, mux, switchmatrix, buffers, IC, pooling and activation units) readEnergy is : " << coreEnergyOther*1e12 << " pJ , "<< chipEnergyOther*1e12 << " pJ" << endl;

				// // cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				// // cout << "layer" << i+start+1 << "'s PEAK readLatency of Forward is: " << layerReadLatencyPeakFW*1e9 << "ns" << endl;
				// // cout << "layer" << i+start+1 << "'s PEAK readDynamicEnergy of Forward is: " << layerReadDynamicEnergyPeakFW*1e12 << "pJ" << endl;
				// // cout << "layer" << i+start+1 << "'s PEAK readLatency of Activation Gradient is: " << layerReadLatencyPeakAG*1e9 << "ns" << endl;
				// // cout << "layer" << i+start+1 << "'s PEAK readDynamicEnergy of Activation Gradient is: " << layerReadDynamicEnergyPeakAG*1e12 << "pJ" << endl;
				// // cout << "layer" << i+start+1 << "'s PEAK readLatency of Weight Gradient is: " << layerReadLatencyPeakWG*1e9 << "ns" << endl;
				// // cout << "layer" << i+start+1 << "'s PEAK readDynamicEnergy of Weight Gradient is: " << layerReadDynamicEnergyPeakWG*1e12 << "pJ" << endl;
				// // cout << "layer" << i+start+1 << "'s PEAK writeLatency of Weight Update is: " << layerWriteLatencyPeakWU*1e9 << "ns" << endl;
				// // cout << "layer" << i+start+1 << "'s PEAK writeDynamicEnergy of Weight Update is: " << layerWriteDynamicEnergyPeakWU*1e12 << "pJ" << endl;
				// // cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				// // cout << "layer" << i+start+1 << "'s leakagePower is: " << numTileEachLayer[0][i] * numTileEachLayer[1][i] * tileLeakage*1e6 << "uW" << endl;
				// // cout << "layer" << i+start+1 << "'s leakageEnergy is: " << layerLeakageEnergy*1e12 << "pJ" << endl;

				count+=1;
				tile_total+=test;
				cout << endl;

				// cout << "************************ Breakdown of Latency and Dynamic Energy *************************" << endl;
				// cout << endl;
				// cout << "----------- ADC (or S/As and precharger for SRAM) readLatency is : " << coreLatencyADC*1e9 << " ns" << endl;
				// cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readLatency is : " << coreLatencyAccum*1e9 << " ns" << endl;
				// cout << "----------- Other Peripheries (e.g. decoders, mux, switchmatrix, buffers, IC, pooling and activation units) readLatency is : " << coreLatencyOther*1e9 << " ns" << endl;
				// // cout << "----------- Other Peripheries Only (e.g. decoders, mux, switchmatrix, pooling and activation units) readLatency is : " << coreLatencyOther_only*1e9 << " ns" << endl;
				// cout << "----------- ADC (or S/As and precharger for SRAM) readEnergy is : " << coreEnergyADC*1e12 << " pJ" << endl;
				// cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readEnergy is : " << coreEnergyAccum*1e12 << " pJ" << endl;
				// cout << "----------- Other Peripheries (e.g. decoders, mux, switchmatrix, buffers, IC, pooling and activation units) readEnergy is : " << coreEnergyOther*1e12 << " pJ" << endl;
				// // cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				// // cout << "----------- Synaptic Array w/o ADC (Forward + Activate Gradient) readLatency is : " << coreLatencyOther*1e9 << "ns" << endl;
				// // cout << "----------- Buffer buffer latency is: " << layerbufferLatency*1e9 << "ns" << endl;
				// cout << "----------- Interconnect latency is: " << layericLatency*1e9 << "ns" << endl;
				// cout << "----------- Weight Gradient Calculation readLatency is : " << layerReadLatencyPeakWG*1e9 << "ns" << endl;
				// cout << "----------- Weight Update writeLatency is : " << layerWriteLatencyPeakWU*1e9 << "ns" << endl;
				// cout << "----------- DRAM data transfer Latency is : " << layerDRAMLatency*1e9 << "ns" << endl;
				// cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				// cout << "----------- ADC (or S/As and precharger for SRAM) readDynamicEnergy is : " << coreEnergyADC*1e12 << "pJ" << endl;
				// cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readDynamicEnergy is : " << coreEnergyAccum*1e12 << "pJ" << endl;
				// cout << "----------- Synaptic Array w/o ADC (Forward + Activate Gradient) readDynamicEnergy is : " << coreEnergyOther*1e12 << "pJ" << endl;
				// cout << "----------- Buffer readDynamicEnergy is: " << layerbufferDynamicEnergy*1e12 << "pJ" << endl;
				// cout << "----------- Interconnect readDynamicEnergy is: " << layericDynamicEnergy*1e12 << "pJ" << endl;
				// cout << "----------- Weight Gradient Calculation readDynamicEnergy is : " << layerReadDynamicEnergyPeakWG*1e12 << "pJ" << endl;
				// cout << "----------- Weight Update writeDynamicEnergy is : " << layerWriteDynamicEnergyPeakWU*1e12 << "pJ" << endl;
				// cout << "----------- DRAM data transfer Energy is : " << layerDRAMDynamicEnergy*1e12 << "pJ" << endl;
				// cout << endl;

				// cout << "************************ Breakdown of Latency and Dynamic Energy *************************" << endl;
				cout << endl;
			}

			std::ofstream mylat;
			if (i==0)
			{	
				mylat.open ("./Final_Results/Latency_chiplet.csv");
			}

			else
			{
				mylat.open ("./Final_Results/Latency_chiplet.csv", std::ios_base::app) ;
			}
				
			mylat<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total readLatency"<<","<<chipReadLatency * 1e9<<","<<" ns"<<endl;
			mylat<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total Buffer Latency"<<","<<chipbufferLatency * 1e9<<","<<" ns"<<endl;
			mylat<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total Routing Latency"<<","<<chipicLatency * 1e9<<","<<" ns"<<endl;
			mylat<<"Chiplet_"+ std::to_string(i+1)<<","<<"AG Latency"<<","<<chipReadLatencyAG * 1e9<<","<<" ns"<<endl;
			mylat<<"Chiplet_"+ std::to_string(i+1)<<","<<"WG Latency"<<","<<chipReadLatencyWG * 1e9<<","<<" ns"<<endl;
			mylat<<"Chiplet_"+ std::to_string(i+1)<<","<<"WU write Latency"<<","<<chipWriteLatencyWU * 1e9<<","<<" ns"<<endl;
			mylat<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total ADC Latency: "<<","<<chipLatencyADC * 1e9<<","<<" ns"<<endl;
			mylat<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total Accumulation Latency: "<<","<<chipLatencyAccum * 1e9<<","<<" ns"<<endl;
			mylat<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total Other Peripheries Latency: "<<","<<chipLatencyOther * 1e9<<","<<" ns"<<endl;
			mylat<<"Chiplet_"+ std::to_string(i+1)<<","<<"training latency not added"<<endl;
			mylat.close();

			std::ofstream myenergy;
			if (i==0)
			{					myenergy.open ("./Final_Results/Energy_chiplet.csv");
			}
			else
			{					myenergy.open ("./Final_Results/Energy_chiplet.csv", std::ios_base::app);
			}		
				
			myenergy<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total readEnergy: "<<","<<chipReadDynamicEnergy * 1e12<<","<<" pJ"<<endl;
			myenergy<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total leakage Energy: "<<","<<chipLeakageEnergy * 1e12<<","<<" pJ"<<endl;
			myenergy<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total Routing Energy: "<<","<<chipicReadDynamicEnergy * 1e12<<","<<" pJ"<<endl;
			myenergy<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total Buffer Energy: "<<","<<chipbufferReadDynamicEnergy * 1e12<<","<<" pJ"<<endl;
			myenergy<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total Local Buffer Energy"<<","<<local_bufferenergy_1 * 1e12<<","<<" pJ"<<endl;
			myenergy<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total Local Routing Energy"<<","<<local_routingenergy_1 * 1e12<<","<<" pJ"<<endl;
			myenergy<<"Chiplet_"+ std::to_string(i+1)<<","<<"AG Energy"<<","<<chipReadDynamicEnergyAG * 1e12 << " pJ" << endl;
			myenergy<<"Chiplet_"+ std::to_string(i+1)<<","<<"WG Energy"<<","<<chipReadDynamicEnergyWG * 1e12 << " pJ" << endl;
			myenergy<<"Chiplet_"+ std::to_string(i+1)<<","<<"WU write Energy"<<","<<chipWriteDynamicEnergyWU * 1e12 << " pJ" << endl;
			myenergy<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total ADC Energy"<<","<<chipEnergyADC * 1e12<<","<<" pJ"<<endl;
			myenergy<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total Accumulation Energy"<<","<<chipEnergyAccum * 1e12<<","<<"pJ"<<endl;
			myenergy<<"Chiplet_"+ std::to_string(i+1)<<","<<"Total Other Peripheries Energy"<<","<<chipEnergyOther * 1e12<<","<<" pJ"<<endl;
			myenergy<<"Chiplet_"+ std::to_string(i+1)<<","<<"training energy not added"<<endl;
			myenergy.close();

			if (breakdownfile.is_open()) 
			{
				breakdownfile << i+1 << "," << layerReadLatency << "," << layerReadLatencyAG << "," << layerReadLatencyWG << "," << layerWriteLatencyWU << ",";
				breakdownfile << layerReadDynamicEnergy << "," << layerReadDynamicEnergyAG << "," << layerReadDynamicEnergyWG << "," << layerWriteDynamicEnergyWU << ",";
				breakdownfile << layerReadLatencyPeakFW << "," << layerReadLatencyPeakAG << "," << layerReadLatencyPeakWG << "," << layerWriteLatencyPeakWU << ",";
				breakdownfile << layerReadDynamicEnergyPeakFW << "," << layerReadDynamicEnergyPeakAG << "," << layerReadDynamicEnergyPeakWG << "," << layerWriteDynamicEnergyPeakWU <<",";
				breakdownfile << ", , " << coreLatencyADC << "," << coreLatencyAccum << "," << coreLatencyOther << "," <<layerbufferLatency << "," << layericLatency << "," << layerReadLatencyPeakWG << "," << layerWriteLatencyPeakWU << "," << layerDRAMLatency << ",";
				breakdownfile << coreEnergyADC << "," << coreEnergyAccum << "," << coreEnergyOther << "," << layerbufferDynamicEnergy << "," << layericDynamicEnergy << "," << layerReadDynamicEnergyPeakWG << "," << layerWriteDynamicEnergyPeakWU  << endl;
			} else {
				cout << "2. Error: the breakdown file cannot be opened!" << endl;
			}

			//cout<<"Count is "<<count<<endl;
			//cout<<"The total layer read latency is : "<<avg_layerReadLatency<<endl;
			avg_layerReadLatency = avg_layerReadLatency/count;

			cout << "------------------------------ Summary --------------------------------" <<  endl;
			cout << endl;
			cout << "------------------------------ Area Summary --------------------------------" <<  endl;
			cout << endl;
			cout << "ChipArea : " << chipArea*1e12 << " um^2" << endl;
			cout << "Total Routing Area on chip (Tile/PE local): " << chipAreaIC*1e12 << " um^2" << endl;
			cout << "Total ADC (or S/As and precharger for SRAM) Area on chip : " << chipAreaADC*1e12 << " um^2" << endl;
			cout << "Total Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) on chip : " << chipAreaAccum*1e12 << " um^2" << endl;
			cout << "Other Peripheries (e.g. decoders, mux, switchmatrix and activation units) : " << chipAreaOther*1e12 << " um^2" << endl;
			cout<<  "The total buffer area is: "<<(chipArealocalbuffer + chipAreaglobalbuffer)*1e12 << " um^2" << endl;
			// cout<<  "The total buffer area within tile is: "<<(chipArealocalbuffer)*1e12 << " um^2" << endl;
			cout<<"The total subarray (compute engine) only area is "<< chipAreasubarrayalone * 1e12 << " um^2"<<endl;
			cout << "Weight Gradient Calculation : " << chipAreaWG*1e12 << "um^2" << endl;
			
			//cout<<  "The total local buffer area is " << chipArealocalbuffer*1e12 << " um^2" << endl;
			//myarea<<"Total local Buffer Area"<<","<<chipArealocalbuffer * 1e12<<","<<"um^2"<<endl;
			//cout<<	"The total global buffer area is "<< chipAreaglobalbuffer*1e12 << " um^2" << endl;
			//cout<<	"The total global routing area is "<< chipAreaglobalIC*1e12 << " um^2" << endl;
			//cout<< 	"The total local routing area is "<< chipAreatileIC*1e12 << " um^2" << endl;
			//myarea<<"Total local Routing Area"<<","<<chipAreatileIC * 1e12<<","<<"um^2"<<endl;
			//cout<		"The total subarray (compute engine) only area is "<< chipAreasubarrayalone*1e12 << " um^2"<<endl;
			cout << "------------------------------ Area Summary --------------------------------" <<  endl;
			cout << endl;

			cout << "------------------------------ Latency Summary --------------------------------" <<  endl;

			cout<<  "The average tile latency of the chip is : "<<avg_layerReadLatency*1e9<<" ns" << endl;
			cout << "Chip total readLatency: " << chipReadLatency*1e9 << " ns" << endl;
			cout << "Chip buffer readLatency is: " << chipbufferLatency*1e9 << " ns" << endl;
			cout << "Chip Routing readLatency is: " << chipicLatency*1e9 << " ns" << endl;
			cout << "Chip AG readLatency is: " << chipReadLatencyAG*1e9 << " ns" << endl;
			cout << "Chip WG readLatency is: " << chipReadLatencyWG*1e9 << " ns" << endl;
			cout << "Chip WU writeLatency is: " << chipWriteLatencyWU*1e9 << " ns" << endl;
			
			//cout << "Chip total global routing latency is : "<<global_routinglatency_1*1e9<<" ns"<<endl;
			//cout << "Chip total tile routing latency is : "<<local_routinglatency_1*1e9<<" ns"<<endl;
			//cout << "Chip total global buffer latency is : "<<global_bufferlatency_1*1e9<<" ns"<<endl;
			//cout << "Chip total local buffer latency is : "<<local_bufferlatency_1*1e9<<" ns"<<endl;
			//mylat<<"Total local buffer Latency"<<","<<local_bufferlatency_1 * 1e9<<","<<"ns"<<endl;
			//cout << "Chip total other peripheries only latency is : "<<coreLatencyOther*1e9<<" ns"<<endl;
			cout << "************************ Breakdown of Latency *************************" << endl;
			cout << "----------- ADC (or S/As and precharger for SRAM) readLatency is : " << chipLatencyADC*1e9 << " ns" << endl;
			cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readLatency is : " << chipLatencyAccum*1e9 << " ns" << endl;
			cout << "----------- Other Peripheries (e.g. decoders, mux, switchmatrix, buffers, IC, pooling and activation units) readLatency is : " << chipLatencyOther*1e9 << " ns" << endl;
			cout<<	endl;

			cout << "************************ Energy Summary*************************" << endl;
			cout << "Chip total readDynamicEnergy: " << chipReadDynamicEnergy*1e12 << " pJ" << endl;
			cout << "Chip total leakage Energy is: " << chipLeakageEnergy*1e12 << " pJ" << endl;
			cout << "Chip total leakage Power is: " << chipLeakage*1e6 << " uW" << endl;
			cout << "Chip Routing readDynamicEnergy is: " << chipicReadDynamicEnergy*1e12 << " pJ" << endl;
			cout << "Chip Buffer readDynamicEnergy is: " << chipbufferReadDynamicEnergy*1e12 << " pJ" << endl;
			cout << "Chip Local buffer readDynamicEnergy is: " << local_bufferenergy_1*1e12 << " pJ" << endl;
			cout << "Chip Local Routing readDynamicEnergy is: " << local_routingenergy_1*1e12 << " pJ" << endl;
			cout << "Chip AG readDynamicEnergy is: " << chipReadDynamicEnergyAG *1e12 << " pJ" << endl;
			cout << "Chip WG readDynamicEnergy is: " << chipReadDynamicEnergyWG *1e12 << " pJ" << endl;
			cout << "Chip WU writeDynamicEnergy is: " << chipWriteDynamicEnergyWU *1e12 << " pJ" << endl;

			cout << "************************ Breakdown of Dynamic Energy *************************" << endl;
			//cout << "----------- Other Peripheries (e.g. decoders, mux, switchmatrix, pooling and activation units) readLatency is : " << coreLatencyOther*1e9 << " ns" << endl;
			cout << "----------- ADC (or S/As and precharger for SRAM) readEnergy is : " << chipEnergyADC*1e12 << " pJ" << endl;
			cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readEnergy is : " << chipEnergyAccum*1e12 << " pJ" << endl;
			cout << "----------- Other Peripheries (e.g. decoders, mux, switchmatrix, buffers, IC, pooling and activation units) readEnergy is : " << chipEnergyOther*1e12 << " pJ" << endl;
		
			//cout << "----------- Other Peripheries Only (e.g. decoders, mux, switchmatrix, pooling and activation units) readEnergy is : " << coreEnergyOther_only_1*1e12 << " pJ" << endl;
			cout << endl;
			//cout << "----------------------------- Performance -------------------------------" << endl;
			//cout << "Energy Efficiency TOPS/W (Layer-by-Layer Process): " << numComputation/(chipReadDynamicEnergy*1e12+chipLeakageEnergy*1e12) << endl;
			//myenergy<<"Energy Efficiency TOPS/W "<<","<<numComputation/(chipReadDynamicEnergy*1e12+chipLeakageEnergy*1e12)<<endl;
			//cout << "Throughput FPS (Layer-by-Layer Process): " << 1/(chipReadLatency) << endl;
			//myenergy<<"Throughput "<<","<<1/(chipReadLatency)<<endl;
			// std::ofstream myfile_1;
			// if (i==0)
			// {
			// 	myfile_1.open ("./to_interconnect/fps.csv");
			// }
			// else
			// {
			// 	myfile_1.open ("./to_interconnect/fps.csv", std::ios_base::app);
			// }
			
			// myfile_1 <<"Chiplet_"+ std::to_string(i+1)<<","<< 1/(chipReadLatency) <<endl;
			// myfile_1.close();
			// auto interconnect_start = chrono::high_resolution_clock::now();
			// std::ostringstream call_line;
    		// call_line << "/home/gkrish19/DAC2020_Hardware/interconnect_estimation(" << param->mode << ", " << i+1 << ").py";
    		// system(call_line.str().c_str());
			// // system(("interconnect_estimation(" + string(param->mode) + "," + layer_number + ")").c_str());
			// auto interconnect_stop = chrono::high_resolution_clock::now();
			// auto interconnect_duration = chrono::duration_cast<chrono::seconds>(interconnect_stop-interconnect_start);
			// cout << "Total Run-time of Interconnect Simulator is : " << interconnect_duration.count() << " seconds" << endl;
			// save breakdown results of each layer to csv files

		}  

		//**********************************************************************************************************
		// cout<<"Global Chiplet Level Accumulator to be evaluated"<<endl;
		// //**********************************************************************************************************

		// double CMHeight;
		// CMHeight = CMTileheight;
		// // Chiplet Final Estimation Addition
		// Chiplet_Accum_Buffer(inputParameter, tech, cell, max_excess, netStructure, desiredNumTileCM, CMHeight, l, &area, &latency, &energy, numTileEachLayer); //nktetc seg fault here for vgg8

		// std::ofstream myarea;
		// std::ofstream mylat;
		// std::ofstream myenergy;

		// myarea.open ("./Final_Results/area_chiplet.csv", std::ios_base::app);
		// mylat.open ("./Final_Results/Latency_chiplet.csv", std::ios_base::app);
		// myenergy.open ("./Final_Results/Energy_chiplet.csv", std::ios_base::app);

		// cout<<"Chiplet Level Accumulator and Buffer Estimation for Across Chiplet Summation"<<endl;
		// cout<<"Total Chiplet Accumulator and Buffer Area is "<<area *1e12<<"um2"<<endl;
		// myarea<<"Chiplet_Global_Accumulation"<<","<<"Chip Area"<<","<<(area) * 1e12<<","<<"um^2"<<endl;
		// cout<<"Total Chiplet Accumulator and Buffer Latency is "<<latency *1e9 << " ns" << endl;
		// mylat<<"Chiplet_Global_Accumulator"<<","<<"Total readLatency"<<","<<(latency) * 1e9<<","<<"ns"<<endl;
		// cout<<"Total Chiplet Accumulator and Buffer Energy is "<<energy *1e9 << " ns" << endl;
		// myenergy<<"Chiplet_Global_Accumulator"<<","<<"Total readEnergy"<<","<<(energy) * 1e12<<","<<"pJ"<<endl;

		// myenergy.close();
		// mylat.close();
		// myarea.close();		

		double overall_utilization;
		if (param->fixed_cnt_chiplet)
		{
			overall_utilization = (avg_utilization_chiplet/ (double)(num_chip+1));
		}
		else
		{
			overall_utilization = (avg_utilization_chiplet/ (double)(num_chip+1));
		}
		
		cout<<"*************************************************************************************************"<<endl;
		// cout<<"The average utilization sum of the chiplets in the whole 2.5D fabric is "<<avg_utilization_chiplet<<" %"<<endl;
		cout<<"The overall average utilization of the memory in the whole 2.5D fabric is "<<overall_utilization<<" %"<<endl;
		cout<<"The number of used chiplets are: "<<(num_chip_final+1)<<endl;
		cout<<"*************************************************************************************************"<<endl;
		cout << "-------------------------------------- Hardware Performance Done --------------------------------------" <<  endl;
		cout << endl;	
	

	cout << "Chip readLatency of Forward (per epoch) is: " << chipReadLatency*1e9 << "ns" << endl;
	cout << "Chip readDynamicEnergy of Forward (per epoch) is: " << chipReadDynamicEnergy*1e12 << "pJ" << endl;
	cout << "Chip readLatency of Activation Gradient (per epoch) is: " << chipReadLatencyAG*1e9 << "ns" << endl;
	cout << "Chip readDynamicEnergy of Activation Gradient (per epoch) is: " << chipReadDynamicEnergyAG*1e12 << "pJ" << endl;
	cout << "Chip readLatency of Weight Gradient (per epoch) is: " << chipReadLatencyWG*1e9 << "ns" << endl;
	cout << "Chip readDynamicEnergy of Weight Gradient (per epoch) is: " << chipReadDynamicEnergyWG*1e12 << "pJ" << endl;
	cout << "Chip writeLatency of Weight Update (per epoch) is: " << chipWriteLatencyWU*1e9 << "ns" << endl;
	cout << "Chip writeDynamicEnergy of Weight Update (per epoch) is: " << chipWriteDynamicEnergyWU*1e12 << "pJ" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Chip total Latency (per epoch) is: " << (chipReadLatency+chipReadLatencyAG+chipReadLatencyWG+chipWriteLatencyWU)*1e9 << "ns" << endl;
	cout << "Chip total Energy (per epoch) is: " << (chipReadDynamicEnergy+chipReadDynamicEnergyAG+chipReadDynamicEnergyWG+chipWriteDynamicEnergyWU)*1e12 << "pJ" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Chip PEAK readLatency of Forward (per epoch) is: " << chipReadLatencyPeakFW*1e9 << "ns" << endl;
	cout << "Chip PEAK readDynamicEnergy of Forward (per epoch) is: " << chipReadDynamicEnergyPeakFW*1e12 << "pJ" << endl;
	cout << "Chip PEAK readLatency of Activation Gradient (per epoch) is: " << chipReadLatencyPeakAG*1e9 << "ns" << endl;
	cout << "Chip PEAK readDynamicEnergy of Activation Gradient (per epoch) is: " << chipReadDynamicEnergyPeakAG*1e12 << "pJ" << endl;
	cout << "Chip PEAK readLatency of Weight Gradient (per epoch) is: " << chipReadLatencyPeakWG*1e9 << "ns" << endl;
	cout << "Chip PEAK readDynamicEnergy of Weight Gradient (per epoch) is: " << chipReadDynamicEnergyPeakWG*1e12 << "pJ" << endl;
	cout << "Chip PEAK writeLatency of Weight Update (per epoch) is: " << chipWriteLatencyPeakWU*1e9 << "ns" << endl;
	cout << "Chip PEAK writeDynamicEnergy of Weight Update (per epoch) is: " << chipWriteDynamicEnergyPeakWU*1e12 << "pJ" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Chip PEAK total Latency (per epoch) is: " << (chipReadLatencyPeakFW+chipReadLatencyPeakAG+chipReadLatencyPeakWG+chipWriteLatencyPeakWU)*1e9 << "ns" << endl;
	cout << "Chip PEAK total Energy (per epoch) is: " << (chipReadDynamicEnergyPeakFW+chipReadDynamicEnergyPeakAG+chipReadDynamicEnergyPeakWG+chipWriteDynamicEnergyPeakWU)*1e12 << "pJ" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Chip leakage Energy is: " << chipLeakageEnergy*1e12 << "pJ" << endl;
	cout << "Chip leakage Power is: " << chipLeakage*1e6 << "uW" << endl;
	cout << endl;
	cout << "************************ Breakdown of Latency and Dynamic Energy *************************" << endl;
	cout << endl;
	cout << "----------- ADC (or S/As and precharger for SRAM) readLatency is : " << chipLatencyADC*1e9 << "ns" << endl;
	cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readLatency is : " << chipLatencyAccum*1e9 << "ns" << endl;
	cout << "----------- Synaptic Array w/o ADC (Forward + Activate Gradient) readLatency is : " << chipLatencyOther*1e9 << "ns" << endl;
	cout << "----------- Buffer readLatency is: " << chipbufferLatency*1e9 << "ns" << endl;
	cout << "----------- Interconnect readLatency is: " << chipicLatency*1e9 << "ns" << endl;
	cout << "----------- Weight Gradient Calculation readLatency is : " << chipReadLatencyPeakWG*1e9 << "ns" << endl;
	cout << "----------- Weight Update writeLatency is : " << chipWriteLatencyPeakWU*1e9 << "ns" << endl;
	cout << "----------- DRAM data transfer Latency is : " << chipDRAMLatency*1e9 << "ns" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "----------- ADC (or S/As and precharger for SRAM) readDynamicEnergy is : " << chipEnergyADC*1e12 << "pJ" << endl;
	cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readDynamicEnergy is : " << chipEnergyAccum*1e12 << "pJ" << endl;
	cout << "----------- Synaptic Array w/o ADC (Forward + Activate Gradient) readDynamicEnergy is : " << chipEnergyOther*1e12 << "pJ" << endl;
	cout << "----------- Buffer readDynamicEnergy is: " << chipbufferReadDynamicEnergy*1e12 << "pJ" << endl;
	cout << "----------- Interconnect readDynamicEnergy is: " << chipicReadDynamicEnergy*1e12 << "pJ" << endl;
	cout << "----------- Weight Gradient Calculation readDynamicEnergy is : " << chipReadDynamicEnergyPeakWG*1e12 << "pJ" << endl;
	cout << "----------- Weight Update writeDynamicEnergy is : " << chipWriteDynamicEnergyPeakWU*1e12 << "pJ" << endl;
	cout << "----------- DRAM data transfer DynamicEnergy is : " << chipDRAMDynamicEnergy*1e12 << "pJ" << endl;
	cout << endl;
	cout << "************************ Breakdown of Latency and Dynamic Energy *************************" << endl;
	cout << endl;
	cout << endl;
	cout << "-----------------------------------Chip layer-by-layer Performance---------------------------------" << endl;
	cout << "Energy Efficiency TOPS/W: " << numComputation/((chipReadDynamicEnergy+chipLeakageEnergy+chipReadDynamicEnergyAG+chipReadDynamicEnergyWG+chipWriteDynamicEnergyWU)*1e12) << endl;
	cout << "Throughput TOPS: " << numComputation/(chipReadLatency+chipReadLatencyAG+chipReadLatencyWG+chipWriteLatencyWU)*1e-12 << endl;
	cout << "Throughput FPS: " << 1/(chipReadLatency+chipReadLatencyAG+chipReadLatencyWG+chipWriteLatencyWU) << endl;
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "Peak Energy Efficiency TOPS/W: " << numComputation/((chipReadDynamicEnergyPeakFW+chipReadDynamicEnergyPeakAG+chipReadDynamicEnergyPeakWG+chipWriteDynamicEnergyPeakWU)*1e12) << endl;
	cout << "Peak Throughput TOPS: " << numComputation/(chipReadLatencyPeakFW+chipReadLatencyPeakAG+chipReadLatencyPeakWG+chipWriteLatencyPeakWU)*1e-12 << endl;
	cout << "Peak Throughput FPS: " << 1/(chipReadLatencyPeakFW+chipReadLatencyPeakAG+chipReadLatencyPeakWG+chipWriteLatencyPeakWU) << endl;
	
	cout << "-------------------------------------- Hardware Performance Done --------------------------------------" <<  endl;
	cout << endl;
	// auto stop = chrono::high_resolution_clock::now();
	// auto duration = chrono::duration_cast<chrono::seconds>(stop-start);
    // cout << "------------------------------ Simulation Performance --------------------------------" <<  endl;
	// cout << "Total Run-time of NeuroSim: " << duration.count() << " seconds" << endl;
	// cout << "------------------------------ Simulation Performance --------------------------------" <<  endl;
	
	// save results to top level csv file (only total results)
	ofstream outfile;
	outfile.open ("SIAM_Output.csv", ios::app);
	if (outfile.is_open()) {
			outfile << "chipReadLatency" << "," << "chipReadLatencyAG" << "," << "chipReadLatencyWG" << "," << "chipWriteLatencyWU" << ",";
			outfile << "chipReadDynamicEnergy" << "," << "chipReadDynamicEnergyAG" << "," << "chipReadDynamicEnergyWG" << "," << "chipWriteDynamicEnergyWU" << ",";
			outfile << "chipReadLatencyPeakFW" << "," << "chipReadLatencyPeakAG" << "," << "chipReadLatencyPeakWG" << "," << "chipWriteLatencyPeakWU" << ",";
			outfile << "chipReadDynamicEnergyPeakFW" << "," << "chipReadDynamicEnergyPeakAG" << "," << "chipReadDynamicEnergyPeakWG" << "," << "chipWriteDynamicEnergyPeakWU" << ",";
			outfile << "numComputation / total ReadEnergy" << ",";
			outfile << "numComputation / total ReadLatency"  << ",";
			outfile << "numComputation / peak ReadEnergy" << ",";
			outfile << "numComputation / peak ReadLatency" << endl;

		outfile << chipReadLatency << "," << chipReadLatencyAG << "," << chipReadLatencyWG << "," << chipWriteLatencyWU << ",";
		outfile << chipReadDynamicEnergy << "," << chipReadDynamicEnergyAG << "," << chipReadDynamicEnergyWG << "," << chipWriteDynamicEnergyWU << ",";
		outfile << chipReadLatencyPeakFW << "," << chipReadLatencyPeakAG << "," << chipReadLatencyPeakWG << "," << chipWriteLatencyPeakWU << ",";
		outfile << chipReadDynamicEnergyPeakFW << "," << chipReadDynamicEnergyPeakAG << "," << chipReadDynamicEnergyPeakWG << "," << chipWriteDynamicEnergyPeakWU << ",";
		outfile << numComputation/((chipReadDynamicEnergy+chipLeakageEnergy+chipReadDynamicEnergyAG+chipReadDynamicEnergyWG+chipWriteDynamicEnergyWU)*1e12) << ",";
		outfile << numComputation/(chipReadLatency+chipReadLatencyAG+chipReadLatencyWG+chipWriteLatencyWU)*1e-12 << ",";
		outfile << numComputation/((chipReadDynamicEnergyPeakFW+chipReadDynamicEnergyPeakAG+chipReadDynamicEnergyPeakWG+chipWriteDynamicEnergyPeakWU)*1e12) << ",";
		outfile << numComputation/(chipReadLatencyPeakFW+chipReadLatencyPeakAG+chipReadLatencyPeakWG+chipWriteLatencyPeakWU)*1e-12 << endl;
	} else {
		cout << "Error: the output file cannot be opened!" << endl;
	}
	outfile.close();
	
	}
	else 
	{
		//******************************************************** This is the regular Single Chip Mode**************************************************************
		//***********************************************************************************************************************************************************
		int residue_area_cal;
		markNM = ChipDesignInitialize(inputParameter, tech, cell, false, netStructure, &maxPESizeNM, &maxTileSizeCM, &numPENM, param->mode);	
		pipelineSpeedUp = ChipDesignInitialize(inputParameter, tech, cell, true, netStructure, &maxPESizeNM, &maxTileSizeCM, &numPENM, param->mode);

		numTileEachLayer = ChipFloorPlan(true, false, false, netStructure, markNM,
						maxPESizeNM, maxTileSizeCM, numPENM, pipelineSpeedUp,
						&desiredNumTileNM, &desiredPESizeNM, &desiredNumTileCM, &desiredTileSizeCM, &desiredPESizeCM, &numTileRow, &numTileCol);

		utilizationEachLayer = ChipFloorPlan(false, true, false, netStructure, markNM,
						maxPESizeNM, maxTileSizeCM, numPENM, pipelineSpeedUp,
						&desiredNumTileNM, &desiredPESizeNM, &desiredNumTileCM, &desiredTileSizeCM, &desiredPESizeCM, &numTileRow, &numTileCol);

		speedUpEachLayer = ChipFloorPlan(false, false, true, netStructure, markNM,
						maxPESizeNM, maxTileSizeCM, numPENM, pipelineSpeedUp,
						&desiredNumTileNM, &desiredPESizeNM, &desiredNumTileCM, &desiredTileSizeCM, &desiredPESizeCM, &numTileRow, &numTileCol);

		tileLocaEachLayer = ChipFloorPlan(false, false, false, netStructure, markNM,
						maxPESizeNM, maxTileSizeCM, numPENM, pipelineSpeedUp,
						&desiredNumTileNM, &desiredPESizeNM, &desiredNumTileCM, &desiredTileSizeCM, &desiredPESizeCM, &numTileRow, &numTileCol);

		cout << "------------------------------ FloorPlan --------------------------------" <<  endl;
		cout << endl;
		//cout << "Tile and PE size are optimized to maximize memory utilization ( = memory mapped by synapse / total memory on chip)" << endl;
		cout << endl;
		if (!param->novelMapping) {
			cout << "Desired Conventional Mapped Tile Storage Size: " << desiredTileSizeCM << "x" << desiredTileSizeCM << endl;
			cout << "Desired Conventional PE Storage Size: " << desiredPESizeCM << "x" << desiredPESizeCM << endl;
		} else {
			cout << "Desired Conventional Mapped Tile Storage Size: " << desiredTileSizeCM << "x" << desiredTileSizeCM << endl;
			cout << "Desired Conventional PE Storage Size: " << desiredPESizeCM << "x" << desiredPESizeCM << endl;
			cout << "Desired Novel Mapped Tile Storage Size: " << numPENM << "x" << desiredPESizeNM << "x" << desiredPESizeNM << endl;
		}
		cout << "User-defined SubArray Size: " << param->numRowSubArray << "x" << param->numColSubArray << endl;
		cout << endl;
		cout << "----------------- # of tile used for each layer -----------------" <<  endl;

		std::ofstream myfile;
		myfile.open ("./to_interconnect/num_tiles_per_layer.csv"); //Dumps file for the number of tile per layer for the interconnect simulator.
		for (int i=0; i<netStructure.size(); i++) {
			cout << "layer" << i+1 << ": " << numTileEachLayer[0][i] * numTileEachLayer[1][i] << endl;
			//myfile << i+1 <<","<< numTileEachLayer[0][i] * numTileEachLayer[1][i] << endl;
			myfile << numTileEachLayer[0][i] * numTileEachLayer[1][i] << endl;
			totalNumTile += numTileEachLayer[0][i] * numTileEachLayer[1][i];
		}
		myfile.close();
		cout << endl;
		/*
		cout << "----------------- Speed-up of each layer ------------------" <<  endl;
		for (int i=0; i<netStructure.size(); i++) {
			cout << "layer" << i+1 << ": " << speedUpEachLayer[0][i] << ", " << speedUpEachLayer[1][i] << endl;
		}*/
		cout << endl;
		cout << "----------------- Utilization of each layer ------------------" <<  endl;
		
		for (int i=0; i<netStructure.size(); i++) {
			cout << "layer" << i+1 << ": " << utilizationEachLayer[i][0] << endl;
			realMappedMemory += numTileEachLayer[0][i] * numTileEachLayer[1][i] * utilizationEachLayer[i][0];
		}
		cout<<"The total number of tiles are : "<<totalNumTile<<endl;
		cout<<"The real Mapped memory is : "<<realMappedMemory<<endl;
		cout << "Memory Utilization of Whole Chip: " << realMappedMemory/totalNumTile*100 << " % " << endl;
		cout << endl;
		cout << "---------------------------- FloorPlan Done ------------------------------" <<  endl;
		cout << endl;
		cout << endl;
		cout << endl;	
		for (int i=0; i<netStructure.size(); i++) {
			numComputation += 2*(netStructure[i][0] * netStructure[i][1] * netStructure[i][2] * netStructure[i][3] * netStructure[i][4] * netStructure[i][5] * netStructure[i][7]); //nocs 2* because FF and BP
		}
		//cout<<"The total number of computations are : "<<numComputation<<endl;
 	//nocs
		if (param->trainingEstimation) {
		numComputation *= 3;  // forward, computation of activation gradient, weight gradient
		numComputation *= param->batchSize * param->numIteration;  // count for one epoch
	}	
	//nocs
		ChipInitialize(inputParameter, tech, cell, netStructure, markNM, numTileEachLayer,
						numPENM, desiredNumTileNM, desiredPESizeNM, desiredNumTileCM, desiredTileSizeCM, desiredPESizeCM, numTileRow, numTileCol, &numArrayWriteParallel);

		// if (param->fixed_cnt_chiplet)
		// {
		// 	if (desiredNumTileCM < param->cnt_chiplet)
		// 	{
		// 		desiredNumTileCM = param->cnt_chiplet;
		// 		numTileRow = ceil((double)sqrt((double)(desiredNumTileCM)));
		// 		numTileCol = ceil((double)((desiredNumTileCM))/(double)(numTileRow));				
		// 	}
				
		// 	else
		// 	{
		// 		desiredNumTileCM = desiredNumTileCM;				
		// 	}
		// 	cout<<"********************Large Chip Designed with fixed number of Tiles inside********************"<<endl;
		// }
		
		

		chipAreaResults = ChipCalculateArea(inputParameter, tech, cell, desiredNumTileNM, numPENM, desiredPESizeNM, desiredNumTileCM, desiredTileSizeCM, desiredPESizeCM, numTileRow,
						&chipHeight, &chipWidth, &CMTileheight, &CMTilewidth, &NMTileheight, &NMTilewidth, 1);
		
		chipArea = chipAreaResults[0];
		chipAreaIC = chipAreaResults[1];
		chipAreaADC = chipAreaResults[2];
		chipAreaAccum = chipAreaResults[3];
		chipAreaOther = chipAreaResults[4];
		chipArealocalbuffer = chipAreaResults[5];
		chipAreaglobalbuffer = chipAreaResults[6];
		//chipAreaOtheralone = chipAreaResults[7];
		chipAreaglobalIC = chipAreaResults[7];
		chipAreatileIC = chipAreaResults[8];
		chipAreasubarrayalone=chipAreaResults[9];
		// chipAreaArray = chipAreaResults[10]; //nocs

		// if (param->fixed_cnt_chiplet)
		// 	{		
		// 		if (desiredNumTileCM < param->cnt_chiplet)		
		// 			cout<<"This is the residual area addition for the remaining unused tiles."<<endl;
		// 			residue_area_cal = param->cnt_chiplet - desiredNumTileCM;
		// 			chipArea += chipAreaResults[0] * residue_area_cal;
		// 			chipAreaIC += chipAreaResults[1]* residue_area_cal;
		// 			chipAreaADC += chipAreaResults[2]* residue_area_cal;
		// 			chipAreaAccum += chipAreaResults[3]* residue_area_cal;
		// 			chipAreaOther += chipAreaResults[4]* residue_area_cal;
		// 			chipArealocalbuffer += chipAreaResults[5]* residue_area_cal;
		// 			chipAreaglobalbuffer += chipAreaResults[6]* residue_area_cal;
		// 			//chipAreaOtheralone = chipAreaResults[7];
		// 			chipAreaglobalIC += chipAreaResults[7]* residue_area_cal;
		// 			chipAreatileIC += chipAreaResults[8]* residue_area_cal;
		// 			chipAreasubarrayalone +=chipAreaResults[9]* residue_area_cal;
				
		// 	}


		std::ofstream myarea;
		myarea.open ("./Final_Results/area.csv");
		myarea<<"Subarray Area"<<","<<chipAreasubarrayalone * 1e12<<","<<"um^2"<<endl;
		//cout<<"The total subarray (compute engine) only area is "<< chipAreasubarrayalone * 1e12 << " um^2"<<endl;
		cout << "-------------------------------------- Hardware Performance --------------------------------------" <<  endl;

//nocs
			// save breakdown results of each layer to csv files
	ofstream breakdownfile;
	string breakdownfile_name = "./SIAM_Results_Each_Epoch/SIAM_Breakdown_Epoch_";
	breakdownfile_name.append(argv[1]);
	breakdownfile_name.append(".csv");
	breakdownfile.open (breakdownfile_name, ios::app);
	if (breakdownfile.is_open()) {
		// firstly save the area results to file
		breakdownfile << "Total Area(m^2), Total CIM (FW+AG) Area (m^2), Routing Area(m^2), ADC Area(m^2), Accumulation Area(m^2), Other Logic&Storage Area(m^2), Weight Gradient Area(m^2),"<< endl;
		breakdownfile << chipArea << "," << chipAreaArray << "," << chipAreaIC << "," << chipAreaADC << "," << chipAreaAccum << "," << chipAreaOther << "," << chipAreaWG << endl;
		breakdownfile << endl;
		breakdownfile << endl;
		breakdownfile << "layer_number, latency_FW(s), latency_AG(s), latency_WG(s), latency_WU(s), energy_FW(J), energy_AG(J), energy_WG(J), energy_WU(J),";
		breakdownfile << "Peak_latency_FW(s), Peak_latency_AG(s), Peak_latency_WG(s), Peak_latency_WU(s), Peak_energy_FW(J), Peak_energy_AG(J), Peak_energy_WG(J), Peak_energy_WU(J),";
		breakdownfile << ", , ADC_latency(s), Accumulation_latency(s), Synaptic Array w/o ADC_latency(s), Buffer_latency(s), IC_latency(s), Weight_gradient_latency(s), Weight_update(s), DRAM_latency(s), ";
		breakdownfile << "ADC_energy(J), Accumulation_energy(J), Synaptic Array w/o ADC_energy(J), Buffer_energy(J), IC_energy(J), Weight_gradient_energy(J), Weight_update_energy(J), DRAM_energy(J)" << endl;
	} else {
		cout << "Error: the breakdown file cannot be opened!" << endl;
	}

		// layer-by-layer process
		// show the detailed hardware performance for each layer
		for (int i=0; i<netStructure.size(); i++) {
			cout << "-------------------- Estimation of Layer " << i+1 << " ----------------------" << endl;
			
			param->activityRowReadWG = atof(argv[4*i+8]);
                        param->activityRowWriteWG = atof(argv[4*i+8]);
                        param->activityColWriteWG = atof(argv[4*i+8]);
			
			ChipCalculatePerformance(inputParameter, tech, cell, i, argv[4*i+5], argv[4*i+6], argv[4*i+7], netStructure[i][6],
						netStructure, markNM, numTileEachLayer, utilizationEachLayer, speedUpEachLayer, tileLocaEachLayer,
						numPENM, desiredPESizeNM, desiredTileSizeCM, desiredPESizeCM, CMTileheight, CMTilewidth, NMTileheight, NMTilewidth, numArrayWriteParallel,
						&layerReadLatency, &layerReadDynamicEnergy, &tileLeakage, &layerReadLatencyAG, &layerReadDynamicEnergyAG, &layerReadLatencyWG, &layerReadDynamicEnergyWG, 
						&layerWriteLatencyWU, &layerWriteDynamicEnergyWU, &layerbufferLatency, &layerbufferDynamicEnergy, &layericLatency, &layericDynamicEnergy,
						&coreLatencyADC, &coreLatencyAccum, &coreLatencyOther, &coreEnergyADC, &coreEnergyAccum, &coreEnergyOther, &layerDRAMLatency, &layerDRAMDynamicEnergy,
						&layerReadLatencyPeakFW, &layerReadDynamicEnergyPeakFW, &layerReadLatencyPeakAG, &layerReadDynamicEnergyPeakAG,
						&layerReadLatencyPeakWG, &layerReadDynamicEnergyPeakWG, &layerWriteLatencyPeakWU, &layerWriteDynamicEnergyPeakWU, &global_routinglatency, &test, &local_bufferlatency, &local_bufferenergy
						, &local_routinglatency, &local_routingenergy, &global_routingenergy, &global_bufferlatency, &global_bufferenergy, 1);
			
			double numTileOtherLayer = 0;
			double layerLeakageEnergy = 0;		
			for (int j=0; j<netStructure.size(); j++) {
				if (j != i) {
					numTileOtherLayer += numTileEachLayer[0][j] * numTileEachLayer[1][j];
				}
			}
			layerLeakageEnergy = numTileOtherLayer*tileLeakage*(layerReadLatency+layerReadLatencyAG);
			
			cout << "layer" << i+1 << "'s readLatency of Forward is: " << layerReadLatency*1e9 << "ns" << endl;
			cout << "layer" << i+1 << "'s readDynamicEnergy of Forward is: " << layerReadDynamicEnergy*1e12 << "pJ" << endl;
			cout << "layer" << i+1 << "'s readLatency of Activation Gradient is: " << layerReadLatencyAG*1e9 << "ns" << endl;
			cout << "layer" << i+1 << "'s readDynamicEnergy of Activation Gradient is: " << layerReadDynamicEnergyAG*1e12 << "pJ" << endl;
			cout << "layer" << i+1 << "'s readLatency of Weight Gradient is: " << layerReadLatencyWG*1e9 << "ns" << endl;
			cout << "layer" << i+1 << "'s readDynamicEnergy of Weight Gradient is: " << layerReadDynamicEnergyWG*1e12 << "pJ" << endl;
			cout << "layer" << i+1 << "'s writeLatency of Weight Update is: " << layerWriteLatencyWU*1e9 << "ns" << endl;
			cout << "layer" << i+1 << "'s writeDynamicEnergy of Weight Update is: " << layerWriteDynamicEnergyWU*1e12 << "pJ" << endl;
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "layer" << i+1 << "'s PEAK readLatency of Forward is: " << layerReadLatencyPeakFW*1e9 << "ns" << endl;
			cout << "layer" << i+1 << "'s PEAK readDynamicEnergy of Forward is: " << layerReadDynamicEnergyPeakFW*1e12 << "pJ" << endl;
			cout << "layer" << i+1 << "'s PEAK readLatency of Activation Gradient is: " << layerReadLatencyPeakAG*1e9 << "ns" << endl;
			cout << "layer" << i+1 << "'s PEAK readDynamicEnergy of Activation Gradient is: " << layerReadDynamicEnergyPeakAG*1e12 << "pJ" << endl;
			cout << "layer" << i+1 << "'s PEAK readLatency of Weight Gradient is: " << layerReadLatencyPeakWG*1e9 << "ns" << endl;
			cout << "layer" << i+1 << "'s PEAK readDynamicEnergy of Weight Gradient is: " << layerReadDynamicEnergyPeakWG*1e12 << "pJ" << endl;
			cout << "layer" << i+1 << "'s PEAK writeLatency of Weight Update is: " << layerWriteLatencyPeakWU*1e9 << "ns" << endl;
			cout << "layer" << i+1 << "'s PEAK writeDynamicEnergy of Weight Update is: " << layerWriteDynamicEnergyPeakWU*1e12 << "pJ" << endl;
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "layer" << i+1 << "'s leakagePower is: " << numTileEachLayer[0][i] * numTileEachLayer[1][i] * tileLeakage*1e6 << "uW" << endl;
			cout << "layer" << i+1 << "'s leakageEnergy is: " << layerLeakageEnergy*1e12 << "pJ" << endl;

			cout << endl;
			cout << "************************ Breakdown of Latency and Dynamic Energy *************************" << endl;
			cout << endl;
			cout << "----------- ADC (or S/As and precharger for SRAM) readLatency is : " << coreLatencyADC*1e9 << "ns" << endl;
			cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readLatency is : " << coreLatencyAccum*1e9 << "ns" << endl;
			cout << "----------- Synaptic Array w/o ADC (Forward + Activate Gradient) readLatency is : " << coreLatencyOther*1e9 << "ns" << endl;
			cout << "----------- Buffer buffer latency is: " << layerbufferLatency*1e9 << "ns" << endl;
			cout << "----------- Interconnect latency is: " << layericLatency*1e9 << "ns" << endl;
			cout << "----------- Weight Gradient Calculation readLatency is : " << layerReadLatencyPeakWG*1e9 << "ns" << endl;
			cout << "----------- Weight Update writeLatency is : " << layerWriteLatencyPeakWU*1e9 << "ns" << endl;
			cout << "----------- DRAM data transfer Latency is : " << layerDRAMLatency*1e9 << "ns" << endl;
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "----------- ADC (or S/As and precharger for SRAM) readDynamicEnergy is : " << coreEnergyADC*1e12 << "pJ" << endl;
			cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readDynamicEnergy is : " << coreEnergyAccum*1e12 << "pJ" << endl;
			cout << "----------- Synaptic Array w/o ADC (Forward + Activate Gradient) readDynamicEnergy is : " << coreEnergyOther*1e12 << "pJ" << endl;
			cout << "----------- Buffer readDynamicEnergy is: " << layerbufferDynamicEnergy*1e12 << "pJ" << endl;
			cout << "----------- Interconnect readDynamicEnergy is: " << layericDynamicEnergy*1e12 << "pJ" << endl;
			cout << "----------- Weight Gradient Calculation readDynamicEnergy is : " << layerReadDynamicEnergyPeakWG*1e12 << "pJ" << endl;
			cout << "----------- Weight Update writeDynamicEnergy is : " << layerWriteDynamicEnergyPeakWU*1e12 << "pJ" << endl;
			cout << "----------- DRAM data transfer Energy is : " << layerDRAMDynamicEnergy*1e12 << "pJ" << endl;
			cout << endl;
			
			cout << "************************ Breakdown of Latency and Dynamic Energy *************************" << endl;
			cout << endl;
			
			
			if (breakdownfile.is_open()) {
				breakdownfile << i+1 << "," << layerReadLatency << "," << layerReadLatencyAG << "," << layerReadLatencyWG << "," << layerWriteLatencyWU << ",";
				breakdownfile << layerReadDynamicEnergy << "," << layerReadDynamicEnergyAG << "," << layerReadDynamicEnergyWG << "," << layerWriteDynamicEnergyWU << ",";
				breakdownfile << layerReadLatencyPeakFW << "," << layerReadLatencyPeakAG << "," << layerReadLatencyPeakWG << "," << layerWriteLatencyPeakWU << ",";
				breakdownfile << layerReadDynamicEnergyPeakFW << "," << layerReadDynamicEnergyPeakAG << "," << layerReadDynamicEnergyPeakWG << "," << layerWriteDynamicEnergyPeakWU <<",";
				breakdownfile << ", , " << coreLatencyADC << "," << coreLatencyAccum << "," << coreLatencyOther << "," <<layerbufferLatency << "," << layericLatency << "," << layerReadLatencyPeakWG << "," << layerWriteLatencyPeakWU << "," << layerDRAMLatency << ",";
				breakdownfile << coreEnergyADC << "," << coreEnergyAccum << "," << coreEnergyOther << "," << layerbufferDynamicEnergy << "," << layericDynamicEnergy << "," << layerReadDynamicEnergyPeakWG << "," << layerWriteDynamicEnergyPeakWU << "," << layerDRAMDynamicEnergy << endl;
			} else {
				cout << "Error: the breakdown file cannot be opened!" << endl;
			}
			
			chipReadLatency += layerReadLatency;
			chipReadDynamicEnergy += layerReadDynamicEnergy;
			chipReadLatencyAG += layerReadLatencyAG;
			chipReadDynamicEnergyAG += layerReadDynamicEnergyAG;
			chipReadLatencyWG += layerReadLatencyWG;
			chipReadDynamicEnergyWG += layerReadDynamicEnergyWG;
			chipWriteLatencyWU += layerWriteLatencyWU;
			chipWriteDynamicEnergyWU += layerWriteDynamicEnergyWU;
			chipDRAMLatency += layerDRAMLatency;
			chipDRAMDynamicEnergy += layerDRAMDynamicEnergy;
			
			chipReadLatencyPeakFW += layerReadLatencyPeakFW;
			chipReadDynamicEnergyPeakFW += layerReadDynamicEnergyPeakFW;
			chipReadLatencyPeakAG += layerReadLatencyPeakAG;
			chipReadDynamicEnergyPeakAG += layerReadDynamicEnergyPeakAG;
			chipReadLatencyPeakWG += layerReadLatencyPeakWG;
			chipReadDynamicEnergyPeakWG += layerReadDynamicEnergyPeakWG;
			chipWriteLatencyPeakWU += layerWriteLatencyPeakWU;
			chipWriteDynamicEnergyPeakWU += layerWriteDynamicEnergyPeakWU;
			
			chipLeakageEnergy += layerLeakageEnergy;
			chipLeakage += tileLeakage*numTileEachLayer[0][i] * numTileEachLayer[1][i];
			chipbufferLatency += layerbufferLatency;
			chipbufferReadDynamicEnergy += layerbufferDynamicEnergy;
			chipicLatency += layericLatency;
			chipicReadDynamicEnergy += layericDynamicEnergy;
			
			chipLatencyADC += coreLatencyADC;
			chipLatencyAccum += coreLatencyAccum;
			chipLatencyOther += coreLatencyOther;
			chipEnergyADC += coreEnergyADC;
			chipEnergyAccum += coreEnergyAccum;
			chipEnergyOther += coreEnergyOther;
		}

	if (breakdownfile.is_open()) {
		breakdownfile << "Total" << "," << chipReadLatency << "," << chipReadLatencyAG << "," << chipReadLatencyWG << "," << chipWriteLatencyWU << ",";
		breakdownfile << chipReadDynamicEnergy << "," << chipReadDynamicEnergyAG << "," << chipReadDynamicEnergyWG << "," << chipWriteDynamicEnergyWU << ",";
		breakdownfile << chipReadLatencyPeakFW << "," << chipReadLatencyPeakAG << "," << chipReadLatencyPeakWG << "," << chipWriteLatencyPeakWU << ",";
		breakdownfile << chipReadDynamicEnergyPeakFW << "," << chipReadDynamicEnergyPeakAG << "," << chipReadDynamicEnergyPeakWG << "," << chipWriteDynamicEnergyPeakWU << ",";
		breakdownfile << ", , " << chipLatencyADC << "," << chipLatencyAccum << "," << chipLatencyOther << "," << chipbufferLatency << "," << chipicLatency << "," << chipReadLatencyPeakWG <<"," << chipWriteLatencyPeakWU <<"," << chipDRAMLatency <<",";
		breakdownfile << chipEnergyADC << "," << chipEnergyAccum << "," << chipEnergyOther << "," << chipbufferReadDynamicEnergy << "," << chipicReadDynamicEnergy << "," << chipReadDynamicEnergyPeakWG << "," << chipWriteDynamicEnergyPeakWU << "," << chipDRAMDynamicEnergy << endl;
		breakdownfile << endl;
		breakdownfile << endl;
		breakdownfile << "TOPS/W,FPS,TOPS,Peak TOPS/W,Peak FPS,Peak TOPS," << endl;
		breakdownfile << numComputation/((chipReadDynamicEnergy+chipLeakageEnergy+chipReadDynamicEnergyAG+chipReadDynamicEnergyWG+chipWriteDynamicEnergyWU)*1e12) << ",";
		breakdownfile <<  1/(chipReadLatency+chipReadLatencyAG+chipReadLatencyWG+chipWriteLatencyWU) << ",";
		breakdownfile <<  numComputation/(chipReadLatency+chipReadLatencyAG+chipReadLatencyWG+chipWriteLatencyWU)*1e-12 << ",";
		breakdownfile <<  numComputation/((chipReadDynamicEnergyPeakFW+chipReadDynamicEnergyPeakAG+chipReadDynamicEnergyPeakWG+chipWriteDynamicEnergyPeakWU)*1e12) << ",";
		breakdownfile <<  1/(chipReadLatencyPeakFW+chipReadLatencyPeakAG+chipReadLatencyPeakWG+chipWriteLatencyPeakWU) << ",";
		breakdownfile <<  numComputation/(chipReadLatencyPeakFW+chipReadLatencyPeakAG+chipReadLatencyPeakWG+chipWriteLatencyPeakWU)*1e-12 << endl;
	} else {
		cout << "Error: the breakdown file cannot be opened!" << endl;
	}
	
	breakdownfile.close();
	
	cout << "------------------------------ Summary --------------------------------" <<  endl;
	cout << endl;
	cout << "ChipArea : " << chipArea*1e12 << "um^2" << endl;
	cout << "Chip total CIM (Forward+Activation Gradient) array : " << chipAreaArray*1e12 << "um^2" << endl;
	cout << "Total IC Area on chip (Global and Tile/PE local): " << chipAreaIC*1e12 << "um^2" << endl;
	cout << "Total ADC (or S/As and precharger for SRAM) Area on chip : " << chipAreaADC*1e12 << "um^2" << endl;
	cout << "Total Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) on chip : " << chipAreaAccum*1e12 << "um^2" << endl;
	cout << "Other Peripheries (e.g. decoders, mux, switchmatrix, buffers, pooling and activation units) : " << chipAreaOther*1e12 << "um^2" << endl;
	cout << "Weight Gradient Calculation : " << chipAreaWG*1e12 << "um^2" << endl;
	cout << endl;
	cout << "-----------------------------------Chip layer-by-layer Estimation---------------------------------" << endl;

	cout << "Chip readLatency of Forward (per epoch) is: " << chipReadLatency*1e9 << "ns" << endl;
	cout << "Chip readDynamicEnergy of Forward (per epoch) is: " << chipReadDynamicEnergy*1e12 << "pJ" << endl;
	cout << "Chip readLatency of Activation Gradient (per epoch) is: " << chipReadLatencyAG*1e9 << "ns" << endl;
	cout << "Chip readDynamicEnergy of Activation Gradient (per epoch) is: " << chipReadDynamicEnergyAG*1e12 << "pJ" << endl;
	cout << "Chip readLatency of Weight Gradient (per epoch) is: " << chipReadLatencyWG*1e9 << "ns" << endl;
	cout << "Chip readDynamicEnergy of Weight Gradient (per epoch) is: " << chipReadDynamicEnergyWG*1e12 << "pJ" << endl;
	cout << "Chip writeLatency of Weight Update (per epoch) is: " << chipWriteLatencyWU*1e9 << "ns" << endl;
	cout << "Chip writeDynamicEnergy of Weight Update (per epoch) is: " << chipWriteDynamicEnergyWU*1e12 << "pJ" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Chip total Latency (per epoch) is: " << (chipReadLatency+chipReadLatencyAG+chipReadLatencyWG+chipWriteLatencyWU)*1e9 << "ns" << endl;
	cout << "Chip total Energy (per epoch) is: " << (chipReadDynamicEnergy+chipReadDynamicEnergyAG+chipReadDynamicEnergyWG+chipWriteDynamicEnergyWU)*1e12 << "pJ" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Chip PEAK readLatency of Forward (per epoch) is: " << chipReadLatencyPeakFW*1e9 << "ns" << endl;
	cout << "Chip PEAK readDynamicEnergy of Forward (per epoch) is: " << chipReadDynamicEnergyPeakFW*1e12 << "pJ" << endl;
	cout << "Chip PEAK readLatency of Activation Gradient (per epoch) is: " << chipReadLatencyPeakAG*1e9 << "ns" << endl;
	cout << "Chip PEAK readDynamicEnergy of Activation Gradient (per epoch) is: " << chipReadDynamicEnergyPeakAG*1e12 << "pJ" << endl;
	cout << "Chip PEAK readLatency of Weight Gradient (per epoch) is: " << chipReadLatencyPeakWG*1e9 << "ns" << endl;
	cout << "Chip PEAK readDynamicEnergy of Weight Gradient (per epoch) is: " << chipReadDynamicEnergyPeakWG*1e12 << "pJ" << endl;
	cout << "Chip PEAK writeLatency of Weight Update (per epoch) is: " << chipWriteLatencyPeakWU*1e9 << "ns" << endl;
	cout << "Chip PEAK writeDynamicEnergy of Weight Update (per epoch) is: " << chipWriteDynamicEnergyPeakWU*1e12 << "pJ" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Chip PEAK total Latency (per epoch) is: " << (chipReadLatencyPeakFW+chipReadLatencyPeakAG+chipReadLatencyPeakWG+chipWriteLatencyPeakWU)*1e9 << "ns" << endl;
	cout << "Chip PEAK total Energy (per epoch) is: " << (chipReadDynamicEnergyPeakFW+chipReadDynamicEnergyPeakAG+chipReadDynamicEnergyPeakWG+chipWriteDynamicEnergyPeakWU)*1e12 << "pJ" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Chip leakage Energy is: " << chipLeakageEnergy*1e12 << "pJ" << endl;
	cout << "Chip leakage Power is: " << chipLeakage*1e6 << "uW" << endl;
	cout << endl;
	cout << "************************ Breakdown of Latency and Dynamic Energy *************************" << endl;
	cout << endl;
	cout << "----------- ADC (or S/As and precharger for SRAM) readLatency is : " << chipLatencyADC*1e9 << "ns" << endl;
	cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readLatency is : " << chipLatencyAccum*1e9 << "ns" << endl;
	cout << "----------- Synaptic Array w/o ADC (Forward + Activate Gradient) readLatency is : " << chipLatencyOther*1e9 << "ns" << endl;
	cout << "----------- Buffer readLatency is: " << chipbufferLatency*1e9 << "ns" << endl;
	cout << "----------- Interconnect readLatency is: " << chipicLatency*1e9 << "ns" << endl;
	cout << "----------- Weight Gradient Calculation readLatency is : " << chipReadLatencyPeakWG*1e9 << "ns" << endl;
	cout << "----------- Weight Update writeLatency is : " << chipWriteLatencyPeakWU*1e9 << "ns" << endl;
	cout << "----------- DRAM data transfer Latency is : " << chipDRAMLatency*1e9 << "ns" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "----------- ADC (or S/As and precharger for SRAM) readDynamicEnergy is : " << chipEnergyADC*1e12 << "pJ" << endl;
	cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readDynamicEnergy is : " << chipEnergyAccum*1e12 << "pJ" << endl;
	cout << "----------- Synaptic Array w/o ADC (Forward + Activate Gradient) readDynamicEnergy is : " << chipEnergyOther*1e12 << "pJ" << endl;
	cout << "----------- Buffer readDynamicEnergy is: " << chipbufferReadDynamicEnergy*1e12 << "pJ" << endl;
	cout << "----------- Interconnect readDynamicEnergy is: " << chipicReadDynamicEnergy*1e12 << "pJ" << endl;
	cout << "----------- Weight Gradient Calculation readDynamicEnergy is : " << chipReadDynamicEnergyPeakWG*1e12 << "pJ" << endl;
	cout << "----------- Weight Update writeDynamicEnergy is : " << chipWriteDynamicEnergyPeakWU*1e12 << "pJ" << endl;
	cout << "----------- DRAM data transfer DynamicEnergy is : " << chipDRAMDynamicEnergy*1e12 << "pJ" << endl;
	cout << endl;
	cout << "************************ Breakdown of Latency and Dynamic Energy *************************" << endl;
	cout << endl;
	cout << endl;
	cout << "-----------------------------------Chip layer-by-layer Performance---------------------------------" << endl;
	cout << "Energy Efficiency TOPS/W: " << numComputation/((chipReadDynamicEnergy+chipLeakageEnergy+chipReadDynamicEnergyAG+chipReadDynamicEnergyWG+chipWriteDynamicEnergyWU)*1e12) << endl;
	cout << "Throughput TOPS: " << numComputation/(chipReadLatency+chipReadLatencyAG+chipReadLatencyWG+chipWriteLatencyWU)*1e-12 << endl;
	cout << "Throughput FPS: " << 1/(chipReadLatency+chipReadLatencyAG+chipReadLatencyWG+chipWriteLatencyWU) << endl;
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "Peak Energy Efficiency TOPS/W: " << numComputation/((chipReadDynamicEnergyPeakFW+chipReadDynamicEnergyPeakAG+chipReadDynamicEnergyPeakWG+chipWriteDynamicEnergyPeakWU)*1e12) << endl;
	cout << "Peak Throughput TOPS: " << numComputation/(chipReadLatencyPeakFW+chipReadLatencyPeakAG+chipReadLatencyPeakWG+chipWriteLatencyPeakWU)*1e-12 << endl;
	cout << "Peak Throughput FPS: " << 1/(chipReadLatencyPeakFW+chipReadLatencyPeakAG+chipReadLatencyPeakWG+chipWriteLatencyPeakWU) << endl;
	
	cout << "-------------------------------------- Hardware Performance Done --------------------------------------" <<  endl;
	cout << endl;
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::seconds>(stop-start);
    cout << "------------------------------ Simulation Performance --------------------------------" <<  endl;
	cout << "Total Run-time of NeuroSim: " << duration.count() << " seconds" << endl;
	cout << "------------------------------ Simulation Performance --------------------------------" <<  endl;
	
	// save results to top level csv file (only total results)
	ofstream outfile;
	outfile.open ("SIAM_Output.csv", ios::app);
	if (outfile.is_open()) {
		outfile << chipReadLatency << "," << chipReadLatencyAG << "," << chipReadLatencyWG << "," << chipWriteLatencyWU << ",";
		outfile << chipReadDynamicEnergy << "," << chipReadDynamicEnergyAG << "," << chipReadDynamicEnergyWG << "," << chipWriteDynamicEnergyWU << ",";
		outfile << chipReadLatencyPeakFW << "," << chipReadLatencyPeakAG << "," << chipReadLatencyPeakWG << "," << chipWriteLatencyPeakWU << ",";
		outfile << chipReadDynamicEnergyPeakFW << "," << chipReadDynamicEnergyPeakAG << "," << chipReadDynamicEnergyPeakWG << "," << chipWriteDynamicEnergyPeakWU << ",";
		outfile << numComputation/((chipReadDynamicEnergy+chipLeakageEnergy+chipReadDynamicEnergyAG+chipReadDynamicEnergyWG+chipWriteDynamicEnergyWU)*1e12) << ",";
		outfile << numComputation/(chipReadLatency+chipReadLatencyAG+chipReadLatencyWG+chipWriteLatencyWU)*1e-12 << ",";
		outfile << numComputation/((chipReadDynamicEnergyPeakFW+chipReadDynamicEnergyPeakAG+chipReadDynamicEnergyPeakWG+chipWriteDynamicEnergyPeakWU)*1e12) << ",";
		outfile << numComputation/(chipReadLatencyPeakFW+chipReadLatencyPeakAG+chipReadLatencyPeakWG+chipWriteLatencyPeakWU)*1e-12 << endl;
	} else {
		cout << "Error: the output file cannot be opened!" << endl;
	}
	outfile.close();
	
	
//nocs

		for (int i=0; i<netStructure.size(); i++) {

			cout << "-------------------- Estimation of Layer " << i+1 << " ----------------------" << endl;

			ChipCalculatePerformance(inputParameter, tech, cell, i, argv[4*i+5], argv[4*i+6], argv[4*i+7], netStructure[i][6],
						netStructure, markNM, numTileEachLayer, utilizationEachLayer, speedUpEachLayer, tileLocaEachLayer,
						numPENM, desiredPESizeNM, desiredTileSizeCM, desiredPESizeCM, CMTileheight, CMTilewidth, NMTileheight, NMTilewidth, numArrayWriteParallel,
						&layerReadLatency, &layerReadDynamicEnergy, &tileLeakage, &layerReadLatencyAG, &layerReadDynamicEnergyAG, &layerReadLatencyWG, &layerReadDynamicEnergyWG, 
						&layerWriteLatencyWU, &layerWriteDynamicEnergyWU, &layerbufferLatency, &layerbufferDynamicEnergy, &layericLatency, &layericDynamicEnergy,
						&coreLatencyADC, &coreLatencyAccum, &coreLatencyOther, &coreEnergyADC, &coreEnergyAccum, &coreEnergyOther, &layerDRAMLatency, &layerDRAMDynamicEnergy,
						&layerReadLatencyPeakFW, &layerReadDynamicEnergyPeakFW, &layerReadLatencyPeakAG, &layerReadDynamicEnergyPeakAG,
						&layerReadLatencyPeakWG, &layerReadDynamicEnergyPeakWG, &layerWriteLatencyPeakWU, &layerWriteDynamicEnergyPeakWU, &global_routinglatency, &test, &local_bufferlatency, &local_bufferenergy
					, &local_routinglatency, &local_routingenergy, &global_routingenergy, &global_bufferlatency, &global_bufferenergy, 1);
			for (int j=0; j<netStructure.size(); j++) {
				if (j != i) {
					numTileOtherLayer += numTileEachLayer[0][j] * numTileEachLayer[1][j];
				}
			}
			layerLeakageEnergy = numTileOtherLayer*layerReadLatency*tileLeakage*netStructure[i][7];

			chipReadLatency += layerReadLatency*netStructure[i][7];
			avg_layerReadLatency += layerReadLatency*netStructure[i][7];
			chipReadDynamicEnergy += layerReadDynamicEnergy*netStructure[i][7];
			chipLeakageEnergy += layerLeakageEnergy*netStructure[i][7];
			chipLeakage += tileLeakage*numTileEachLayer[0][i] * numTileEachLayer[1][i]*netStructure[i][7];
			chipbufferLatency += layerbufferLatency*netStructure[i][7];
			chipbufferReadDynamicEnergy += layerbufferDynamicEnergy*netStructure[i][7];
			chipicLatency += layericLatency*netStructure[i][7];
			chipicReadDynamicEnergy += layericDynamicEnergy*netStructure[i][7];

			global_routinglatency_1+=global_routinglatency*netStructure[i][7];
			local_routinglatency_1+=local_routinglatency*netStructure[i][7];
			global_bufferlatency_1+=global_bufferlatency*netStructure[i][7];
			local_bufferlatency_1+=local_bufferlatency*netStructure[i][7];
			coreLatencyOther_only_1+=coreLatencyOther_only*netStructure[i][7];

			global_bufferenergy_1 += global_bufferenergy*netStructure[i][7];
			coreEnergyOther_only_1 +=coreEnergyOther_only*netStructure[i][7];
			local_bufferenergy_1 +=local_bufferenergy*netStructure[i][7];
			global_routingenergy_1 +=global_routingenergy*netStructure[i][7];
			local_routingenergy_1 +=local_routingenergy*netStructure[i][7];
			chipLatencyADC += coreLatencyADC*netStructure[i][7];
			chipLatencyAccum += coreLatencyAccum*netStructure[i][7];
			//max_glob_acc_lat = MAX(max_glob_acc_lat, Global_accum_est_lat);

			//cout<<"\n The new global accum latency to support the chip max addition is: "<< Global_accum_est_lat*1e9<< "ns" <<endl;
			//cout<<"\n The new maximum global accum latency to support the chip max addition is: "<< max_glob_acc_lat*1e9<< "ns" <<endl;

			//chipLatencyAccum += max_glob_acc_lat;
			chipLatencyOther += coreLatencyOther*netStructure[i][7];
			chipEnergyADC += coreEnergyADC*netStructure[i][7];
			chipEnergyAccum += coreEnergyAccum*netStructure[i][7];
			//max_glob_acc_energy = MAX(max_glob_acc_energy, Global_accum_est_energy);
			//cout<<"\n The new global accum energy to support the chip max addition is: "<< Global_accum_est_energy*1e12<< "pJ" <<endl;
			//cout<<"\n The new maximum global accum energy to support the chip max addition is: "<< max_glob_acc_energy*1e12<< "pJ" <<endl;
			//chipEnergyAccum += max_glob_acc_energy;

			chipEnergyOther += coreEnergyOther*netStructure[i][7];
			global_iclatency += global_bufferlatency*netStructure[i][7];

			cout << "layer" << i+1 << "'s readLatency is: " << layerReadLatency*1e9 << "ns" << endl;
			cout << "layer" << i+1 << "'s readDynamicEnergy is: " << layerReadDynamicEnergy*1e12 << "pJ" << endl;
			cout << "layer" << i+1 << "'s leakagePower is: " << numTileEachLayer[0][i] * numTileEachLayer[1][i] * tileLeakage*1e6 << "uW" << endl;
			cout << "layer" << i+1 << "'s leakageEnergy is: " << layerLeakageEnergy*1e12 << "pJ" << endl;
			cout << "layer" << i+1 << "'s buffer latency is: " << layerbufferLatency*1e9 << "ns" << endl;
			cout << "layer" << i+1 << "'s buffer readDynamicEnergy is: " << layerbufferDynamicEnergy*1e12 << "pJ" << endl;
			cout << "layer" << i+1 << "'s Routing latency is: " << layericLatency*1e9 << "ns" << endl;
			cout << "layer" << i+1 << "'s Routing readDynamicEnergy is: " << layericDynamicEnergy*1e12 << "pJ" << endl;

			count+=1;
			tile_total+=test;
			cout << endl;

			cout << "************************ Breakdown of Latency and Dynamic Energy *************************" << endl;
			cout << endl;
			cout << "----------- ADC (or S/As and precharger for SRAM) readLatency is : " << coreLatencyADC*1e9 << "ns" << endl;
			cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readLatency is : " << coreLatencyAccum*1e9 << "ns" << endl;
			cout << "----------- Other Peripheries (e.g. decoders, mux, switchmatrix, buffers, IC, pooling and activation units) readLatency is : " << coreLatencyOther*1e9 << "ns" << endl;
			cout << "----------- Other Peripheries Only (e.g. decoders, mux, switchmatrix, pooling and activation units) readLatency is : " << coreLatencyOther_only*1e9 << "ns" << endl;
			cout << "----------- ADC (or S/As and precharger for SRAM) readEnergy is : " << coreEnergyADC*1e12 << "pJ" << endl;
			cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readEnergy is : " << coreEnergyAccum*1e12 << "pJ" << endl;
			cout << "----------- Other Peripheries (e.g. decoders, mux, switchmatrix, buffers, IC, pooling and activation units) readEnergy is : " << coreEnergyOther*1e12 << "pJ" << endl;
			cout << endl;
			cout << "************************ Breakdown of Latency and Dynamic Energy *************************" << endl;
			cout << endl;


		}

		//cout<<"Count is "<<count<<endl;
		//cout<<"The total layer read latency is : "<<avg_layerReadLatency<<endl;
		avg_layerReadLatency = avg_layerReadLatency/count;

		cout << "------------------------------ Summary --------------------------------" <<  endl;
		cout << endl;
		cout << "------------------------------ Area Summary --------------------------------" <<  endl;
		cout << endl;
		cout << "ChipArea : " << chipArea*1e12 << " um^2" << endl;
		myarea<<"Chip Area"<<","<<chipArea * 1e12<<","<<"um^2"<<endl;
		cout << "Total Routing Area on chip (Tile/PE local): " << chipAreaIC*1e12 << " um^2" << endl;
		myarea<<"Total Within Tile Routing Area"<<","<<chipAreaIC * 1e12<<","<<"um^2"<<endl;
		cout << "Total ADC (or S/As and precharger for SRAM) Area on chip : " << chipAreaADC*1e12 << " um^2" << endl;
		myarea<<"Total ADC (or S/As and precharger for SRAM) Area"<<","<<chipAreaADC * 1e12<<","<<"um^2"<<endl;
		cout << "Total Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) on chip : " << chipAreaAccum*1e12 << " um^2" << endl;
		myarea<<"Total Accumulation Area"<<","<<chipAreaAccum * 1e12<<","<<"um^2"<<endl;
		cout << "Other Peripheries (e.g. decoders, mux, switchmatrix and activation units) : " << chipAreaOther*1e12 << " um^2" << endl;
		myarea<<"Total Other Peripheries Area"<<","<<chipAreaOther * 1e12<<","<<"um^2"<<endl;
		//cout<<  "The total buffer area is: "<<(chipArealocalbuffer + chipAreaglobalbuffer)*1e12 << " um^2" << endl;
		cout<<  "The total buffer area within tile is: "<<(chipArealocalbuffer)*1e12 << " um^2" << endl;
		myarea<<"Total Buffer Area within the Tile"<<","<<chipArealocalbuffer * 1e12<<","<<"um^2"<<endl;
		//cout<<  "The total local buffer area is " << chipArealocalbuffer*1e12 << " um^2" << endl;
		//myarea<<"Total local Buffer Area"<<","<<chipArealocalbuffer * 1e12<<","<<"um^2"<<endl;
		//cout<<	"The total global buffer area is "<< chipAreaglobalbuffer*1e12 << " um^2" << endl;
		//cout<<	"The total global routing area is "<< chipAreaglobalIC*1e12 << " um^2" << endl;
		//cout<< 	"The total local routing area is "<< chipAreatileIC*1e12 << " um^2" << endl;
		//myarea<<"Total local Routing Area"<<","<<chipAreatileIC * 1e12<<","<<"um^2"<<endl;
		//cout<		"The total subarray (compute engine) only area is "<< chipAreasubarrayalone*1e12 << " um^2"<<endl;
		cout << "------------------------------ Area Summary --------------------------------" <<  endl;
		cout << endl;
		myarea.close();

		cout << "------------------------------ Latency Summary --------------------------------" <<  endl;
		std::ofstream mylat;
		mylat.open ("./Final_Results/Latency.csv");
		std::ofstream myenergy;
		myenergy.open ("./Final_Results/Energy.csv");
		cout << endl;
		cout<<  "The average tile latency of the chip is : "<<avg_layerReadLatency*1e9<<" ns" << endl;
		cout << "Chip total readLatency is: " << chipReadLatency*1e9 << " ns" << endl;
		mylat<<"Total readLatency"<<","<<chipReadLatency * 1e9<<","<<"ns"<<endl;
		cout << "Chip buffer readLatency is: " << chipbufferLatency*1e9 << " ns" << endl;
		mylat<<"Total Buffer Latency"<<","<<chipbufferLatency * 1e9<<","<<"ns"<<endl;
		cout << "Chip Routing readLatency is: " << chipicLatency*1e9 << " ns" << endl;
		mylat<<"Total Routing Latency"<<","<<chipicLatency * 1e9<<","<<"ns"<<endl;
		//cout << "Chip total global routing latency is : "<<global_routinglatency_1*1e9<<" ns"<<endl;
		//cout << "Chip total tile routing latency is : "<<local_routinglatency_1*1e9<<" ns"<<endl;
		//cout << "Chip total global buffer latency is : "<<global_bufferlatency_1*1e9<<" ns"<<endl;
		//cout << "Chip total local buffer latency is : "<<local_bufferlatency_1*1e9<<" ns"<<endl;
		//mylat<<"Total local buffer Latency"<<","<<local_bufferlatency_1 * 1e9<<","<<"ns"<<endl;
		//cout << "Chip total other peripheries only latency is : "<<coreLatencyOther*1e9<<" ns"<<endl;

		cout<<	endl;
		cout << "************************ Breakdown of Latency *************************" << endl;
		cout << endl;
		cout << "----------- ADC (or S/As and precharger for SRAM) readLatency is : " << chipLatencyADC*1e9 << " ns" << endl;
		mylat<<"Total ADC Latency"<<","<<chipLatencyADC * 1e9<<","<<"ns"<<endl;
		cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readLatency is : " << chipLatencyAccum*1e9 << " ns" << endl;
		mylat<<"Total Accumulation Latency"<<","<<chipLatencyAccum * 1e9<<","<<"ns"<<endl;
		cout << "----------- Other Peripheries (e.g. decoders, mux, switchmatrix, buffers, IC, pooling and activation units) readLatency is : " << chipLatencyOther*1e9 << " ns" << endl;
		mylat<<"Total Other Peripheries Latency"<<","<<chipLatencyOther * 1e9<<","<<"ns"<<endl;
		cout << "************************ Breakdown of Latency *************************" << endl;
		cout<<	endl;
		cout << "************************ Energy Summary*************************" << endl;
		cout << "Chip total readDynamicEnergy is: " << chipReadDynamicEnergy*1e12 << " pJ" << endl;
		myenergy<<"Total readEnergy"<<","<<chipReadDynamicEnergy * 1e12<<","<<"pJ"<<endl;
		cout << "Chip total leakage Energy is: " << chipLeakageEnergy*1e12 << " pJ" << endl;
		myenergy<<"Total leakage Energy"<<","<<chipLeakageEnergy * 1e12<<","<<"pJ"<<endl;
		cout << "Chip total leakage Power is: " << chipLeakage*1e6 << " uW" << endl;
		myenergy<<"Total leakage Power"<<","<<chipLeakage * 1e6<<","<<"uW"<<endl;
		cout << "Chip Routing readDynamicEnergy is: " << chipicReadDynamicEnergy*1e12 << " pJ" << endl;
		myenergy<<"Total Routing Energy"<<","<<chipicReadDynamicEnergy * 1e12<<","<<"pJ"<<endl;
		cout << "Chip Buffer readDynamicEnergy is: " << chipbufferReadDynamicEnergy*1e12 << " pJ" << endl;
		myenergy<<"Total Buffer Energy"<<","<<chipbufferReadDynamicEnergy * 1e12<<","<<"pJ"<<endl;
		//cout << "Chip total other peripheries energy is : "<<coreEnergyOther_only_1*1e12<<" pJ"<<endl;
		cout<<endl;
		//cout << "Chip Global buffer readDynamicEnergy is: " << global_bufferenergy_1*1e12 << " pJ" << endl;
		cout << "Chip Local buffer readDynamicEnergy is: " << local_bufferenergy_1*1e12 << " pJ" << endl;
		myenergy<<"Total Local Buffer Energy"<<","<<local_bufferenergy_1 * 1e12<<","<<"pJ"<<endl;
		//cout << "Chip Global Routing readDynamicEnergy is: " << global_routingenergy_1*1e12 << " pJ" << endl;
		cout << "Chip Local Routing readDynamicEnergy is: " << local_routingenergy_1*1e12 << " pJ" << endl;
		myenergy<<"Total Local Routing Energy"<<","<<local_routingenergy_1 * 1e12<<","<<"pJ"<<endl;
		cout<<endl;

		cout << endl;
		cout << "************************ Breakdown of Dynamic Energy *************************" << endl;
		cout << endl;
		//cout << "----------- Other Peripheries (e.g. decoders, mux, switchmatrix, pooling and activation units) readLatency is : " << coreLatencyOther*1e9 << " ns" << endl;
		cout << "----------- ADC (or S/As and precharger for SRAM) readEnergy is : " << chipEnergyADC*1e12 << " pJ" << endl;
		myenergy<<"Total ADC Energy"<<","<<chipEnergyADC * 1e12<<","<<"pJ"<<endl;
		cout << "----------- Accumulation Circuits (subarray level: adders, shiftAdds; PE/Tile/Global level: accumulation units) readEnergy is : " << chipEnergyAccum*1e12 << " pJ" << endl;
		myenergy<<"Total Accumulation Energy"<<","<<chipEnergyAccum * 1e12<<","<<"pJ"<<endl;
		cout << "----------- Other Peripheries (e.g. decoders, mux, switchmatrix, buffers, IC, pooling and activation units) readEnergy is : " << chipEnergyOther*1e12 << " pJ" << endl;
		myenergy<<"Total Other Peripheries Energy"<<","<<chipEnergyOther * 1e12<<","<<"pJ"<<endl;
		//cout << "----------- Other Peripheries Only (e.g. decoders, mux, switchmatrix, pooling and activation units) readEnergy is : " << coreEnergyOther_only_1*1e12 << " pJ" << endl;
		cout << endl;
		cout << "************************ Breakdown Dynamic Energy *************************" << endl;
		cout << endl;
		mylat.close();
		cout << endl;
		//cout << "----------------------------- Performance -------------------------------" << endl;
		//cout << "Energy Efficiency TOPS/W (Layer-by-Layer Process): " << numComputation/(chipReadDynamicEnergy*1e12+chipLeakageEnergy*1e12) << endl;
		//myenergy<<"Energy Efficiency TOPS/W "<<","<<numComputation/(chipReadDynamicEnergy*1e12+chipLeakageEnergy*1e12)<<endl;
		//cout << "Throughput FPS (Layer-by-Layer Process): " << 1/(chipReadLatency) << endl;
		//myenergy<<"Throughput "<<","<<1/(chipReadLatency)<<endl;
		myenergy.close();
		std::ofstream myfile_1;
		myfile_1.open ("./to_interconnect/fps.csv");
		myfile_1 << 1/(chipReadLatency) <<endl;
		myfile_1.close();
		cout << "-------------------------------------- Hardware Performance Done --------------------------------------" <<  endl;
		cout << endl;
	}
	
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::seconds>(stop-start);
  	cout << "------------------------------ Simulation Performance --------------------------------" <<  endl;
	cout << "Total Run-time of SIAM: " << duration.count() << " seconds" << endl;
	cout << "------------------------------ Simulation Performance --------------------------------" <<  endl;
	return 0;

}

vector<vector<double> > getNetStructure(const string &inputfile) {
	ifstream infile(inputfile.c_str());
	string inputline;
	string inputval;

	int ROWin=0, COLin=0;
	if (!infile.good()) {
		cerr << "Error: the input file cannot be opened!" << inputval << endl;
		exit(1);
	}else{
		while (getline(infile, inputline, '\n')) {
			ROWin++;
		}
		infile.clear();
		infile.seekg(0, ios::beg);
		if (getline(infile, inputline, '\n')) {
			istringstream iss (inputline);
			while (getline(iss, inputval, ',')) {
				COLin++;
			}
		}
	}
	infile.clear();
	infile.seekg(0, ios::beg);

	vector<vector<double> > netStructure;
	for (int row=0; row<ROWin; row++) {
		vector<double> netStructurerow;
		getline(infile, inputline, '\n');
		istringstream iss;
		iss.str(inputline);
		for (int col=0; col<COLin; col++) {
			while(getline(iss, inputval, ',')){
				istringstream fs;
				fs.str(inputval);
				double f=0;
				fs >> f;
				netStructurerow.push_back(f);
			}
		}
		netStructure.push_back(netStructurerow);
	}
	infile.close();
	//cout<<"The size of netStructure is:"<<netStructure.size();
	return netStructure;
	netStructure.clear();	
}

vector<vector<double>> slicing(vector<vector<double>>& arr, int X, int Y) 
	{ 	
		// Starting and Ending iterators 
		auto start = arr.begin() + X; 
		auto end = arr.begin() + Y + 1; 
		
		// To store the sliced vector 
		vector<vector<double>> result(Y - X + 1); 
		
		// Copy vector using copy function() 
		copy(start, end, result.begin()); 
		
		// Return the final sliced vector 
		return result; 
	}
