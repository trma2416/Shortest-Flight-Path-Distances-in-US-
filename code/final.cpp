#include "final.h"
using namespace std;


//node_p member functions
Node_p::Node_p(string city){
    name = city;
}
Node_p::~Node_p(){

}
string Node_p::get_name(){
    return name;
}
void Node_p::set_d(float d){
    dist = d;
}
//input params need to be in degrees
void Node_p::set_coord(float lattitude, float longi){
    //convert from degrees to radians
    lat = (lattitude * 2 * M_PI) / 360.0f;
    longitude = (longi * 2 * M_PI) / 360.0f;
}
void Node_p::set_parent(Node_p* p){
    parent = p;
}
float Node_p::get_lat(){
    return lat;
}
float Node_p::get_long(){
    return longitude;
}

float Node_p::get_dist(){
    return dist;
}
Node_p* Node_p::get_parent(){
    return parent;
}



//graph member functions
Graph:: Graph(){

}
Graph:: ~Graph(){

}
vector<Node_p*> Graph::get_nodes(){
 return nodes;   
}
vector<edge*> Graph::get_edges(){
    return edges;
}
void Graph::add_node(Node_p* n){
    nodes.push_back(n);
}
void Graph::add_edge(edge* e){
    edges.push_back(e);
}
vector<edge*> Graph::getadj(Node_p* n){
    vector<edge*> ret;
    for(vector<edge*>::iterator i = edges.begin(); i != edges.end(); i++){
        edge* e = *i;
        if(e->get_st() == n || e->get_end() == n){
            ret.push_back(e);
        }
        
    }
    return ret; 
}
vector<Node_p*> Graph::dijkstra(Node_p* source,Node_p* dest){

    //iterate through all vertices in graph and initialize the distances and parent nodes
    for(auto& v : nodes){
        if(v == source){
            v->set_d(0);
        }
        else{
            v->set_d(INFINITY);
        }
    }
    //initialize our queue
    heapq* Q = init();
    push_(Q, source);
    //this will store our destination node 
    Node_p* x = nullptr;
    while(!Q->queue.empty()){
        Node_p* u = pop_(Q); 
        //condition to break out of loop
        if(u->get_name() == dest->get_name()){
            x = u;
            break;
        }
        //get all edges from u which will give us the adjacent nodes
        vector<edge*> adj = getadj(u);
        for(auto& e: adj){
            //calculate the distance between each edge which will act as the weight
            float wt_updated = e->hav();
            e->set_wt(wt_updated); //update the weight 
            //start will always be node u
            Node_p* v = e->get_end();
            //edge relax condition
            float dist = u->get_dist() + e->get_wt();
            if(dist < v->get_dist()){
                v->set_d(dist);
                v->set_parent(u);
                push_(Q, v);
            }
        }
    }
    //we will popuate this vector with the nodes that are on the shortest path
    vector<Node_p*> path;
    if(x->get_parent() != nullptr || x->get_name() == dest->get_name()){
        //iterate backwards starting from destination node and 
        //add the parent to the beginning of the path vector
        while(x != nullptr){
            path.insert(path.begin(), x);
            x = x->get_parent();
    }
    }
    return path;


}


//EDGES MEMBER FUNCTIONS
edge::edge(Node_p* n1, Node_p* n2){
    end1 = n1;
    end2 = n2;
}
edge::~edge(){

}
Node_p* edge::get_st(){
    return end1;
}
Node_p* edge::get_end(){
    return end2;
}
void edge::set_wt(float w){
    wt = w;
}
float edge::get_wt(){
    return wt;
}
float edge::hav(){
    Node_p* st = get_st();
    Node_p* end = get_end();
    
    float phi_1 = st->get_lat();
    float phi_2 = end->get_lat();
    float delta_phi = phi_2 - phi_1;
    float delta_lambda = end->get_long() - st->get_long();
    float hav = haversine(delta_phi) + cos(phi_1)*cos(phi_2)*haversine(delta_lambda);
    //now we have hav(theta) = (1 - cos(theta))/2
    //this is the inverse haversine formula
    float w = 2 * EARTH_RADIUS * asin(pow(hav, 0.5));
    return w;
}





//min heap functions and extra helper functions
heapq* init(){
    heapq* queue = new(heapq);
    return queue;
}
//swap function for our heapq
void swap_(heapq*& heap, int i, int j){
    Node_p* temp = heap->queue[i];
    heap->queue[i] = heap->queue[j];
    heap->queue[j] = temp;
}

void bubble_down(heapq*& heap, int j){
    int c = 2*j + 1; //child index of elem at index j
    float priority = (*heap->queue[j]).get_dist();
    while(c < heap->queue.size()){
        float min = priority;
        int min_idx = -1;
        for(int i = 0; i < 2 && i + c < heap->queue.size(); i++){
            if((*heap->queue[i + c]).get_dist() < min){
                min = (*heap->queue[i + c]).get_dist();
                min = i + c;
            }
        }
        if(min == priority){
            return;
        }
        else{
            swap_(heap, j, min_idx);
            j = min;
            c = 2*j + 1;
        }
    }
}
void bubble_up(heapq*& heap, int j){
    if(j == 0){
        return;
    }
    int p = (j - 1) / 2; //parent index, integer division in c++ will give us the floor
    if((*heap->queue[j]).get_dist() < (*heap->queue[p]).get_dist()){
        swap_(heap, j, p);
        bubble_up(heap, p);
    }
}
void push_(heapq*& heap, Node_p* node){
    heap->queue.push_back(node);
    int n = heap->queue.size();
    bubble_up(heap, n-1);
}
void heapify(heapq*& heap, int i, int n){
    int smallest = i;
    int l = 2*i + 1; //left child
    int r = 2*i + 2; //right child
    //if left child is smaller than parent
    if(l < n && (*heap->queue[l]).get_dist() < (*heap->queue[smallest]).get_dist()){
        smallest = l;
    }
    //if right child is smaller than parent
    if(r < n && (*heap->queue[r]).get_dist() < (*heap->queue[smallest]).get_dist()){
        smallest = r;
    }
    //if child node's distance is less than parents
    if (smallest != i){
        swap_(heap, i, smallest); //swap
        heapify(heap, smallest, n);//recursively call heapify
    }
}
Node_p* pop_(heapq*& heap){
    if(heap->queue.empty()){
        return nullptr;
    }
    else{
        //first element in the queue which has the lowest priority
        Node_p* elem = heap->queue[0];
        int n = heap->queue.size() - 1;
        //swap the last and the first element
        swap_(heap, 0, n); 
        //remove last element
        heap->queue.pop_back();
        //restore the heap property
        heapify(heap, 0, n-1); //n-1 because queue will have one less element
        return elem;
    }

}
//hav(theta) = sin^2(theta/2)
float haversine(float theta){
    return pow(sin(theta/2), 2);
}