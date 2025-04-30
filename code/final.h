#ifndef FINAL_H__
#define FINAL_H__

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include <set>
using namespace std;
//Use a node to store the data we need to calculate distances
//also need to keep track of the parent node in order to determine path
class Node_p {
private:
    //lat and longitude to store the coordinates of the city
    float lat;
    float longitude;
    string name; //name of city 
    float dist;
    Node_p* parent = nullptr; 
public:
    Node_p(string city);
    ~Node_p();
    string get_name();
    void set_data(string s);
    void set_d(float d);
    void set_coord(float lattitude, float longi);
    void set_parent(Node_p* p);
    float get_lat();
    float get_long();
    float get_dist();
    Node_p* get_parent();
};
//Dijkstra's algorithm requires a priority queue to work efficiently
//use a min heap since it maintains the property that the smallest priorty value will always be popped first
struct heapq{
    vector<Node_p*> queue;
    
};

class edge{
    private:
        Node_p* end1;
        Node_p* end2;
        float wt;
    public:
        edge(Node_p* n1, Node_p* n2);
        ~edge();
        
        Node_p* get_st();
        Node_p* get_end();
        //set_wt will use the haversine function to determine the weight of the edge
        void set_wt(float w); 
        float get_wt();
        float hav();
    };
    
class Graph{
public:
    Graph();
    ~Graph();
    
    vector<Node_p*> get_nodes();
    vector<edge*> get_edges();
    void add_node(Node_p* n);
    void add_edge(edge* e);
    vector<edge*> getadj(Node_p* n);
    //this will implement our search algorithm which we will use to find the shortest flight path based
    //on DISTANCE determined by using haversine function
    vector<Node_p*> dijkstra(Node_p* source, Node_p* dest);
private:
    vector<Node_p*> nodes;
    vector<edge*> edges;

    
};


//initialize new min heap queue should return a pointer to a new min-heap
heapq* init();

//useful helperfunction which swaps elemets at index i and j in place
void swap_(heapq*& heap, int i, int j);

//this will bubble down an element in the min-heap at postion j to restore the
//heap property of its children must be less than or equal to the parent node
void bubble_down(heapq*& heap, int j);

//helper for insert as we will insert the node at the end of heap and bubble it up into the correct position
void bubble_up(heapq*& heap, int pos);

//push a node onto the heap, we will only call push and pop in the algorithm, all the rest are helpers
void push_(heapq*& heap, Node_p* node);

//this will be used to restore a heap until it satisfies the min-heap property
void heapify(heapq*& heap, int i, int n);

//pop will pop the lowest priority element off the queue and return a pointer to it whilst removing it from the queue
Node_p* pop_(heapq*& heap);



//this will be our shorthand hav function
float haversine(float theta);

#endif