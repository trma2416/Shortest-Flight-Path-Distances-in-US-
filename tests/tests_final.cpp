#include <cmath>
#include <gtest/gtest.h>
#include "../code/final.h"
using namespace std;
//in our tests, we will chech that our edge distances differences between
//our cmputed weigths and the actual weights are within this EPSILON
constexpr float EPSILON = 1e-4f;

//this tests our initialization of our nodes 
//and member functions to ensure they are working properly
TEST(NodeTest, initialization){
    Node_p node("cityA");
    //made up coordinates and distances
    node.set_coord(40.0f, 20.0f);
    node.set_d(100.0f);

    EXPECT_EQ(node.get_name(), "cityA");
    EXPECT_NEAR(node.get_lat(), 40.0f, EPSILON);
    EXPECT_NEAR(node.get_long(), 20.0f, EPSILON);
    EXPECT_NEAR(node.get_dist(), 100.0f, EPSILON);
    


}