import csv

def floyd_warshall(adj_matrix):
    n = len(adj_matrix)
    dist = [[float('inf') if i != j else 0 for j in range(n)] for i in range(n)]

    for i in range(n):
        for j in range(n):
            if adj_matrix[i][j] != 0:
                dist[i][j] = adj_matrix[i][j]

    for k in range(n):
        for i in range(n):
            for j in range(n):
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])

    return dist


def read_adjacency_matrix_from_csv(filename):
    adjacency_matrix = []
    with open(filename, 'r') as csvfile:
        csv_reader = csv.reader(csvfile)
        for row in csv_reader:
            adjacency_matrix.append([int(cell) for cell in row])
    return adjacency_matrix



# Example usage:
if __name__ == "__main__":
    adjacency_matrix = read_adjacency_matrix_from_csv('data.csv')
    print(adjacency_matrix)
    shortest_distances = floyd_warshall(adjacency_matrix)
    # Writing the matrix to a CSV file
    with open('distance.csv', 'w', newline='') as csvfile:
        csv_writer = csv.writer(csvfile)
        for row in shortest_distances:
            csv_writer.writerow(row)
