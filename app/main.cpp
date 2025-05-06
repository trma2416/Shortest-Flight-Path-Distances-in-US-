#include <iostream>

#include "../code/final.h"

using namespace std;

int main(){
    Graph* G = new Graph;
    creategraph(G, "../code/us_state_capitals.txt");
    create_edges(G);
    
    
    
    string start;
    string finish;

    while(start != "Q"){

        cout << "Please Enter a Starting City, or enter Q to quit (must be a US state capital)" << endl;
        cout <<"\n";
        cin>>start;
        if(start == "Q"){
            break;
        }
        bool in_graph = false;
        if(G->get_node_by_name(start) != nullptr){
            in_graph = true;
        }
        if(in_graph){
            cout << "Valid Starting Location, Please enter a Destination city" << endl;
            cin>>finish;
        }
        bool fin_in_graph = false;
        if(G->get_node_by_name(finish) != nullptr){
            fin_in_graph = true;
            
        }
        if(fin_in_graph){
            cout << "Valid Destination" << endl;
            cout << "\n";
            vector<Node_p*> path = G->dijkstra(start, finish);
            
            cout << "Distance traveled " << path[1]->get_d() << endl;
        }

        
    }
    

    
    delete G;
    return 0;
}