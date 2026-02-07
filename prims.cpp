#include <iostream>
using namespace std;


class prims {
    int graph[10][10];
    int n;

public:
    prims() {
        cout << "Enter number of offices (vertices): ";
        cin >> n;

        // initialize graph with 999
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                graph[i][j] = 999;
            }
        }
    }

    void create() {
        int edges;
        cout << "Enter number of connections (edges): ";
        cin >> edges;

        cout << "Enter (source destination cost):\n";
        for(int i = 0; i < edges; i++){
            int u, v, w;
            cin >> u >> v >> w;

            graph[u][v] = w;
            graph[v][u] = w; // undirected graph
        }
    }

    void display() {
        cout << "\nCost Matrix:\n";
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(graph[i][j] == 999)
                    cout << "999 ";
                else
                    cout << graph[i][j] << " ";
            }
            cout << endl;
        }
    }

    void prims_algorithm(int start) {

        int nearest[20];
        int mincost = 0;

        // initialize nearest array
        nearest[start] = -1;

        for(int i = 0; i < n; i++){
            if(i != start)
                nearest[i] = start;
        }

        cout << "\nEdges in Minimum Spanning Tree:\n";

        int count = 1;

        while(count < n){

            int min = 999;
            int j = -1;

            // find minimum edge
            for(int i = 0; i < n; i++){
                if(nearest[i] != -1 && graph[nearest[i]][i] < min){
                    min = graph[nearest[i]][i];
                    j = i;
                }
            }

            if(j == -1){
                cout << "No spanning tree exists\n";
                return;
            }

            int u = nearest[j];
            int v = j;

            cout << u << " --> " << v << " = " << min << endl;

            mincost += min;
            nearest[j] = -1;

            // update nearest values
            for(int k = 0; k < n; k++){
                if(nearest[k] != -1 && graph[j][k] < graph[nearest[k]][k]){
                    nearest[k] = j;
                }
            }

            count++;
        }

        cout << "\nMinimum Total Cost = " << mincost << endl;
    }
};

int main(){

    prims p;

    p.create();
    p.display();

    int start;
    cout << "\nEnter starting vertex: ";
    cin >> start;

    p.prims_algorithm(start);

    return 0;
}
