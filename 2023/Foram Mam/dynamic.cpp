#include <bits/stdc++.h>

using namespace std;

void DyXYRouting(int sourceX, int sourceY, int destX, int destY, int gridSize) 
{
    std::vector<std::vector<int>> grid(gridSize, std::vector<int>(gridSize, 0));

    // Generating the grid starting from bottom-left
    int nodeNumber = 0; // Starting from 0
    for (int i = gridSize - 1; i >= 0; --i) 
    {
        for (int j = 0; j < gridSize; ++j) 
        {
            grid[i][j] = nodeNumber++;
        }
    }

    // Displaying the grid
    std::cout << "Grid:\n\n";
    for (int i = 0; i < gridSize; ++i) 
    {
        for (int j = 0; j < gridSize; ++j) 
        {
            std::cout << grid[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    // Routing logic
    std::cout << "Routing from (" << sourceX << ", " << sourceY << ") to (" << destX << ", " << destY << "):\n\n";

    while (sourceX != destX || sourceY != destY) 
    {
        if (sourceX < destX) 
        {
            std::cout << "North ";
            ++sourceX;
        } 
        else if (sourceX > destX) 
        {
            std::cout << "South ";
            --sourceX;
        }

        if (sourceY < destY) 
        {
            std::cout << "East ";
            ++sourceY;
        } 
        else if (sourceY > destY) 
        {
            std::cout << "West ";
            --sourceY;
        }
    }
    
    std::cout << std::endl;
    
    std::cout << "\n" << "Destination reached at (" << destX << ", " << destY << ")\n";
}

int main() 
{
    int src_x, src_y, dest_x, dest_y, size;
    bool choice;

    std::cout << "Enter 1 for numbers or 0 for coordinates: ";
    std::cin >> choice;
    std::cout << std::endl;

    if (choice) 
    {
        int src, dest;

        std::cout << "Enter the data as Source Destination Matrix_size: ";
        std::cin >> src >> dest >> size;

        src_x = src / size; // Calculate source X coordinate
        src_y = src % size; // Calculate source Y coordinate
        dest_x = dest / size; // Calculate destination X coordinate
        dest_y = dest % size; // Calculate destination Y coordinate
    } 
    else 
    {
        std::cout << "Enter the coordinates as Source_X Source_Y Destination_X Destination_Y Matrix_size: ";
        std::cin >> src_x >> src_y >> dest_x >> dest_y >> size;
    }

    std::cout << std::endl;

    DyXYRouting(src_x, src_y, dest_x, dest_y, size);

    std::cout << std::endl;
    
    return 0;
}
