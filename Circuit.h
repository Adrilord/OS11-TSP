#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "MersenneTwister.h"

using namespace std;

#ifndef CIRCUIT_H
#define CIRCUIT_H

#define SEED 2018 //seed used
#define NMAX 1000 //max number of nodes
#define DISTANCE_MAX 10000

static MTRand generator(SEED); // Mersenne Twister PRNG

struct Point {
    double x;
    double y;
};

typedef struct Circuit{
    int n; //number of nodes (=> (n-1) connections)
    int node[NMAX]; //nodes (node 0 is the depot)
    int index_in_node[NMAX]; //index of a node
    int next[NMAX]; //connections
    int before[NMAX]; //connections
}Circuit;

//Local variables (for all)
static Circuit circuit;
static int N; // total number of points on map
static double maxX;
static double maxY;
static Point input_points[NMAX];
static bool in_circuit[NMAX];
static double dist2[NMAX + 1][NMAX + 1];


//add a node to the circuit
void addNode(int p, int next_to) {
    //neighbors management
    int temp_next = circuit.next[next_to];
    circuit.next[next_to] = p;
    circuit.before[p] = next_to;
    circuit.next[p] = temp_next;
    circuit.before[temp_next] = p;
    //circuit management
    circuit.node[circuit.n] = p;
    circuit.index_in_node[p] = circuit.n;
    in_circuit[p] = true;
    circuit.n++;
}

void initCircuit() {
    for(int i=0; i<N; i++) {
        circuit.node[i] = 0;
        circuit.next[i] = 0;
        circuit.before[i] = 0;
        in_circuit[i] = false;
    }
    circuit.n=0;
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

//generate a list of points
//using a Pseudo Random Number Generator (PRNG) from Mersenne Twister
void generateInputPoints(int n, double maxX, double maxY) {
    N=n;
    for (int i = 0; i < N; i++) {
        input_points[i].x = generator.randDblExc(maxX);
        input_points[i].y = generator.randDblExc(maxY);
    }
}

//calculate the current circuit length
double getCircuitLength() {
    double length=0;
    int node=circuit.node[0];
    for (int i=0; i<circuit.n; i++) {
        length+=dist2[node][circuit.next[node]];
        node = circuit.next[node];
        cout << dist2[node][circuit.next[node]] << endl;
    }
    return length;
}

#endif //CIRCUIT_H
