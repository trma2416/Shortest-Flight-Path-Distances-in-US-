#include <iostream>

#include "../code/final.h"

using namespace std;

int main(){
    Graph* G = new Graph;
    creategraph(G, "../code/us_state_capitals.txt");
    create_edges(G);

    cout << "Please enter a starting city (must be a US state capital) " << endl;
    string start;
    string finish;
    cin >> start;

    bool is_in = false;
    for(auto n: G->get_nodes()){
        if(start==n->get_name()){
            is_in = true;
        }
    }
    
    if(is_in){
        
        cout << "Please enter a destination US state capital: " << endl;
    
        cin >> finish;
    }

    vector<Node_p*> path = G->dijkstra(start, finish);
    cout << path.size() << endl;
    for(auto n: path){
        cout << "Your Travel Includes: " << 
        n->get_name();
    }
    cout << "Distance Traveled: " << path[1]->get_d() << endl;
    return 0;
}