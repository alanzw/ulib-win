/*
DIJKSTRA(G, s, w)
  for each vertex u in V             ;initialize vertex u
    d[u] := infinity 
    p[u] := u 
    color[u] := WHITE
  end for
  color[s] := GRAY 
  d[s] := 0 
  INSERT(Q, s)                       ;discover vertex s
  while (Q != ?)
    u := EXTRACT-MIN(Q)              ;examine vertex u
    S := S U { u }
    for each vertex v in Adj[u]      ;examine edge (u,v)
      if (w(u,v) + d[u] < d[v])
        d[v] := w(u,v) + d[u]        ;edge (u,v) relaxed
        p[v] := u 
        if (color[v] = WHITE) 
          color[v] := GRAY
          INSERT(Q, v)               ;discover vertex v
        else if (color[v] = GRAY)
          DECREASE-KEY(Q, v)
      else
        ...                          ;edge (u,v) not relaxed
    end for
    color[u] := BLACK                ;finish vertex u
  end while
  return (d, p)
*/

/*
 1  function Dijkstra(Graph, source):
 2      for each vertex v in Graph:           // Initializations
 3          dist[v] := infinity               // Unknown distance function from source to v
 4          previous[v] := undefined          // Previous node in optimal path from source
 5      dist[source] := 0                     // Distance from source to source
 6      Q := the set of all nodes in Graph
        // All nodes in the graph are unoptimized - thus are in Q
 7      while Q is not empty:                 // The main loop
 8          u := vertex in Q with smallest dist[]
 9          if dist[u] = infinity:
10              break                         // all remaining vertices are inaccessible from source
11          remove u from Q
12          for each neighbor v of u:         // where v has not yet been removed from Q.
13              alt := dist[u] + dist_between(u, v) 
14              if alt < dist[v]:             // Relax (u,v,a)
15                  dist[v] := alt
16                  previous[v] := u
17      return dist[]
 */

#include <iostream>
//#include <vector>
#include <climits>
using namespace std;

#include "adt/uvector.h"
 
const int SIZE = 5; //number of nodes
int graph[SIZE][SIZE];//the graph itself
int d[SIZE]; //distances from the source to each node
int pi[SIZE];//contains the predecessor node for each node
//vector<int> s;//list of nodes which were already visited
//vector<int> vs;//list of nodes which were not visited yet
typedef huys::ADT::UVector<int> UNodeList;
UNodeList s;
UNodeList vs;

//#define SHRT_MAX 99999

void sort(UNodeList* vec) {
    //sorts the vector of nodes according to
    //distances from the source to each node
    for (unsigned int i = 0; i < vec->size(); i++)
        for (unsigned int j = i; j < vec->size(); j++)
            if (d[vec->at(i)] > d[vec->at(j)]) {
                int temp = vec->at(i);
                vec->at(i) = vec->at(j);
                vec->at(j) = temp;
            }
}

void relax(UNodeList* vec, int u) {
    //updates the distances from the source to each node
    //and the predecessor for each node
    for (unsigned int i = 0; i < vec->size(); i++) {
        int vi = vec->at(i);
        if (graph[u][vi] == SHRT_MAX)
            continue;
        if (d[vi] > d[u] + graph[u][vi]) {
            d[vi] = d[u] + graph[u][vi];
            pi[vi] = u;
        }
    }
}

void printShortestPathTo(int v) {
    //this simply prints the vector of predecessors
    //for the requested node (v)
    cout << "Distance to " << v << " is " << d[v] << endl;
    cout << "Shortest path: ";
    int x = pi[v];
    cout << v << "<-";
    while (x != -1 && x != 0) {
        cout << x << "<-";
        x = pi[x];
    }
    cout << "0";
    cout << endl << endl;
}

int main() {
    //initialize all the costs in the graph
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            graph[i][j] = SHRT_MAX;

    graph[0][1] = 20;
    graph[0][4] = 40;
    graph[1][4] = 50;
    graph[2][3] = 30;
    graph[2][4] = 60;
    graph[4][2] = 20;
    graph[4][3] = 100;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << graph[i][j] << "\t";
        }
        cout << endl;
    }
    for (int i = 0; i < SIZE; i++) {
        vs.push_back(i);//initialize all the variables
        d[i] = SHRT_MAX;//all distances to infinite
        pi[i] = -1;//nodes have no predecesors
    }
    d[0] = 0; //the distance of the source is 0
    sort(&vs); //we sort the nodes according to the distance

    while (vs.size() > 0) {
        int x = vs.front();//take the node with the shortest distance
        for (unsigned int i = 0; i < vs.size() - 1; i++) {
            vs.at(i) = vs.at(i + 1);
        }
        vs.pop_back();
        s.push_back(x);//mark it as visited
        relax(&vs, x);//update the distances
        sort(&vs); //sort the nodes according to the new distances
    }

    printShortestPathTo(4);//choose any node
    printShortestPathTo(2);//choose any node
    printShortestPathTo(0);//choose any node
    printShortestPathTo(1);//choose any node
    printShortestPathTo(3);//choose any node
    cin.ignore();
    return 0;
}
