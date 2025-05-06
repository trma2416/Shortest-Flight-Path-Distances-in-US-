#include "final.h"

using namespace std;


//node_p member functions
Node_p::Node_p(const string& city){
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

float Node_p::get_d(){
    return dist;
}
Node_p* Node_p::get_parent(){
    return parent;
}



//graph member functions
Graph:: Graph(){

}
Graph:: ~Graph(){
    for(auto e: edges){
        delete e;
    }
    edges.clear();
    for(auto n: nodes){
        delete n;
    }
    nodes.clear();

    node_map.clear();
}
vector<Node_p*> Graph::get_nodes(){
 return nodes;   
}
vector<edge*> Graph::get_edges(){
    return edges;
}
void Graph::add_node(Node_p* n){
    nodes.push_back(n);
    node_map[n->get_name()] = n;
}
void Graph::add_edge(edge* e){
    edges.push_back(e);
    
}
//this function using a hash table (map) to store the actual node pointer object rather than refering to a node by name
Node_p* Graph::get_node_by_name(const string& name){
    if(node_map.find(name) != node_map.end()){
        return node_map[name];
    }
    //if we dont find name in our map
    return nullptr;
}
unordered_set<edge*> Graph::getadj(Node_p* n){
    unordered_set<edge*> ret;
    string name = n->get_name();
    for(auto e: edges){
    
        if(e->get_st()->get_name() == name || e->get_end()->get_name() == name){
            if(ret.count(e))continue;
            ret.insert(e);
        }
        
    }
    return ret; 
}
vector<Node_p*> Graph::dijkstra(string source,string dest){
    //get actual node pointer objects for source and destination
    Node_p* src = get_node_by_name(source);
    Node_p* dst = get_node_by_name(dest);

    if(!src || !dst){
        cout << "Source or Destination not found in graph" << endl;
        return {};
    }
    //initialize our queue
    heapq* Q = init();
    //iterate through all vertices in graph and initialize the distances and parent nodes
    for(auto& v: nodes){
        v->set_d(INFINITY);
        v->set_parent(nullptr);
    }
    
    src->set_d(0);
    push_(Q, src);
    //this will store our destination node 
    Node_p* x = nullptr;


    while(!Q->queue.empty()){
        Node_p* u = pop_(Q); 
        
        //condition to break out of loop
        if(u == dst){
            x = u;
            break;
        }
        if(u==nullptr){
            cout << "seg fault";
            break;
        }

        
        
        //get all edges from u which will give us the adjacent nodes
        unordered_set<edge*> adj = getadj(u);
        
        for(auto& e: adj){
            

            //start will always be node u
            Node_p* v = (e->get_st() == u) ? e->get_end() : e->get_st();

            if(v == nullptr) continue;
            //cout << v->get_name() << ",";

            //edge relax 
            float dist = u->get_d() + e->get_wt();
            if(dist < v->get_d()){
                v->set_d(dist);
                v->set_parent(u);
                push_(Q, v);
            }
        }
    }
    //debug line
    if (x == nullptr) {
        cout << "Destination node not found in Dijkstra." << endl;
    }
    //we will popuate this vector with the nodes that are on the shortest path
    vector<Node_p*> path;
    if(x!= nullptr && (x->get_parent() != nullptr || x->get_name() == dest)){
        //iterate backwards starting from destination node and add the parent to the beginning of the path vector
        while(x){
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
//this will calculate our distances between nodes using the inverse haversine d = 2rarsin(sqrt(hav theta))
float edge::inverse_hav(){
    Node_p* st = get_st();
    Node_p* end = get_end();
    
    float phi_1 = st->get_lat();
    float phi_2 = end->get_lat();
    float delta_phi = phi_2 - phi_1;
    float delta_lambda = end->get_long() - st->get_long();

    //compute the angle using the haversine formula
    float hav = haversine(delta_phi) + cos(phi_1)*cos(phi_2)*haversine(delta_lambda);
    
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


void bubble_up(heapq*& heap, int j){
    if(j == 0){
        return;
    }
    int p = (j - 1) / 2; //parent index, integer division in c++ will give us the floor
    if((*heap->queue[j]).get_d() < (*heap->queue[p]).get_d()){
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
    if(l < n && (*heap->queue[l]).get_d() < (*heap->queue[smallest]).get_d()){
        smallest = l;
    }
    //if right child is smaller than parent
    if(r < n && (*heap->queue[r]).get_d() < (*heap->queue[smallest]).get_d()){
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
//this is a helper function that will read in a txt file with all the necessary data and construct a graph data structure
void creategraph(Graph* G, string filename){
    ifstream file(filename);
    string line;
    while(getline(file, line)){
        
        size_t commaPos = line.find(",");
        size_t colonPos = line.find(":");
        if(commaPos != string::npos && colonPos != string::npos){
            //this will give us the capital city name
            Node_p* node = new Node_p(line.substr(0, commaPos));

            //get coordinates
            float lat;
            float lon;
            string coords = line.substr(colonPos + 2);
            stringstream ss(coords);
            ss >> lat;
            //ignore the comma
            ss.ignore(2);
            ss >> lon;
            node->set_coord(lat, lon);
            G->add_node(node);
        }
    }
}
//create a fully connected graph which will have (n(n-1)/2) edges
void create_edges(Graph* G){
    vector<Node_p*> nodes = G->get_nodes();
    for(int i = 0; i < nodes.size(); i++){
        for(int j = i + 1; j < nodes.size(); j++){
            edge* e = new edge(nodes[i], nodes[j]);
            //were also gonna calculate the distances and add the weights to the edges so we dont have to in dijkstras 
            float new_wt = e->inverse_hav();
            e->set_wt(new_wt);
            G->add_edge(e);
        }
    }
}