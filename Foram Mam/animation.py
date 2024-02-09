# Final code with matrix csv reading, convert into numpy, distance, threshold and animation
import csv
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import re

def matrix_degree(matrix):
    n = len(matrix)
    dist = [[float('inf')] * n for _ in range(n)]

    # Initialize distances between adjacent vertices
    for i in range(n):
        for j in range(n):
            if matrix[i][j] != 0:
                dist[i][j] = matrix[i][j]

    # Calculate shortest path between all pairs
    for k in range(n):
        for i in range(n):
            for j in range(n):
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])

    # Set diagonal values to 0
    for i in range(n):
        dist[i][i] = 0

    return dist


# Open the CSV file and read the data into a list of lists
with open('data.csv', 'r') as file:
    csv_reader = csv.reader(file)
    matrix_data = [list(map(int, row)) for row in csv_reader]

# Convert the list of lists to a numpy array
matrix = np.array(matrix_data)

dist_matrix = matrix_degree(matrix)
print(dist_matrix)

matrix2 = np.array(dist_matrix)

# Create a graph from the adjacency matrix
G = nx.from_numpy_array(matrix2)

# Define the animation function
# Open the file
with open('numbers.txt', 'r') as file:
    
    # Read the contents of the file
    contents = file.read()

    # Use regular expressions to extract the first two integers
    numbers = re.findall(r'\d+', contents)
    source = int(numbers[0])
    dest = int(numbers[1])
# Print the source and destination nodes
print("Source node:", source)
print("Destination node:", dest)
path = nx.shortest_path(G, source=source, target=dest, weight='weight')
path_edges = [(path[i], path[i+1]) for i in range(len(path)-1)]
node_colors = ['lightblue' if node in path else 'white' for node in G.nodes()]


# Define the animation function
def animate(i, path):
    plt.clf()
    sub_matrix = matrix2[:i+1, :i+1]
    sub_G = nx.from_numpy_array(sub_matrix)
    pos = nx.spring_layout(sub_G, seed=42)

    # Highlight the path
    if i == len(matrix2) - 1:
        path_edges = [(path[j], path[j+1]) for j in range(len(path)-1)]
        nx.draw_networkx_edges(
            sub_G, pos, edgelist=path_edges, edge_color='r', width=3)

    nx.draw(sub_G, pos, with_labels=True)
    plt.title('Animation of Distance Matrix')
    time.sleep(0.5)


# Calculate the shortest path
shortest_path = nx.shortest_path(G, source=source, target=dest)

# Create the animation object
fig, ax = plt.subplots()
ani = animation.FuncAnimation(
    fig, animate, frames=len(matrix2), fargs=(shortest_path,), repeat=False)

# Show the animation
plt.show()

