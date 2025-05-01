#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
#include <fstream>
#include <map>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"
#define BLINK   "\033[5m"

using namespace std;


// Graph representation using adjacency matrix
class Graph {
public:
    int V;
    vector<vector<int> >adj;

    Graph(int vertices) {
        V = vertices;
        adj.resize(V, vector<int>(V, INT_MAX));
        for (int i = 0; i < V; i++) adj[i][i] = 0;
    }

    void addEdge(int u, int v, int weight) {
        adj[u][v] = weight;
        adj[v][u] = weight; // Undirected Graph
    }

    void floydWarshall() {
        vector<vector<int> > dist = adj;
        for (int k = 0; k < V; k++) {
            for (int i = 0; i < V; i++) {
                for (int j = 0; j < V; j++) {
                    if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX)
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
        adj = dist;
    }
};


// // Dijkstra's Algorithm for shortest path
// vector<int> dijkstra(Graph &graph, int src) {
//     vector<int> dist(graph.V, INT_MAX);
//     priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
//     dist[src] = 0;
//     // pq.push({0, src});
//     pq.push(make_pair(0, src)); 


//     while (!pq.empty()) {
//         int u = pq.top().second;
//         pq.pop();
//         for (int v = 0; v < graph.V; v++) {
//             if (graph.adjMatrix[u][v] != INT_MAX && dist[u] + graph.adjMatrix[u][v] < dist[v]) {
//                 dist[v] = dist[u] + graph.adjMatrix[u][v];
           
//                 pq.push(make_pair(dist[v], v));  // ✅ Correct

//             }
//         }
//     }
//     return dist;
// }


pair<vector<int>, vector<int> > dijkstra(Graph& graph, int src) {
    int V = graph.V;
    vector<int> dist(V, INT_MAX);
    vector<int> prev(V, -1);
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;

    dist[src] = 0;
    pq.push(make_pair(0, src));

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        // Iterate over all neighbors (vertices) of u
        for (int v = 0; v < V; ++v) {
            int weight = graph.adj[u][v]; // This is the direct weight of the edge from u to v

            // Skip the node if there's no direct edge (i.e., weight is INT_MAX)
            if (weight != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push(make_pair(dist[v], v)); // Add the updated distance and vertex to the priority queue
            }
        }
    }
    return make_pair(dist, prev); // Return the distance and previous nodes arrays
}

vector<int> reconstructPath(int src, int dest, vector<int>& prev) {
    vector<int> path;
    for (int at = dest; at != -1; at = prev[at])
        path.push_back(at);
    reverse(path.begin(), path.end());
    return (path[0] == src) ? path : vector<int>();  // return empty if no path
}



void generateGraphViz(Graph &graph, vector<string> &locations) {
    ofstream file("graph.dot");
    file << "digraph G {\n";
    file << "  graph [splines=true];\n"; // Smooth curves for roads
    file << "  node [shape=box, style=filled, color=GREY, fontname=\"Arial\", fontsize=14];\n"; 

    for (int i = 0; i < graph.V; i++) {
        // Different shape for warehouses (as buildings)
        if (locations[i].find("WAREHOUSE") != string::npos) {
            file << "  " << locations[i] << " [shape=house, color=orange, style=filled, fillcolor=yellow];\n";
        }
    }
        // file << "  " << locations[0] << " [shape=house, color=RED, style=filled, fillcolor=y];\n";

    for (int i = 0; i < graph.V; i++) {
        for (int j = i + 1; j < graph.V; j++) {
            if (graph.adj[i][j] != INT_MAX) {
                file << "  " << locations[i] << " -> " << locations[j]
                     << " [label=\"" << graph.adj[i][j] << " km\", penwidth=2, color=blue, fontcolor=red, dir=both, style=dashed];\n";
            }
        }
    }

    file << "}\n";
    file.close();
    system("dot -Tpng graph.dot -o graph.png");
}



void logDelivery(const string& source, const string& destination, const string& vehicleType,
                 float distance, float time, float cost) {
    ofstream logfile("delivery_logs.csv", ios::app); // Append mode
    if (!logfile) {
        cerr << "Error opening delivery_logs.csv for logging!" << endl;
        return;
    }

    // If file is empty, add header
    static bool isFirstCall = true;
    if (logfile.tellp() == 0 && isFirstCall) {
        logfile << "Source,Destination,Vehicle,Distance(km),Time(hr),Cost(Rs)\n";
        isFirstCall = false;
    }

    logfile << source << "," << destination << "," << vehicleType << ","
            << distance << "," << time << "," << cost << "\n";
    logfile.close();
}

// multiple delivery 
vector<int> tspNearestNeighbor(vector<vector<int> >& graph, vector<int>& points) {
    int n = points.size();
    vector<bool> visited(n, false);
    vector<int> route;
    int current = 0;
    visited[0] = true;
    route.push_back(points[current]);

    for (int i = 1; i < n; ++i) {
        int next = -1, minDist = INT_MAX;
        for (int j = 0; j < n; ++j) {
            if (!visited[j] && graph[points[current]][points[j]] < minDist) {
                minDist = graph[points[current]][points[j]];
                next = j;
            }
        }
        if (next != -1) {
            visited[next] = true;
            current = next;
            route.push_back(points[current]);
        }
    }
    return route;
}







