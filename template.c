/**
 * Group ID - 
 * Member 1 Name - Pranav Deepak Tanna
 * Member 1 BITS ID - 2021A7PS2685P
 * Member 2 Name - Nachiket Kandari
 * Member 2 BITS ID - 2021A7PS2691P
 * Member 3 Name - Priyansh Patel
 * Member 3 BITS ID - 2021A7PS2682P
*/

// ---------------------------DO NOT MODIFY (Begin)------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct Graph {
    int n; // Size of the graph
    int** adj; // Adjacency matrix
    char** station_names; // Array of station names
} Graph;

/**
 * This function has been pre-filled. It reads the input testcase 
 * and creates the Graph structure from it.
*/
Graph* create_graph(char input_file_path[]) {
    FILE* f = fopen(input_file_path, "r");
    Graph* g = (Graph*)malloc(sizeof(Graph));
    // Size of graph
    fscanf(f, "%d", &(g->n));
    // Names of stations
    g->station_names = (char**)malloc(g->n * sizeof(char*));
    for(int i = 0; i < g->n; i++) {
        g->station_names[i] = (char*)malloc(100 * sizeof(char));
        fscanf(f, "%s", g->station_names[i]);
    }
    // Adjacency matrix
    g->adj = (int**)malloc(g->n * sizeof(int*));
    for(int i = 0; i < g->n; i++) {
        g->adj[i] = calloc(g->n, sizeof(int));
    }
    int edges;
    fscanf(f, "%d", &edges);
    for(int i = 0; i < edges; i++) {
        int x, y;
        fscanf(f, "%d%d", &x, &y);
        g->adj[x][y] = 1;
        g->adj[y][x] = 1;
    }
    fclose(f);
    return g;
}
// ---------------------------DO NOT MODIFY (End)------------------------------

/**
 * Q.1 
 * Return the number of junctions.
*/
int find_junctions(Graph* g) {
    int junctions = 0;

    for (int i = 0; i < g->n; i++) {
        int connections = 0;

        // Count the number of direct connections for each station
        for (int j = 0; j < g->n; j++) {
            if (g->adj[i][j] == 1) {
                connections++;
            }
        }

        // Check if the station is a junction
        if (connections >= 4) {
            junctions++;
        }
    }

    return junctions;
}

/**
 * Q.2
 * Return true if the tour specified in the question is possible for this 
 * graph, else return false.
 * Parameter SAME_STATION - If it is true, then question 2(a) must be solved. 
 * If false, then question 2(b) must be solved.
*/
bool sheldons_tour(Graph* g, bool SAME_STATION) {
    if (SAME_STATION) {
        // For an Eulerian circuit, every vertex should have even degree
        for (int i = 0; i < g->n; i++) {
            int degree = 0;
            for (int j = 0; j < g->n; j++) {
                if (g->adj[i][j] == 1) {
                    degree++;
                }
            }
            if (degree % 2 != 0) {
                return false;
            }
        }
        return true;
    } else {
        // For an Eulerian path, exactly two vertices should have odd degree
        int odd_degree_count = 0;
        for (int i = 0; i < g->n; i++) {
            int degree = 0;
            for (int j = 0; j < g->n; j++) {
                if (g->adj[i][j] == 1) {
                    degree++;
                }
            }
            if (degree % 2 != 0) {
                odd_degree_count++;
            }
        }
        return odd_degree_count == 2;
    }
}

/**
 * Q.3
 * Return closure, an n * n matrix filled with 0s and 1s as required. 
*/
int** warshall(Graph* g) {
    // Do not modify or delete pre-filled code!
    int** closure = (int**)malloc(g->n * sizeof(int*));
    for(int i = 0; i < g->n; i++) {
        closure[i] = calloc(g->n, sizeof(int));
    }
    
    // Code goes here
    // Initialize closure matrix with the adjacency matrix
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            closure[i][j] = g->adj[i][j];
        }
    }

    // Warshall's Algorithm
    for (int k = 0; k < g->n; k++) {
        for (int i = 0; i < g->n; i++) {
            for (int j = 0; j < g->n; j++) {
                closure[i][j] = closure[i][j] || (closure[i][k] && closure[k][j]);
            }
        }
    }
    
    return closure; // Do not modify
}

