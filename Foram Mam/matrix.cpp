#include <bits/stdc++.h>

using namespace std;

// Function to generate random weights for each node in the grid
void generateWeights(vector<vector<int>>& grid, int gridSize) 
{
    // Seed for random number generation
    srand(time(0));
    
    for (int i = 0; i < gridSize; ++i) 
    {
        for (int j = 0; j < gridSize; ++j) 
        {
            // Assign random weights (between 1 and 10) to each node
            grid[i][j] = rand() % 8; // Adjust range of weights as needed
        }
    }
}

// Function to perform DyXY routing considering node weights
void DyXYRouting(int sourceX, int sourceY, int destX, int destY, int gridSize, vector<vector<int>>& weights) 
{
    // Displaying the grid with weights
    cout << "Grid with Node Weights:\n\n";
    for (int i = 0; i < gridSize; ++i) 
    {
        for (int j = 0; j < gridSize; ++j) 
        {
            cout << weights[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;

    // Routing logic based on node weights
    cout << "Routing from (" << sourceX << ", " << sourceY << ") to (" << destX << ", " << destY << "):\n\n";

    while (sourceX != destX || sourceY != destY) 
    {
        if (sourceX < destX) 
        {
            if (weights[sourceX + 1][sourceY] <= weights[sourceX][sourceY + 1]) 
            {
                cout << "North ";
                ++sourceX;
            } 
            else 
            {
                cout << "East ";
                ++sourceY;
            }
        } 
        else if (sourceX > destX) 
        {
            if (weights[sourceX - 1][sourceY] <= weights[sourceX][sourceY - 1]) 
            {
                cout << "South ";
                --sourceX;
            } 
            else 
            {
                cout << "West ";
                --sourceY;
            }
        } 
        else 
        {
            if (sourceY < destY) 
            {
                cout << "East ";
                ++sourceY;
            } 
            else if (sourceY > destY) 
            {
                cout << "West ";
                --sourceY;
            }
        }
    }

    cout << endl << "\nDestination reached at (" << destX << ", " << destY << ")\n";
}

int main() {
    int src_x, src_y, dest_x, dest_y, size;
    bool choice;

    cout << "Enter 1 for numbers or 0 for coordinates: ";
    cin >> choice;
    cout << endl;

    if (choice) {
        int src, dest;

        cout << "Enter the data as Source Destination Matrix_size: ";
        cin >> src >> dest >> size;

        src_x = src / size; // Calculate source X coordinate
        src_y = src % size; // Calculate source Y coordinate
        dest_x = dest / size; // Calculate destination X coordinate
        dest_y = dest % size; // Calculate destination Y coordinate
    } else {
        cout << "Enter the coordinates as Source_X Source_Y Destination_X Destination_Y Matrix_size: ";
        cin >> src_x >> src_y >> dest_x >> dest_y >> size;
    }

    cout << endl;

    // Create a grid for weights and generate random weights for nodes
    vector<vector<int>> weights(size, vector<int>(size, 0));
    generateWeights(weights, size);

    DyXYRouting(src_x, src_y, dest_x, dest_y, size, weights);

    cout << endl;

    return 0;
}
