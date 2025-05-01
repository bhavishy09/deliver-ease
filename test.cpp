#include <iostream>
#include <vector>
#include <climits>
#include <queue>
using namespace std;

class Graph {
public:
    int V;
    vector<vector<int> > adjMatrix;

    Graph(int vertices) {
        V = vertices;
        adjMatrix.resize(V, vector<int>(V, INT_MAX));
        for (int i = 0; i < V; i++)
            adjMatrix[i][i] = 0;
    }

    void addEdge(int u, int v, int weight) {
        adjMatrix[u][v] = weight;
        adjMatrix[v][u] = weight;
    }

    vector<int> dijkstra(int src) {
        vector<int> dist(V, INT_MAX);
        dist[src] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
         pq.push(make_pair(0, src)); 


        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (int v = 0; v < V; v++) {
                if (adjMatrix[u][v] != INT_MAX && dist[u] + adjMatrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + adjMatrix[u][v];
                       pq.push(make_pair(dist[v], v)); 

                }
            }
        }

        return dist;
    }
};

int main() {
    Graph g(10);
    vector<string> locations ;
   locations.push_back("COMPANY");

locations.push_back("WAREHOUSE1");
locations.push_back("WAREHOUSE2");
locations.push_back("WAREHOUSE3");

locations.push_back("JIIT");
locations.push_back("BOTANICALGARDEN");
locations.push_back("GREATERNOIDA");
locations.push_back("SECTO104");
    // Add sample connections
    g.addEdge(0, 1, 10); g.addEdge(0, 2, 20);
    g.addEdge(1, 3, 15); g.addEdge(2, 4, 30);
    g.addEdge(3, 5, 25); g.addEdge(4, 6, 35);
    g.addEdge(5, 7, 20); g.addEdge(6, 8, 10);
    g.addEdge(7, 9, 15); g.addEdge(8, 9, 5);

    cout << "Available Locations:\n";
    for (int i = 0; i < 10; i++) {
        cout << i << " - " << locations[i] << endl;
    }

    cout << "\nEnter location numbers in order (at least 2). Enter 0 as first location to exit.\n";

    vector<int> route;
    int input;
    while (true) {
        cout << "Enter location number (0-9): ";
        cin >> input;
        if (input == 0 && route.empty()) {
            cout << "Exiting program...\n";
            return 0;
        }
        if (input < 0 || input >= 10) {
            cout << "Invalid location number! Try again.\n";
            continue;
        }
        route.push_back(input);
        char more;
        cout << "Do you want to enter more locations? (y/n): ";
        cin >> more;
        if (more == 'n' || more == 'N') break;
    }

    if (route.size() < 2) {
        cout << "You must enter at least 2 locations.\n";
        return 0;
    }

    int totalDistance = 0;
    cout << "\n--- Shortest Path Summary ---\n";
    for (int i = 0; i < route.size() - 1; i++) {
        int from = route[i];
        int to = route[i + 1];
        vector<int> dist = g.dijkstra(from);
        cout << locations[from] << " → " << locations[to] << " = " << dist[to] << " km\n";
        totalDistance += dist[to];
    }

    cout << "Total Distance Travelled: " << totalDistance << " km\n";
    return 0;
}
