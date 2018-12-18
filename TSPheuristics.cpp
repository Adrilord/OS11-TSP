//Adrien Wartelle
#include <iostream>
#include <fstream>
#include <string>
#include "MersenneTwister.h"
#include "Circuit.h"

#define SEED 2018 //seed used
#define NB_TEST 100
#define NB_POINTS 50
#define MAX_X 10
#define MAX_Y 10

using namespace std;

static MTRand generator; // Mersenne Twister PRNG

//generate a list of points and register it in a input file
//using a Pseudo Random Number Generator (PRNG) from Mersenne Twister
void gen(int n, double maxX, double maxY, string filename) {
    ofstream f;
    f.open(filename);
    f << n << endl;
    f << maxX << " " << maxY << endl;
    double x, y;
    for (int i = 0; i < n; i++) {
        x = generator.randDblExc(maxX);
        y = generator.randDblExc(maxY);
        f << x << " " << y << endl;
    }
    f.close();
}

//Nearest Insertion heuristic algorithm ("Plus Proche Insertion")
void PPI()
{
    double min_cost; // cost to add nodes in the current subtour
    double distance_to_subtour[NMAX];
    double min_distance=DISTANCE_MAX; // to the graph
    int best_node_to_add=1;
    int add_next_to=0;
    addNode(0,0); // add a initial node
        for(int j=0; j<N; j++) { 
        distance_to_subtour[j] = DISTANCE_MAX;
    }
    for(int i=1; i<N; i++) { // ith node to add
        min_distance=DISTANCE_MAX;
        min_cost = DISTANCE_MAX;
    // Looking for the node to add to the subtour
        for(int j=0; j<N; j++) { 
            if(in_circuit[j] == false) {// for every node not in subtour
                if( dist2[j][newest_node] < distance_to_subtour[j] ) {
                    distance_to_subtour[j] =  dist2[j][newest_node];
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

//Farthest Insertion heuristic algorithm ("Plus Lointaine Insertion")
void PLI()
{
    double min_cost; // cost to add nodes in the current subtour
    double distance_to_subtour[NMAX];
    double max_distance=0; // to the graph
    int best_node_to_add=1;
    int add_next_to=0;
    addNode(0,0); // add a initial node
        for(int j=0; j<N; j++) { 
        distance_to_subtour[j] = DISTANCE_MAX;
    }
    for(int i=1; i<N; i++) { // ith node to add
        max_distance = 0;
        min_cost = DISTANCE_MAX;
    // Looking for the node to add to the subtour
        for(int j=0; j<N; j++) { 
            if(in_circuit[j] == false) {// for every node not in subtour
                if( dist2[j][newest_node] < distance_to_subtour[j] ) {
                    distance_to_subtour[j] =  dist2[j][newest_node];
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
        //~ cout << "adding " << best_node_to_add 
            //~ << " next to " << add_next_to << endl;
    }
}

//~ void PLI(Graph& g, double* d)
//~ {
    //~ int n=g.n;
    //~ // cost to add nodes in the current subtour, next to a node
    //~ double distance_cost[n][n]; // just calculate the min each node
    //~ double distance_to_subtour[n];
    //~ int farest_node_in_subtour[n];
    //~ double max_distance=0; // to the graph
    //~ int best_node_to_add=1;
    //~ int add_next_to=0;
    //~ int last_node_added=0;
    //~ int prec_last_node_added=0;
    //~ bool in_subtour[n];
    //~ in_subtour[0] = true;
    //~ g.next[0] = 0;
    //~ for(int i=1; i<n; i++) { //init (case 0 not needed)
        //~ distance_to_subtour[i] = d[i*n+0]; // (node 0 is the ony one in subtour)
        //~ farest_node_in_subtour[i]=0;
        //~ in_subtour[i] = false;
    //~ }
    //~ for(int i=1; i<n; i++) { // ith node to add
        //~ max_distance=0;
    //~ // Looking for the node to add to the subtour
        //~ for(int j=0; j<n; j++) { 
            //~ if(in_subtour[j] == false) {// for every node not in subtour
                //~ //distance cost for new node to calculate
                //~ distance_cost[j][last_node_added] = 
                        //~ d[j*n+last_node_added] + d[j*n+g.next[last_node_added]]
                        //~ - d[last_node_added*n+g.next[last_node_added]];
                //~ //and to recalculate for its precedent node 
                //~ //because its next node (neighbour) has changed
                //~ distance_cost[j][prec_last_node_added] =
                        //~ d[j*n+prec_last_node_added] + d[j*n+last_node_added]
                        //~ - d[prec_last_node_added*n+last_node_added];
                //~ //recalculate distances
                //~ if(d[j*n+last_node_added] > distance_to_subtour[j]) {
                    //~ distance_to_subtour[j] = d[j*n+last_node_added];
                    //~ farest_node_in_subtour[j] = last_node_added;
                //~ }
                //~ //find the best distance 
                //~ if(distance_to_subtour[j] > max_distance) {
                    //~ max_distance = distance_to_subtour[j];
                    //~ best_node_to_add = j;
                    //~ add_next_to = farest_node_in_subtour[j]; //WRONG to change !!!
                //~ }
            //~ }
        //~ }
        //~ // Adding a node
        //~ int temp_next = g.next[add_next_to];
        //~ g.next[add_next_to] = best_node_to_add;
        //~ g.next[best_node_to_add] = temp_next;
        //~ in_subtour[best_node_to_add] = true;
        //~ last_node_added = best_node_to_add;
        //~ prec_last_node_added = add_next_to;
    //~ }
//~ }

//~ void MI(Graph& g, double* d) // work todo
//~ {
    //~ int n=g.n;
    //~ // cost to add nodes in the current subtour, next to a node
    //~ double distance_cost[n][n];
    //~ double min_distance_cost=DISTANCE_MAX; // to the graph
    //~ int best_node_to_add=1;
    //~ int add_next_to=0;
    //~ int last_node_added=0;
    //~ int prec_last_node_added=0;
    //~ bool in_subtour[n];
    //~ int size_subtour=1;
    //~ in_subtour[0] = true;
    //~ g.next[0] = 0;
    //~ for(int i=1; i<n; i++) { //init (case 0 not needed)
        //~ for(int j=1; j<n; j++) {
            //~ distance_cost[i][j] = DISTANCE_MAX;
        //~ }
        //~ in_subtour[i] = false;
    //~ }
    //~ for(int i=1; i<n; i++) { // ith node to add
    //~ // Looking for the node to add to the subtour
        //~ min_distance_cost=DISTANCE_MAX;
        //~ for(int j=0; j<n; j++) { 
            //~ if(in_subtour[j] == false) {// for every node not in subtour
                //~ //distance cost for new node to calculate
                //~ distance_cost[j][last_node_added] = 
                        //~ d[j*n+last_node_added] + d[j*n+g.next[last_node_added]]
                        //~ - d[last_node_added*n+g.next[last_node_added]];
                //~ //and to recalculate for its precedent node 
                //~ //because its next node (neighbour) has changed
                //~ distance_cost[j][prec_last_node_added] =
                        //~ d[j*n+prec_last_node_added] + d[j*n+last_node_added]
                        //~ - d[prec_last_node_added*n+last_node_added];
                //~ //find the best distance cost
                //~ int current_node=0;
                //~ for(int k=0; k<size_subtour; k++){
                    //~ if(distance_cost[j][current_node] < min_distance_cost) {
                        //~ min_distance_cost = distance_cost[j][current_node];
                        //~ best_node_to_add = j;
                        //~ add_next_to = current_node;
                    //~ }
                    //~ current_node = g.next[current_node];
                //~ } 
            //~ }
        //~ }
        //~ // Adding a node
        //~ int temp_next = g.next[add_next_to];
        //~ g.next[add_next_to] = best_node_to_add;
        //~ g.next[best_node_to_add] = temp_next;
        //~ in_subtour[best_node_to_add] = true;
        //~ size_subtour++;
        //~ last_node_added = best_node_to_add;
        //~ prec_last_node_added = add_next_to;
    //~ }
//~ }

int main ()
{
    string filename_in, filename_out;
    generator.seed(SEED);
    for (int i = 0; i < NB_TEST; i++) {
        // we generate input files to record data (we could also directly
        // save data in variables)
        filename_in = "./input/input_" + to_string(i) + ".txt";
        gen(NB_POINTS, MAX_X, MAX_Y,filename_in); // to make in option
    }
    for (int i = 0; i < NB_TEST; i++) {
        for(int algo=1; algo<=3; algo++) {
            filename_in = "./input/input_" + to_string(i) + ".txt";
            readInputPoints(filename_in);
            initCircuit();
            calculateDist2();
            if(algo==1) {
                PPI();
                filename_out = "./output/output_PPI_" + to_string(i) + ".txt";
            } else if(algo==2) {
                PLI();
                filename_out = "./output/output_PLI_" + to_string(i) + ".txt";
            } else {
                filename_out = "./output/output_MI_" + to_string(i) + ".txt";
            }
            writeOut(filename_out);
        }
        
    }
    return 0;
}
