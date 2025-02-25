#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
#include <fstream>
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
    vector<vector<int> >adjMatrix;

    Graph(int vertices) {
        V = vertices;
        adjMatrix.resize(V, vector<int>(V, INT_MAX));
        for (int i = 0; i < V; i++) adjMatrix[i][i] = 0;
    }

    void addEdge(int u, int v, int weight) {
        adjMatrix[u][v] = weight;
        adjMatrix[v][u] = weight; // Undirected Graph
    }

    void floydWarshall() {
        vector<vector<int> > dist = adjMatrix;
        for (int k = 0; k < V; k++) {
            for (int i = 0; i < V; i++) {
                for (int j = 0; j < V; j++) {
                    if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX)
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
        adjMatrix = dist;
    }
};

// Dijkstra's Algorithm for shortest path
vector<int> dijkstra(Graph &graph, int src) {
    vector<int> dist(graph.V, INT_MAX);
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    dist[src] = 0;
    // pq.push({0, src});
    pq.push(make_pair(0, src)); 


    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        for (int v = 0; v < graph.V; v++) {
            if (graph.adjMatrix[u][v] != INT_MAX && dist[u] + graph.adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + graph.adjMatrix[u][v];
           
                pq.push(make_pair(dist[v], v));  // ✅ Correct

            }
        }
    }
    return dist;
}

// Generate Graphviz visualization
void generateGraphViz(Graph &graph, vector<string> &locations) {
    ofstream file("graph.dot");
    file << "digraph G {\n";
    for (int i = 0; i < graph.V; i++) {
        for (int j = i + 1; j < graph.V; j++) {
            if (graph.adjMatrix[i][j] != INT_MAX) {
                file << "  " << locations[i] << " -> " << locations[j] << " [label=\"" << graph.adjMatrix[i][j] << " km\"];\n";
            }
        }
    }
    file << "}\n";
    file.close();
    system("dot -Tpng graph.dot -o graph.png");
}

int main() {


  cout << GREEN << BOLD << BLINK;
    cout << "Welcome DELIVERY-EASE" << RESET << endl;

    
    int V = 5;
    Graph graph(V);
   vector<string> locations;
locations.push_back("JIIT");
locations.push_back("BOTANICALGARDEN");
locations.push_back("GREATERNOIDA");
locations.push_back("SECTO104");
locations.push_back("WAREHOUSE");

    
    // Adding routes
    graph.addEdge(0, 1, 6);
    graph.addEdge(1, 2, 5);
    graph.addEdge(2, 3, 4);
    graph.addEdge(3, 4, 5);
    graph.addEdge(2, 3, 10);
    
int k;
 cin>>k;
    // Run Floyd-Warshall Algorithm
    
    if(k==1)
    {
            cout<<RED<<"if you want uegent direct delivery ";
            graph.floydWarshall();

            }
    else{

   
    dijkstra(graph,0);
    }
    int c;
    while (c!=-1)
    {
        /* code */
   
    
    // User selects source location
    cout << "Select your current location:\n";
    for (int i = 0; i < V; i++) {
        cout<<RED << i + 1 << ". " << locations[i] << "\n";
    }
    int src;
    cout<<YELLOW << "Enter the number corresponding to your current location: ";
    cin >> src;
    src--;
    
    if (src < 0 || src >= V) {
        cout << "Invalid choice!\n";
        return 0;
    }
    
    // User selects destination
    cout<<RED << "Select your destination:\n";
    for (int i = 0; i < V; i++) {
        cout << i + 1 << ". " << locations[i] << "\n";
    }
    int dest;
    cout<<CYAN << "Enter the number corresponding to your destination: ";
    cin >> dest;
    dest--;
    
    if (dest < 0 || dest >= V) {
        cout << "Invalid choice!\n";
        return 0;
    }
    
    vector<int> distances = dijkstra(graph, src);
    
    cout<<CYAN << "\nDistance from " << locations[src] << " to " << locations[dest] << " is: " << distances[dest] << " km\n";
    
    // Generate Graph Visualization
    generateGraphViz(graph, locations);
    cout<<CYAN << "\nGraph visualization saved as graph.png!\n";

    //choose to go further or not 
    
     cin>>c;
    }
    return 0;
}