    int main() {
    cout << GREEN << BOLD << BLINK;
    cout << "Welcome DELIVERY-EASE" << RESET << endl;

    int V = 8;
    Graph graph(V);
   vector<string> locations;
locations.push_back("COMPANY");

locations.push_back("WAREHOUSE1");
locations.push_back("WAREHOUSE2");
locations.push_back("WAREHOUSE3");

locations.push_back("JIIT");
locations.push_back("BOTANICALGARDEN");
locations.push_back("GREATERNOIDA");
locations.push_back("SECTO104");

    // Add edges (routes)
    graph.addEdge(0, 1, 60);
    graph.addEdge(1, 2, 50);
    graph.addEdge(2, 3, 40);
    graph.addEdge(5, 4, 50);
    graph.addEdge(2, 3, 10);
    graph.addEdge(2, 4, 100);
    graph.addEdge(2, 6, 60);
    graph.addEdge(2, 7, 90);
    graph.addEdge(6, 7, 10);
    graph.addEdge(3, 5, 10);
    graph.addEdge(2, 5, 10);
    cout<<CYAN<<"ENTER 1 IF YOU WANT PRIORITY DELIVERY"<<endl;

     cout<<CYAN<<"ENTER 2 IF YOU WANT MULTIPLE DELIVERY DELIVERY"<<endl;
    

    int k;
    cin >> k;
     
    if (k == 1) {
        cout << RED << "if you want urgent direct delivery ";
        graph.floydWarshall();
    }
    
    
     else {
        dijkstra(graph, 0); // optional pre-run
    }

    int c = 0;
    while (c != -1) {
        cout << "Select your current location:\n";
        for (int i = 0; i < V; i++) {
            cout << RED << i + 1 << ". " << locations[i] << "\n";
        }
        int src;
        cout << YELLOW << "Enter the number corresponding to your current location: ";
        cin >> src;
        src--;

        if (src < 0 || src >= V) {
            cout << "Invalid choice!\n";
            return 0;
        }

        cout << RED << "Select your destination:\n";
        for (int i = 0; i < V; i++) {
            cout << i + 1 << ". " << locations[i] << "\n";
        }
        int dest;
        cout << CYAN << "Enter the number corresponding to your destination: ";
        cin >> dest;
        dest--;

        if (dest < 0 || dest >= V) {
            cout << "Invalid choice!\n";
            return 0;
        }

        // Run Dijkstra from src
      pair<vector<int>, vector<int> > result  = dijkstra(graph, src);
        vector<int> distances = result.first;
        vector<int> prev = result.second;

        // Reconstruct path
        vector<int> path = reconstructPath(src, dest, prev);
        
        cout << "Select mode of transport:\n1. BIKE (40 km/hr)\n2. TRUCK (20 km/hr)\n";
        int vehicle;
        cout << "Enter your choice: ";
        cin >> vehicle;

        float speed, fuel, cost, time;
        float distance = distances[dest];
        int  capacity;
    cout<<RED<<"ENTER CAPACITY"<<endl;
    cin>>capacity;
      cout<<RED<<" CAPACITY:"<<capacity<<CYAN<<"kg"<<endl;

        if (vehicle == 1 && capacity<5) { // Bike
            speed = 40;
            float avg = 60;
            cost = 60;
            time = distance / speed;
            fuel = distance / avg;
            cost *= fuel;

            cout << CYAN << "\nDistance from " << locations[src] << " to " << locations[dest] << " is: "
                 << distance << " km:\nBy a Bike, speed is: " << RED << speed << " km/hr\n";
            cout << CYAN << "Estimated travel time: " << RED << time << " hours\n";
            cout << CYAN << "Estimated fuel: " << RED << fuel << " litre\n";
            cout << CYAN << "Total cost: " << RED << cost << " rupees\n";
             cout << CYAN << "\nShortest path: ";
        for (int i = 0; i < path.size(); i++) {
            cout << locations[path[i]];
            if (i != path.size() - 1) cout << " -> ";
        }
        cout << endl;
        logDelivery(locations[src], locations[dest], "Bike", distance, time, cost);


        } else if (vehicle == 2 && capacity>5) { // Car
            speed = 20;
            float avg = 20;
            cost = 60;
            time = distance / speed;
            fuel = distance / avg;
            cost *= fuel;

            cout << CYAN << "\nDistance from " << locations[src] << " to " << locations[dest] << " is: "
                 << distance << " km:\nBy a TRUCK, speed is: " << RED << speed << " km/hr\n";
            cout << CYAN << "Estimated travel time: " << RED << time << " hours\n";
            cout << CYAN << "Estimated fuel: " << RED << fuel << " litre\n";
            cout << CYAN << "Total cost: " << RED << cost << " rupees\n";
             cout << CYAN << "\nShortest path: ";
        for (int i = 0; i < path.size(); i++) {
            cout << locations[path[i]];
            if (i != path.size() - 1) cout << " -> ";
        }
        cout << endl;
        logDelivery(locations[src], locations[dest], "Truck", distance, time, cost);


        } else { // Compare both
            float avg1 = 60, speed1 = 40, cost1 = 40;
            float time1 = distance / speed1;
            float fuel1 = distance / avg1;
            cost1 *= fuel1;

            float avg2 = 20, speed2 = 20, cost2 = 20;
            float time2 = distance / speed2;
            float fuel2 = distance / avg2;
            cost2 *= fuel2;

            cout << CYAN << "\nBy Bike:\n";
            cout << CYAN << "Time: " << RED << time1 << " hrs, Fuel: " << RED << fuel1
                 << " l, Cost: " << RED << cost1 << " rupees\n";

            cout << CYAN << "******************************************************************\n";

            cout << CYAN << "By Car:\n";
            cout << CYAN << "Time: " << RED << time2 << " hrs, Fuel: " << RED << fuel2
                 << " l, Cost: " << RED << cost2 << " rupees\n";

            cout << CYAN << "******************************************************************\n";
                 for (int i = 0; i < path.size(); i++) {
            cout << locations[path[i]];
            if (i != path.size() - 1) cout << " -> ";
        }
        cout << endl;

            if (cost1 < cost2) {
                cout << RED << "Use Bike – it is more efficient.\n";
            } else {
                cout << RED << "Use Car – it is more efficient.\n";
            }
            logDelivery(locations[src], locations[dest], "Bike vs Truck", distance, min(time1, time2), min(cost1, cost2));

        }

        generateGraphViz(graph, locations);
        cout << CYAN << "\nGraph visualization saved as graph.png!\n";

        cout << RED << "Please enter -1 if you want to exit, or any other number to continue: ";
        cin >> c;
    }

    return 0;
}



    