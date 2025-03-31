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

// // Generate Graphviz visualization
// void generateGraphViz(Graph &graph, vector<string> &locations) {
//     ofstream file("graph.dot");
//     file << "digraph G {\n";
//     for (int i = 0; i < graph.V; i++) {
//         for (int j = i + 1; j < graph.V; j++) {
//             if (graph.adjMatrix[i][j] != INT_MAX) {
//                 file << "  " << locations[i] << " -> " << locations[j] << " [label=\"" << graph.adjMatrix[i][j] << " km\"];\n";
//             }
//         }
//     }
//     file << "}\n";
//     file.close();
//     system("dot -Tpng graph.dot -o graph.png");
// }

//code 2
// void generateGraphViz(Graph &graph, vector<string> &locations) {
//     ofstream file("graph.dot");
//     file << "digraph G {\n";
    
//     for (int i = 0; i < graph.V; i++) {
//         for (int j = i + 1; j < graph.V; j++) {
//             if (graph.adjMatrix[i][j] != INT_MAX) {
//                 file << "  " << locations[i] << " -> " << locations[j] 
//                      << " [label=\"" << graph.adjMatrix[i][j] << " km\", dir=both];\n";
//             }
//         }
//     }

//     file << "}\n";
//     file.close();
//     system("dot -Tpng graph.dot -o graph.png");
// }


//code3

void generateGraphViz(Graph &graph, vector<string> &locations) {
    ofstream file("graph.dot");
    file << "digraph G {\n";
    file << "  graph [splines=true];\n"; // Smooth curves for roads
    file << "  node [shape=box, style=filled, color=lightblue, fontname=\"Arial\", fontsize=14];\n"; 

    for (int i = 0; i < graph.V; i++) {
        // Different shape for warehouses (as buildings)
        if (locations[i].find("WAREHOUSE") != string::npos) {
            file << "  " << locations[i] << " [shape=house, color=orange, style=filled, fillcolor=yellow];\n";
        }
    }

    for (int i = 0; i < graph.V; i++) {
        for (int j = i + 1; j < graph.V; j++) {
            if (graph.adjMatrix[i][j] != INT_MAX) {
                file << "  " << locations[i] << " -> " << locations[j]
                     << " [label=\"" << graph.adjMatrix[i][j] << " km\", penwidth=2, color=blue, fontcolor=red, dir=both, style=dashed];\n";
            }
        }
    }

    file << "}\n";
    file.close();
    system("dot -Tpng graph.dot -o graph.png");
}


//code 4
// Generate Graphviz visualization with custom styles & icons
// void generateGraphViz(Graph &graph, vector<string> &locations) {
//     ofstream file("graph.dot");
//     file << "digraph G {\n";
//     file << "  graph [splines=polyline];\n";  // Better edge styling
//     file << "  node [shape=circle, style=filled, fillcolor=lightblue, fontname=\"Arial\", fontsize=12];\n";

//     // Define warehouse nodes with building icons
//      vector<int> warehouses ;
// warehouses.push_back(4);
// warehouses.push_back(5);
// warehouses.push_back(6);
// warehouses.push_back(7);

//      // Warehouses indices
//     for (int i : warehouses) {
//         file << "  " << locations[i] << " [shape=box, fillcolor=yellow, image=\"warehouse.png\", labelloc=b];\n";
//     }

//     // Define bike & car nodes
//     file << "  Bike [shape=plaintext, image=\"bike.png\", labelloc=b];\n";
//     file << "  Car [shape=plaintext, image=\"car.png\", labelloc=b];\n";

//     // Define roads
//     for (int i = 0; i < graph.V; i++) {
//         for (int j = i + 1; j < graph.V; j++) {
//             if (graph.adjMatrix[i][j] != INT_MAX) {
//                 file << "  " << locations[i] << " -> " << locations[j]
//                      << " [label=\"" << graph.adjMatrix[i][j] << " km\", color=black, penwidth=2, dir=\"both\"];\n";
//             }
//         }
//     }

//     file << "}\n";
//     file.close();

//     // Generate the graph with icons
//     system("dot -Tpng graph.dot -o graph.png");
// }




//printing paths 

