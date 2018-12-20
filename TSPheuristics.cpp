//Adrien Wartelle
#include <iostream>
#include <fstream>
#include <string>
#include "MersenneTwister.h"
#include "Circuit.h"
#include <algorithm>
#include <chrono>

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
    addNode(0,0); // add a initial node
    for(int i=0; i<N; i++) { 
        distance_to_subtour[i] = DISTANCE_MAX;
    }
    for(int i=1; i<N; i++) { // ith node to add
        min_distance=DISTANCE_MAX;
        min_cost = DISTANCE_MAX;
    // Looking for the node to add to the subtour
        for(int j=0; j<N; j++) { 
            if(in_circuit[j] == false) {// for every node not in subtour
                if( dist2[j][circuit.node[circuit.n]] < distance_to_subtour[j] ) {
                    distance_to_subtour[j] =  dist2[j][circuit.node[circuit.n]];
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
            //distance cost for new node to calculate
            double temp_distance_cost = 
                    dist2[temp_node][best_node_to_add] 
                    + dist2[best_node_to_add][circuit.next[temp_node]]
                    - dist2[temp_node][circuit.next[temp_node]];
            //recalculate distances
            if(temp_distance_cost < min_cost) {
                min_cost = temp_distance_cost;
                add_next_to = temp_node; 
            }
        }
        // Adding a node
        addNode(best_node_to_add, add_next_to);
        //~ cout << "adding " << best_node_to_add 
            //~ << " next to " << add_next_to << endl;
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
    addNode(0,0); // add a initial node
    for(int i=0; i<N; i++) { 
        distance_to_subtour[i] = DISTANCE_MAX;
    }
    for(int i=1; i<N; i++) { // ith node to add
        max_distance = 0;
        min_cost = DISTANCE_MAX;
    // Looking for the node to add to the subtour
        for(int j=0; j<N; j++) { 
            if(in_circuit[j] == false) {// for every node not in subtour
                if( dist2[j][circuit.node[circuit.n]] < distance_to_subtour[j] ) {
                    distance_to_subtour[j] =  dist2[j][circuit.node[circuit.n]];
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
            //distance cost for new node to calculate
            double temp_distance_cost = 
                    dist2[temp_node][best_node_to_add] 
                    + dist2[best_node_to_add][circuit.next[temp_node]]
                    - dist2[temp_node][circuit.next[temp_node]];
            //recalculate distances
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
    double distance_cost[NMAX][NMAX]; // cost to a node next to another node
    int best_node_to_add=1;
    int add_next_to=0;
    addNode(0,0); // add a initial node
    for(int i=0; i<N; i++) { 
        for(int j=0; j<N; j++) { 
            distance_cost[i][j] = DISTANCE_MAX;
        }
    }
    for(int i=1; i<N; i++) { // ith node to add
        min_cost = DISTANCE_MAX+1;
        //Update the distance costs
        for(int j=0; j<N; j++) { 
            if(in_circuit[j] == false) {// for every node not in subtour
                
                //distance cost for new node to calculate
                distance_cost[j][circuit.node[circuit.n]] = 
                        dist2[j][circuit.node[circuit.n]] + dist2[j][circuit.next[circuit.node[circuit.n]]]
                        - dist2[circuit.node[circuit.n]][circuit.next[circuit.node[circuit.n]]];
                //and to recalculate for its precedent node 
                //because its next node (neighbour) has changed
                distance_cost[j][circuit.before[circuit.node[circuit.n]]] = 
                        dist2[j][circuit.before[circuit.node[circuit.n]]] + dist2[j][circuit.node[circuit.n]]
                        - dist2[circuit.before[circuit.node[circuit.n]]][circuit.node[circuit.n]];
            }
        }
        // Find best insertion
        for(int j=0; j<N; j++) { 
            if(in_circuit[j] == false) {
                for(int k=0; k<circuit.n; k++) {
                    if(distance_cost[j][circuit.node[k]] < min_cost) {
                        min_cost = distance_cost[j][circuit.node[k]];
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


high_resolution_clock::time_point now() {
    return high_resolution_clock::now();
}

int main() {
    high_resolution_clock::time_point start, finish;
    duration<double, std::milli> timespan;
    double exec_times[3][NB_TEST];
    double circuit_length[3][NB_TEST];
    //~ double time_classement_counts[3][3];
    //~ double length_classement_counts[3][3];
    //~ double mean_circuit_length[3];
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
    PPI();
    writeOut("./output/output_MI_test.txt");
    
    for(int i=0; i<3; i++) {
        for(int j=0; j<NB_TEST; j++) {
            initCircuit();
            generateInputPoints(NB_POINTS,MAX_X,MAX_Y);
            calculateDist2();
            cout << "GENERATION" << endl;
            for(int l=0; l<NB_POINTS; l++) {
                cout << input_points[l].x << " " << input_points[l].y  << endl;
            }
            if(j==0) {
                start = now();
                PPI();
                finish = now();
            } else if (j==1) {
                start = now();
                PLI();
                finish = now();                    
            } else {
                start = now();
                MI();
                finish = now();    
            }
            timespan = finish - start;
            exec_times[i][j] = timespan.count();
            circuit_length[i][j] = getCircuitLength();
        }
    }
    
    file_stats.open("./output/statistics.txt",fstream::in | fstream::out | fstream::trunc);
    if (file_stats.is_open()) {
        for(int i=0; i<3; i++) {
            for(int j=0; j<NB_TEST; j++) {
                cout << i << " " << j << " " << exec_times[i][j] << " "
                    << circuit_length[i][j]
                    << endl;
                file_stats << i << " " << j << " " << exec_times[i][j] << " "
                    << circuit_length[i][j]
                    << endl;
            }
        }
        file_stats.close();
    } else {
        throw invalid_argument("Error on writing statistics");
    }
    //~ for (int i = 0; i < NB_TEST; i++) {
        //~ for(int algo=1; algo<=3; algo++) {
            //~ filename_in = "./input/input_" + to_string(i) + ".txt";
            //~ readInputPoints(filename_in);
            //~ initCircuit();
            //~ calculateDist2();
            //~ if(algo==1) {
                //~ PPI();
                //~ filename_out = "./output/output_PPI_" + to_string(i) + ".txt";
            //~ } else if(algo==2) {
                //~ PLI();
                //~ filename_out = "./output/output_PLI_" + to_string(i) + ".txt";
            //~ } else {
                //~ MI();
                //~ filename_out = "./output/output_MI_" + to_string(i) + ".txt";
            //~ }
            //~ writeOut(filename_out);
        //~ }
        
    //~ }
    return 0;
}
