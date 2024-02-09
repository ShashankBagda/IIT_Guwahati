#include <bits/stdc++.h>
using namespace std;

void printPath(const string& message, char direction, int diff) 
{
    cout << message;
    for (int i = 0; i < diff; i++) 
    {
        cout << " " << direction << " "; // Output the direction 'diff' times
    }
}

void calculatePath(int start, int end, const string& message, char direction) 
{
    if (start < end) 
    {
        printPath(message, direction, end - start);
    } 
    else 
    {
        printPath(message, direction, start - end);
    }
}

int main() 
{
    int s_x, s_y, d_x, d_y, size;
    bool choice;

    cout << "Enter 1 for numbers or 0 for coordinates: ";
    cin >> choice;

    if (choice) 
    {
        int src, dest;

        cout << "Enter the data as Source Destination Matrix_size: \n";
        cin >> src >> dest >> size;

        s_x = src / size; // Calculate source X coordinate
        s_y = src % size; // Calculate source Y coordinate
        d_x = dest / size; // Calculate destination X coordinate
        d_y = dest % size; // Calculate destination Y coordinate
    } 
    else 
    {
        cout << "Enter the coordinates as Source_X Source_Y Destination_X Destination_Y Matrix_size: \n";
        cin >> s_x >> s_y >> d_x >> d_y >> size;
    }

    if (s_x == d_x && s_y == d_y) 
    {
        cout << "Source and Destination are Same" << endl;
    } 
    else 
    {
        if (s_x == d_x) 
        {
            printPath("Source and Destination are in the same Row\nThe Path is: ", 'E', abs(d_y - s_y));
        } 
        else if (s_y == d_y) 
        {
            printPath("Source and Destination are in the same Column\nThe Path is: ", 'N', abs(d_x - s_x));
        } 
        else 
        {
            cout << "Calculate according to X_Y Routing\nThe Path is: ";
            calculatePath(s_y, d_y, "", 'E');
            calculatePath(s_x, d_x, "", 'N');
        }
    }

    cout<<endl;

    return 0;
}