#include <iostream>
#include "../code/final.h"

using namespace std;

int main(){
    Graph* G = new Graph;
    creategraph(G);
    for(Node_p* n: G->get_nodes()){
        cout << n->get_name() << endl;
    }
    return 0;
}