//Adrien Wartelle and Seddik Habiballah
#include <iostream>
#include <fstream>
#include <string>
#include "MersenneTwister.h"
#include "Circuit.h"
#include <algorithm>
#include <chrono>

//Macros
#define NB_TEST 100
#define NB_POINTS 50
#define MAX_X 10
#define MAX_Y 10
#define MAX_TIME 1000000

using namespace std;
using namespace std::chrono;


//Nearest Insertion heuristic algorithm (PPI : "Plus Proche Insertion")
void PPI()
{
    double min_cost; // cost to add a new node in the current subtour
    double distance_to_subtour[NMAX];
    double min_distance=DISTANCE_MAX; // to the graph
    int best_node_to_add=1;
    int add_next_to=0;
    addNode(0,0); // add an initial node
    for(int i=0; i<N; i++) { //init distances
        distance_to_subtour[i] = DISTANCE_MAX;
    }
    for(int i=1; i<N; i++) { // ith node to add
        min_distance = DISTANCE_MAX;
        min_cost = DISTANCE_MAX;
    // Looking for the node to add to the subtour
        for(int j=1; j<N; j++) { 
            if(in_circuit[j] == false) {// for every node not in subtour
                int last_node = circuit.n-1;
                //update distance to current subtour with last point added
                if(dist2[j][circuit.node[last_node]] < distance_to_subtour[j] ) {
                    distance_to_subtour[j] =  dist2[j][circuit.node[last_node]];
                }
                //find the best distance
                if(distance_to_subtour[j] < min_distance) {
                    min_distance = distance_to_subtour[j];
                    best_node_to_add = j;
                }
            }
        }
        for(int j=0; j<circuit.n; j++) {  //looking for the best insertion
            int temp_node = circuit.node[j];
            //distance cost to add best_node next to temp_node
            double temp_distance_cost = 
                    dist2[temp_node][best_node_to_add] 
                    + dist2[best_node_to_add][circuit.next[temp_node]]
                    - dist2[temp_node][circuit.next[temp_node]];
            //find minimal cost
            if(temp_distance_cost < min_cost) {
                min_cost = temp_distance_cost;
                add_next_to = temp_node; 
            }
        }
        // Adding a node
        addNode(best_node_to_add, add_next_to);
    }
}

//Farthest Insertion heuristic algorithm (PLI : "Plus Lointaine Insertion")
void PLI()
{
    double min_cost; // cost to add a new node in the current subtour
    double distance_to_subtour[NMAX];
    double max_distance=0; // to the graph
    int best_node_to_add=1;
    int add_next_to=0;
    addNode(0,0); // add an initial node
    for(int i=0; i<N; i++) { //init distances
        distance_to_subtour[i] = DISTANCE_MAX;
    }
    for(int i=1; i<N; i++) { // ith node to add
        max_distance = 0;
        min_cost = DISTANCE_MAX;
    // Looking for the node to add to the subtour
        for(int j=1; j<N; j++) { 
            if(in_circuit[j] == false) {// for every node not in subtour
                int last_node = circuit.n - 1;
                //update distance to current subtour with last point added
                if( dist2[j][circuit.node[last_node]] < distance_to_subtour[j] ) {
                    distance_to_subtour[j] =  dist2[j][circuit.node[last_node]];
                }
                //find the best distance
                if(distance_to_subtour[j] > max_distance) {
                    max_distance = distance_to_subtour[j];
                    best_node_to_add = j;
                }
            }
        }
        for(int j=0; j<circuit.n; j++) {  //looking for the best insertion
            int temp_node = circuit.node[j];
            //distance cost to add best_node next to temp_node
            double temp_distance_cost = 
                    dist2[temp_node][best_node_to_add] 
                    + dist2[best_node_to_add][circuit.next[temp_node]]
                    - dist2[temp_node][circuit.next[temp_node]];
            //find minimal cost
            if(temp_distance_cost < min_cost) {
                min_cost = temp_distance_cost;
                add_next_to = temp_node; 
            }
        }
        // Adding a node
        addNode(best_node_to_add, add_next_to);
    }
}

