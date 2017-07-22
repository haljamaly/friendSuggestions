/*
 * FriendGraph.h
 * Author: Hasan Al Jamaly
 * Date:   6/5/2017
 * Description: This file contains the class declaration of the FriendGraph, 
 * a graph that is formed by twitter and facebook user connections determined
 * by the followers/friends of each social platform. Nodes represent users and
 * edges represent the connection whether friendship or follow.
 */

#ifndef FRIENDGRAPH_H
#define FRIENDGRAPH_H

#include <iostream>
#include <string> 
#include <unordered_map>
#include <map>
#include <vector>
#include <stack>

#include "User.h"
#include "Friendship.h"

class User;
class Friendship;

using namespace std;


class FriendGraph {
private:
  vector<Friendship*> friendships; // the edges of the graph
  vector<Friendship*> reverseFriendships; // the reverse edges for SCC algorithm
  unordered_map<int, User*> users; // the nodes of the graph

public:

    /* 
     * This is function that reads in the nodes from a file, and creates and 
     * connects the graph accordingly. It takes in the name of the file to 
     * load the graph from, and returns true or false for success or failure.
     */
    bool loadFromFile(const char* in_filename);

    /* 
     * This is the function that finds the user in the map of user nodes 
     * depending on the id of the user provided. Takes the id of the user and 
     * returns a pointer to the user. 
     */
    User * findUser(int id);
    
    /* 
     * This is the function that is responsible for creating an egonetwork 
     * with the source being the id provided as the parameter. Returns the 
     * smaller subgraph that is the egonetwork of the id node.
     */
    FriendGraph getEgonet(int id);
  
    /* This is the function responsible for performing a depth first search 
     * of the graph that is performing the function call. Takes in a 
     * stack of vertices, an output parameter stack for visited nodes, 
     * a boolean determining if this is the forward search of the graph or 
     * the second backward search in the SCC algorithm, and a pointer to a 
     * vector to pointers to vectors of user pointers that will be filled with
     * the resulting strongly connected components as the output parameter.
     */
    void DFS(std::stack<User*>* vertices, std::stack<User*>* finished,
              bool forward, std::vector<std::vector<User*>*>* returnVec);
    
    /* This is a helper function that is responsible for visiting a single 
     * node and is called by the DFS algorithm to perform the visit. Takes 
     * in the vertex to visit, the stack of finished nodes as an output 
     * parameter, a boolean to indicate a forward or backward search, and 
     * the vector to place the nodes in the same SCC to return to DFS.
     */
    void DFSVisit(User* vertex, std::stack<User*>* finished, bool forward,
                  std::vector<User*>& returnVec);
    /*
     * An algorithm that takes the map of users and places them in a stack to 
     * pass in to the DFS algorithm. Takes in an output parameter stack to fill
     * and return nothing.
     */
    void stackVertices(std::stack<User*>* vertices);
    
    /* 
     * This is the function responsible for finding the different strongly 
     * connected components in a twitter graph, and place all the nodes of 
     * a strongly connected component in a 2 dimensional array that contains
     * all the SCCs. Takes in the output parameter 2D vector to fill with 
     * SCCs, and returns nothing.
     */
    void findSCC(vector<vector<User*>*>* returnVec);

    /* 
     * This is the function that calls findSCC, and based on the output of that
     * loops through the strongly connected components and finds the nodes that
     * have no connection between them (neither one follows the other). The 
     * function takes the name of an output file to write the suggestions as a
     * parameter, and return nothing.
     */
    void suggestFollows(const char * outfile);

    /* 
     * This is the function that calls getEgonet, and is responsible for 
     * firstly creating the egonetwork and then looping through the users in the
     * network to check who is not friends with the other, and makes the 
     * suggestion for both to friends each other. Takes in the id of the user 
     * to source the Egonetwork as an input parameter, and also the name of the
     * output file to write the suggestions to. Returns nothing.
     */
    void suggestFriends(const char * outfile, int id);
    
    /* 
     * This is the destructor for the FriendGraph, which is responsible for 
     * deallocating all the users in the tree, but not the friendships as that
     * is the responsibility of the User destructor. Takes in no parameters and
     * returns nothing.
     */
    ~FriendGraph();
};
#endif // FRIENDGRAPH_H