int main() {


  cout << GREEN << BOLD << BLINK;
    cout << "Welcome DELIVERY-EASE" << RESET << endl;

    
    int V = 8;
    Graph graph(V);
   vector<string> locations;
locations.push_back("JIIT");
locations.push_back("BOTANICALGARDEN");
locations.push_back("GREATERNOIDA");
locations.push_back("SECTO104");
locations.push_back("WAREHOUSE1");
locations.push_back("WAREHOUSE2");
locations.push_back("WAREHOUSE3");
locations.push_back("WAREHOUSE4");

    
    // Adding routes
   
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

    //  graph.floydWarshall();
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
   


// adding vechile chosse and speed and time 
cout << "Select mode of transport:\n1. Bike (40 km/hr)\n2. Car (50 km/hr)\n";
   
    
    int vehicle;
    cout<< "Enter your choice: ";
    cin>>vehicle;
    
   
    float speed ;

    
    float fuel;
    
    
     
   
     
    if(vehicle == 1)
    {
        float cost=60;
         float avg=60;
        speed=40;
        float time = distances[dest] / speed;
        fuel=distances[dest]/avg;
        cost=cost*fuel;
        cout<<CYAN << "\nDistance from " << locations[src] << " to " << locations[dest] << " is: " << distances[dest] << " km:\n"<<"By a"<<"Bike  "<<"speed is"<<":"<<RED<<speed<<"km.hr\n";
         cout <<CYAN<< "Estimated travel time: " <<RED<< time << " hours\n";
          cout <<CYAN<< "Estimated fuel: " <<RED<< fuel << " litre"<<endl;
          cout <<CYAN<< "total cost: " <<RED<< cost << " rupees"<<endl;
    
    
    }
    else if(vehicle == 2) {
       float cost=60;
        speed=50;
         float avg=20;
         
        float time = distances[dest] / speed;
          fuel=distances[dest]/avg;
             cost=cost*fuel;
         cout<<CYAN << "\nDistance from " << locations[src] << " to " << locations[dest] << " is: " <<  distances[dest] << " km:\n"<<"By a "<<" Car "<<"speed is"<<":"<<RED<<speed<<"km.hr\n";
           cout<<CYAN << "Estimated travel time: "<<RED << time << " hours"<<endl;
           cout <<CYAN<< "Estimated fuel: " <<RED<< fuel << " litre"<<endl;
            cout <<CYAN<< "total cost: " <<RED<< cost << " rupees"<<endl;
    }

    else{
         float avg1=60;
         float speed1=40;
         float cost1=40;
        float time1 = distances[dest] / speed1;
       float fuel1=distances[dest]/avg1;
          cost1=cost1*fuel1;
        cout<<CYAN << "\nDistance from " << locations[src] << " to " << locations[dest] << " is: " << distances[dest] << " km:\n"<<"By a"<<"Bike  "<<"speed is"<<":"<<RED<<speed1<<"km.hr\n";
         cout <<CYAN<< "Estimated travel time: " <<RED<< time1 << " hours\n";
          cout <<CYAN<< "Estimated fuel: " <<RED<< fuel1 << " litre"<<endl;
          cout <<CYAN<< "total cost: " <<RED<< cost1 << " rupees"<<endl;


          cout<<CYAN<<"******************************************************************"<<endl;
    
         float speed2=50;
         float avg2=20;
         float cost2=20;
         
        float time2= distances[dest] / speed2;
          float fuel2=distances[dest]/avg2;
             cost2=cost2*fuel2;
         cout<<CYAN << "\nDistance from " << locations[src] << " to " << locations[dest] << " is: " <<  distances[dest] << " km:\n"<<"By a "<<" Car "<<"speed is"<<":"<<RED<<speed2<<"km.hr\n";
           cout<<CYAN << "Estimated travel time: "<<RED << time2 << " hours"<<endl;
           cout <<CYAN<< "Estimated fuel: " <<RED<< fuel2 << " litre"<<endl;
            cout <<CYAN<< "total cost: " <<RED<< cost2 << " rupees"<<endl;

             cout<<CYAN<<"******************************************************************"<<endl;

             if(cost1<cost2)
             {
                cout<<RED<<"use bike this is efficient "<<endl;
             }

             else{
                cout<<RED<<"use car this is efficient "<<endl;
             }




    }
   
    
    // cout<<CYAN << "\nDistance from " << locations[src] << " to " << locations[dest] << " is: " << distances[dest] << " km\n";
    
    // Generate Graph Visualization
    generateGraphViz(graph, locations);
    cout<<CYAN << "\nGraph visualization saved as graph.png!"<<endl;

    //choose to go further or not 
    cout<<RED<<"please enter -1 if you want to exit"<<endl;
     cin>>c;
    }









    
    
    return 0;
  






}