/**
 * Q.3
 * Return the number of impossible pairs.
*/
int find_impossible_pairs(Graph* g) {
    int** closure = warshall(g); // Do not modify

     int impossible_pairs = 0;

    // Count the pairs with no connection in the closure matrix
    for (int i = 0; i < g->n; i++) {
        for (int j = i + 1; j < g->n; j++) {
            // Check if there is no connection in both directions
            if (closure[i][j] == 0 && closure[j][i] == 0) {
                impossible_pairs++;
            }
        }
    }

    // Free the memory allocated for the closure matrix
    for (int i = 0; i < g->n; i++) {
        free(closure[i]);
    }
    free(closure);

    return impossible_pairs;
    
}

/**
 * Q.4
 * Return the number of vital train tracks.
*/

/**
 * Helper function to perform Depth-First Search (DFS) to check connectivity.
 */
void dfs(int** adj, int n, bool* visited, int start) {
    visited[start] = true;
    for (int i = 0; i < n; i++) {
        if (adj[start][i] && !visited[i]) {
            dfs(adj, n, visited, i);
        }
    }
}

/**
 * Helper function to count the number of connected components in the graph.
 */
int count_connected_components(Graph* g) {
    int components = 0;
    bool* visited = calloc(g->n, sizeof(bool));

    for (int i = 0; i < g->n; i++) {
        if (!visited[i]) {
            dfs(g->adj, g->n, visited, i);
            components++;
        }
    }

    free(visited);
    return components;
}


int find_vital_train_tracks(Graph* g) {
    int initial_components = count_connected_components(g);

    int vital_tracks = 0;

    for (int i = 0; i < g->n; i++) {
        for (int j = i + 1; j < g->n; j++) {
            if (g->adj[i][j] == 1) {
                // Temporarily remove the edge
                g->adj[i][j] = 0;
                g->adj[j][i] = 0;

                // Check the number of connected components
                int current_components = count_connected_components(g);

                // Count the vital edge if the number of components increases
                if (current_components > initial_components) {
                    vital_tracks++;
                }

                // Restore the removed edge
                g->adj[i][j] = 1;
                g->adj[j][i] = 1;
            }
        }
    }

    return vital_tracks;
}

/**
 * Q.5
 * Return upgrades, an array of size n.
 * upgrades[i] should be 0 if railway station i gets a restaurant
 * upgrades[i] should be 1 if railway station i gets a maintenance depot
 * If it is not possible to upgrade, then return "-1" in the entire array
*/

// Helper function to check if it is possible to color the graph
bool is_colorable(Graph* g, int v, int color, int* colors) {
    for (int i = 0; i < g->n; i++) {
        if (g->adj[v][i] && colors[i] == color) {
            return false;
        }
    }
    return true;
}

int* upgrade_railway_stations(Graph* g) {
    int* upgrades = calloc(g->n, sizeof(int)); // Do not modify
    
    // Code goes here
    int* colors = (int*)malloc(g->n * sizeof(int));

    // Initialize colors to -1 (not colored)
    for (int i = 0; i < g->n; i++) {
        colors[i] = -1;
    }

    // Color the first station with a restaurant (color 0)
    colors[0] = 0;

    // Assign colors to the remaining stations
    for (int i = 1; i < g->n; i++) {
        for (int c = 0; c <= 1; c++) {
            if (is_colorable(g, i, c, colors)) {
                colors[i] = c;
                break;
            }
        }
        // If a color cannot be assigned, set all elements of upgrades to -1
        if (colors[i] == -1) {
            for (int j = 0; j < g->n; j++) {
                upgrades[j] = -1;
            }
            free(upgrades);
            free(colors);
            return upgrades;
        }
    }

    // Assign upgrades based on colors
    for (int i = 0; i < g->n; i++) {
        upgrades[i] = colors[i];
    }

    free(colors);

    return upgrades; // Do not modify
}

