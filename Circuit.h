#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

using namespace std;

#ifndef CIRCUIT_H
#define CIRCUIT_H

#define NMAX 10000 //max number of nodes
#define DISTANCE_MAX 10000

struct Point {
    double x;
    double y;
};

typedef struct Circuit{
    int n; //number of nodes (=> (n-1) connections)
    int node[NMAX]; //nodes (node 0 is the depot)
    int next[NMAX]; //connections
    double totalLength = 0.0;
}Circuit;

//Local variables (for all)
static Circuit circuit;
static int N; // total number of points on map
static double maxX;
static double maxY;
static Point input_points[NMAX];
static bool in_circuit[NMAX];
static double dist2[NMAX + 1][NMAX + 1];
static int newest_node;


//add a node to the circuit
void addNode(int p, int next_to) {
    //neighbors management
    int temp_next = circuit.next[next_to];
    circuit.next[next_to] = p;
    circuit.next[p] = temp_next;
    circuit.totalLength += sqrt(dist2[next_to][p]);
    //circuit management
    circuit.node[circuit.n] = p;
    in_circuit[p] = true;
    newest_node = p;
    circuit.n++;
}

void initCircuit() {
    for(int i=0; i<N; i++) {
        circuit.node[i] = 0;
        circuit.next[i] = 0;
        in_circuit[i] = false;
    }
    circuit.totalLength=0;
    circuit.n=0;
    newest_node=0;
}

//read input points (to construct the nodes for the network) from 
//an input file
void readInputPoints(string filepath) {
    ifstream f;
    f.open(filepath);
    if (f.is_open()) {
        // First 2 lines are number of points, max abscissa, max coordinate
        f >> N >> maxX >> maxY;
        // Then coordinates of points
        for (int i = 0; i < N; i++) {
            f >> input_points[i].x >> input_points[i].y;
        }
        f.close();
    } else {
        throw invalid_argument("Error on reading");
    }
}

//write the connections of the network to an outputfile
void writeOut(string filepath) {
    ofstream f;
    f.open(filepath,fstream::in | fstream::out | fstream::trunc);
    if (f.is_open()) {
        int current_node = circuit.node[0];
        f << circuit.totalLength << endl;
        for (int i = 0; i < circuit.n; i++) {
            f << input_points[current_node].x << " "
                << input_points[current_node].y
                << endl;
            current_node = circuit.next[current_node];
        }
        f.close();
    } else {
        throw invalid_argument("Error on writing");
    }
}

//Caculate the distances between the nodes
void calculateDist2() {
    for (int i = 0; i <= N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            dist2[i][j] = (input_points[i].x - input_points[j].x)*(input_points[i].x - input_points[j].x)
                + (input_points[i].y - input_points[j].y)*(input_points[i].y - input_points[j].y);
            dist2[j][i] = dist2[i][j]; // the matrix is symetric
        }
    }
}

#endif //CIRCUIT_H
