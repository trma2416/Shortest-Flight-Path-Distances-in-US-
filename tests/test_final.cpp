#include <cmath>
#include <gtest/gtest.h>
#include "../code/final.h"
using namespace std;
//in our tests, we will chech that our edge distances differences between
//our cmputed weigths and the actual weights are within this EPSILON
constexpr float EPSILON = 1e-3f;

//helper function since inputs of coordinates are in degrees
float deg_to_rad(float angle){
    return angle * (2 * M_PI) / 360.0f;
}

//this tests our initialization of our nodes 
//and member functions to ensure they are working properly
//note:this first test suite was written with chat-gpt to get an understanding of the syntax for test functions
TEST(NodeTest, initialization){
    Node_p node("cityA");
    //made up coordinates and distances
    node.set_coord(40.0f, 20.0f);
    node.set_d(100.0f);

    EXPECT_EQ(node.get_name(), "cityA");
    EXPECT_NEAR(node.get_lat(), deg_to_rad(40.0f), EPSILON);
    EXPECT_NEAR(node.get_long(), deg_to_rad(20.0f), EPSILON);
    EXPECT_NEAR(node.get_d(), 100.0f, EPSILON);
    


}
//we will specifically test that our haversine function is working correctly
TEST(EdgeTests, haversine){
    Node_p* n1 = new Node_p("City1");
    Node_p* n2 = new Node_p("City2");

    //make some fake coordinates that are perpendicular such thaat the distane doesnt cross any latitudes
    n1->set_coord(0.0f, 0.0f);
    n2->set_coord(0.0f, 90.0f);
    //compute the haversine distance
    edge e(n1, n2);
    float wt_dist = e.inverse_hav();
    e.set_wt(wt_dist);

    EXPECT_EQ(e.get_st()->get_name(), "City1");
    EXPECT_EQ(e.get_end()->get_name(), "City2");

    EXPECT_NEAR(e.get_wt(), 6225.222, EPSILON);

    delete n1;
    delete n2;
    
}
//test our priority queue to make sure the min heap maintains its structure
TEST(PriorityQueueTests, Push_and_pop){
    heapq* Q = init();
    Node_p* a = new Node_p("a");
    Node_p* b = new Node_p("b");
    Node_p* c = new Node_p("c");
    Node_p* d = new Node_p("d");
    //use the distance as our priority in queue
    a->set_d(5.0f);
    b->set_d(2.0f);
    c->set_d(3.0f);
    d->set_d(1.0f);
    //push nodes into queue
    push_(Q, a);
    push_(Q, b);
    push_(Q, c);

    EXPECT_EQ(pop_(Q)->get_name(), "b");
    push_(Q, d);
    EXPECT_EQ(pop_(Q)->get_name(), "d");

    //relaim data
    delete a;
    delete b;
    delete c;
    delete d;
    delete Q;
}

TEST(GraphTest, AdjacentNodes){
    Node_p* a = new Node_p("a");
    Node_p* b = new Node_p("b");
    Node_p* c = new Node_p("c");
    Node_p* d = new Node_p("d");

    edge* e1 = new edge(a,b);
    edge* e2 = new edge(a,c);
    edge* e3 = new edge(b,c);
    edge* e4 = new edge(c,d);

    Graph* g = new Graph;
    g->add_edge(e1);
    g->add_edge(e2);
    g->add_edge(e3);
    g->add_edge(e4);

    g->add_node(a);
    g->add_node(b);
    g->add_node(c);
    g->add_node(d);
    vector<string> names;
    unordered_set<edge*> adj = g->getadj(a);
    for(auto e: adj){
        names.push_back(e->get_end()->get_name());
    }
    vector<string> adj_a = {"c", "b"};
    EXPECT_EQ(names, adj_a);

    
    delete g;
    
}

TEST(GraphRealData, DijkstrasAlgorithm){
    //now lets use some real data with real coordinates and see if my code and fastest path works
    Graph* G = new Graph;
    Node_p* NY = new Node_p("New York City");
    NY->set_coord(40.7128f, 74.0060f);
    G->add_node(NY);

    Node_p* CO = new Node_p("Denver");
    CO->set_coord(39.7392f, -104.9903f);
    G->add_node(CO);

    Node_p* CA = new Node_p("Sacremento");
    CA->set_coord(38.575764f, -121.478851f);
    G->add_node(CA);

    Node_p* IL = new Node_p("Chicago");
    IL->set_coord(41.881832f, -87.623177f);
    G->add_node(IL);

    Node_p* WA = new Node_p("Seattle");
    WA->set_coord(47.6062f, -122.3321f);
    G->add_node(WA);

    Node_p* AK = new Node_p("Anchorage");
    AK->set_coord(61.2176f, -149.863129f);
    G->add_node(AK);

    //create our edges
    create_edges(G);
    //lets test our path from denver to nyc
    vector<Node_p*> path = G->dijkstra("Denver", "New York City");
    vector<string> visited;
    for(auto v : path){
        visited.push_back(v->get_name());
        
    }
    vector<string> exp = {"Denver", "New York City"};
    EXPECT_EQ(visited, exp);
    delete G;
}