/**
 * Q.6
 * Return distance between X and Y
 * city_x is the index of X, city_y is the index of Y
*/
int distance(Graph* g, int city_x, int city_y) {
    // Check if the given indices are valid
    if (city_x < 0 || city_x >= g->n || city_y < 0 || city_y >= g->n) {
        return -1; // Invalid indices
    }

    // Use Breadth-First Search to find the shortest path between city_x and city_y
    int* visited = (int*)calloc(g->n, sizeof(int));
    int* distance = (int*)malloc(g->n * sizeof(int));
    
    for (int i = 0; i < g->n; i++) {
        distance[i] = 2147483647; // Initialize distances to infinity
    }

    // Initialize BFS queue
    int* queue = (int*)malloc(g->n * sizeof(int));
    int front = 0, rear = 0;

    // Start from city_x
    queue[rear++] = city_x;
    visited[city_x] = 1;
    distance[city_x] = 0;

    while (front < rear) {
        int current_city = queue[front++];
        
        for (int i = 0; i < g->n; i++) {
            if (g->adj[current_city][i] == 1 && !visited[i]) {
                queue[rear++] = i;
                visited[i] = 1;
                distance[i] = distance[current_city] + 1;

                if (i == city_y) {
                    // Found the destination city, return the distance
                    free(visited);
                    free(queue);
                    free(distance);
                    return distance[i];
                }
            }
        }
    }

    // No path found between city_x and city_y
    free(visited);
    free(queue);
    free(distance);
    return -1;
}

/**
 * Q.7
 * Return the index of any one possible railway capital in the network
*/
int railway_capital(Graph* g) {
    // Using Floyd-Warshall algorithm to compute all-pairs shortest paths
    for (int k = 0; k < g->n; k++) {
        for (int i = 0; i < g->n; i++) {
            for (int j = 0; j < g->n; j++) {
                if (g->adj[i][k] && g->adj[k][j]) {
                    if (!g->adj[i][j] || g->adj[i][j] > g->adj[i][k] + g->adj[k][j]) {
                        g->adj[i][j] = g->adj[i][k] + g->adj[k][j];
                    }
                }
            }
        }
    }

    int min_sum = 2147483647; // Initialize with a large value
    int railway_capital_index = -1;

    // Iterate through each city and calculate the sum of distances
    for (int i = 0; i < g->n; i++) {
        int sum = 0;
        for (int j = 0; j < g->n; j++) {
            if (i != j) {
                sum += g->adj[i][j];
            }
        }
        // Update the railway capital if the sum is smaller
        if (sum < min_sum) {
            min_sum = sum;
            railway_capital_index = i;
        }
    }

    return railway_capital_index;
}

/**
 * Helper function for Q.8
*/
bool maharaja_express_tour(Graph* g, int source, int current_city, int previous_city, int* visited) {
    visited[current_city] = 1;

    for (int i = 0; i < g->n; i++) {
        if (g->adj[current_city][i] == 1) {
            // Check if the adjacent city is not visited and is not the previous city
            if (!visited[i] && i != previous_city) {
                // Recursively explore the next city
                if (maharaja_express_tour(g, source, i, current_city, visited)) {
                    return true;
                }
            }
            // If the adjacent city is visited and is not the source city (indicating a cycle)
            else if (visited[i] && i == source && i != previous_city) {
                return true;
            }
        }
    }

    visited[current_city] = 0; // Backtrack
    return false;
}