//Best Insertion heuristic algorithm (MI : "Meilleure Insertion")
void MI()
{
    double min_cost; // cost to add a new node in the current subtour
    double cost[NMAX][NMAX]; // cost to a node next to another node
    int best_node_to_add=1;
    int add_next_to=0;
    addNode(0,0); // add an initial node
    for(int i=0; i<N; i++) { //init costs
        for(int j=0; j<N; j++) { 
            cost[i][j] = DISTANCE_MAX;
        }
    }
    for(int i=1; i<N; i++) { // ith node to add
        min_cost = DISTANCE_MAX+1;
        //Update the distance costs
        for(int j=1; j<N; j++) { 
            if(in_circuit[j] == false) {// for every node not in subtour
                int last_node = circuit.n - 1;
                //distance cost to add next to last node added to calculate
                cost[j][circuit.node[last_node]] = 
                        dist2[j][circuit.node[last_node]] + dist2[j][circuit.next[circuit.node[last_node]]]
                        - dist2[circuit.node[last_node]][circuit.next[circuit.node[last_node]]];
                //distance cost to add next to predecessor of last node to recalculate
                cost[j][circuit.before[circuit.node[last_node]]] = 
                        dist2[j][circuit.before[circuit.node[last_node]]] + dist2[j][circuit.node[last_node]]
                        - dist2[circuit.before[circuit.node[last_node]]][circuit.node[last_node]];
            }
        }
        // Find best insertion
        for(int j=0; j<N; j++) { 
            if(in_circuit[j] == false) {
                for(int k=0; k<circuit.n; k++) {
                    if(cost[j][circuit.node[k]] < min_cost) {
                        min_cost = cost[j][circuit.node[k]];
                        best_node_to_add = j;
                        add_next_to = circuit.node[k];
                    }
                }
            }
        }
        // Adding a node
        addNode(best_node_to_add, add_next_to);
    }
}


//Return current time
high_resolution_clock::time_point now() {
    return high_resolution_clock::now();
}

int main() {
    //Declaration of variables for statistics
    high_resolution_clock::time_point start, finish;
    duration<double, std::milli> timespan;
    double exec_times[3][NB_TEST];  //in millis
    double circuit_length[3][NB_TEST];
    ofstream file_stats;
    
    //Graphic test file
    readInputPoints("./input/input_test.txt");
    calculateDist2();
    initCircuit();
    PPI();
    writeOut("./output/output_PPI_test.txt");
    initCircuit();
    PLI();
    writeOut("./output/output_PLI_test.txt");
    initCircuit();
    MI();
    writeOut("./output/output_MI_test.txt");
    
    //Statistics test
    for(int i=0; i<3; i++) { //for PPI, PLI and MI
        for(int j=0; j<NB_TEST; j++) { //for NB_TEST tests
            initCircuit(); //initialize the circuit defined in "Circuit.h"
            //random generation of NB_POINTS input points to use for the TSP
            //(Travelling Salesman problem)
            generateInputPoints(NB_POINTS,MAX_X,MAX_Y); 
            calculateDist2(); //Calculate the distance matrix
            if(j==0) { //PPI
                start = now();
                PPI();
                finish = now();
            } else if (j==1) { //PLI
                start = now();
                PLI();
                finish = now();                    
            } else { //MI
                start = now();
                MI();
                finish = now();    
            }
            //Statistics recording
            timespan = finish - start;
            exec_times[i][j] = timespan.count(); //in millis
            circuit_length[i][j] = getCircuitLength();
        }
    }
    
    //Writing out statistics
    file_stats.open("./output/statistics.csv",fstream::in | fstream::out | fstream::trunc);
    if (file_stats.is_open()) {
        //~ cout << "n ExecPPI LengthPPI ExecPLI LengthPLI  ExecMI LengthMI" << endl;
        file_stats << "n ExecPPI LengthPPI ExecPLI LengthPLI  ExecMI LengthMI" << endl;
        for(int j=0; j<NB_TEST; j++) {
            //~ cout << j << " ";
            file_stats << j << " ";
            for(int i=0; i<3; i++) {
                //Console output
                //~ cout << exec_times[i][j] << " " << circuit_length[i][j] << " ";
                //File output
                file_stats << exec_times[i][j] << " " << circuit_length[i][j] << " ";
            }
            //~ cout << endl;
            file_stats << endl;
        }
        file_stats.close();
    } else {
        throw invalid_argument("Error on writing statistics");
    }
    cout << "Execution finished : results in ./output" << endl;
    return 0;
}
