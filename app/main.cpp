#include <iostream>

#include "../code/final.h"

using namespace std;

int main(){
    Graph* G = new Graph;
    creategraph(G, "../code/us_state_capitals.txt");
    create_edges(G);
    bool in_graph = false;
    for(auto& v: G->get_nodes()){
        if(v->get_name() == "Phoenix"){
            in_graph = true;
        }
    }
    if(in_graph){
        cout << "desination found in graph" << endl;;
    }
    
    
    string start = "Albany";
    string finish = "Honolulu";
    

    vector<Node_p*> path = G->dijkstra(start, finish);
    cout << path.size() << endl;
    cout << "Distance traveled " << path[1]->get_d() << endl;
    
    delete G;
    return 0;
}