/**
 * Q.8 
 * Return true if it is possible to have a Maharaja Express tour 
 * starting from source city, else return false. 
*/
bool maharaja_express(Graph* g, int source) {
    int* visited = (int*)malloc(g->n * sizeof(int)); 
    for(int i = 0; i < g->n; i++) {
        visited[i] = 0;
    }
    // Hint: Call the helper function and pass the visited array created here.
    return maharaja_express_tour(g, source, source, -1, visited);
}

int main() {
    char input_file_path[100] = "/home/postman/disco/testcase_1.txt"; // Can be modified
    Graph* g = create_graph(input_file_path); // Do not modify
    
    // Code to find and print the number of junctions
    int junctions = find_junctions(g);
    printf("Number of junctions: %d\n", junctions);

    // Test Sheldon's tour for the same station (Q.2a)
    bool sheldons_tour_same_station = sheldons_tour(g, true);
    printf("Sheldon's tour (ending in same city as start)= %s\n", sheldons_tour_same_station ? "POSSIBLE" : "IMPOSSIBLE");

    // Test Sheldon's tour for a different station (Q.2b)
    bool sheldons_tour_diff_station = sheldons_tour(g, false);
    printf("Sheldon's tour (ending in different city as start)= %s\n", sheldons_tour_diff_station ? "POSSIBLE" : "IMPOSSIBLE");

    // Test Warshall's Algorithm
    int** closure = warshall(g);

    // Print the closure matrix
    printf("Transitive Closure Matrix:\n");
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            printf("%d ", closure[i][j]);
        }
        printf("\n");
    }

    // Test find_impossible_pairs
    int impossible_pairs = find_impossible_pairs(g);
    printf("Number of impossible pairs: %d\n", impossible_pairs);

    // Test find_vital_train_tracks
    int vital_tracks = find_vital_train_tracks(g);
    printf("Number of vital train tracks: %d\n", vital_tracks);

    // Upgrade Railway Stations
    int* upgrades = upgrade_railway_stations(g);

    // Print the Railway Station Upgrades
    printf("\nRailway Station Upgrades:\n");
    int notPossible = 0; // Flag to check if upgrading is not possible

    for (int i = 0; i < g->n; i++) {
        if (upgrades[i] == -1) {
            notPossible = 1;
            break; // Break the loop as soon as we find a station that cannot be upgraded
        }
    }

    if (notPossible) {
        printf("Not possible to upgrade all stations.\n");
    } else {
        for (int i = 0; i < g->n; i++) {
            printf("%s = %s\n", g->station_names[i], upgrades[i] == 0 ? "Restaurant" : "Maintenance depot");
        }
    }

    // Test distance between two cities (Q.6)
    int city_x = 0; // Replace with the index of the source city
    int city_y = 1; // Replace with the index of the destination city
    int dist = distance(g, city_x, city_y);

    if (dist != -1) {
        printf("Distance between %s and %s is %d\n", g->station_names[city_x], g->station_names[city_y], dist);
    } else {
        printf("No path between %s and %s\n", g->station_names[city_x], g->station_names[city_y]);
    }

    int capital_index = railway_capital(g);
    printf("Railway Capital= %s\n", g->station_names[capital_index]);

    // Test Maharaja Express tour
    int maharaja_source = 5; // Replace with the index of the source city for Maharaja Express tour
    bool maharaja_possible = maharaja_express(g, maharaja_source);
    printf("Maharaja Express tour starting from %s = %s\n", g->station_names[maharaja_source], maharaja_possible ? "POSSIBLE" : "IMPOSSIBLE");

    // Free the memory allocated for the closure matrix
    for (int i = 0; i < g->n; i++) {
        free(closure[i]);
    }
    free(closure);

    // Free the adjacency matrix
    for (int i = 0; i < g->n; i++) {
        free(g->adj[i]);
    }
    free(g->adj);

    // Free the array of station names
    for (int i = 0; i < g->n; i++) {
        free(g->station_names[i]);
    }
    free(g->station_names);

    // Free the graph structure
    free(g);

    return 0;
}
