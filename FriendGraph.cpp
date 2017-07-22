/*
 * FriendGraph.cpp
 * Author: Hasan Al Jamaly & Nicole Stone
 * Date:   6/5/2017
 * Assignment: PA4 
 * Description: This file contains the class implementation of the FriendGraph, 
 * a graph that is formed by twitter and facebook user connections determined
 * by the followers/friends of each social platform. Nodes represent users and
 * edges represent the connection whether friendship or follow.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <climits>
#include <stack>
#include <queue>
#include <functional>
#include <set>
#include "FriendGraph.h" 
#define OUTHEADER "Follow Suggestions to be released:\n"
#define OUTHEADER2 "Friend Suggestions to be released:\n"

using namespace std; 

    /* 
     * This is function that reads in the nodes from a file, and creates and 
     * connects the graph accordingly. It takes in the name of the file to 
     * load the graph from, and returns true or false for success or failure.
     */

bool FriendGraph::loadFromFile(const char* in_filename) {
    cout << "Reading " << in_filename << endl;
    // Initialize the file stream
    ifstream infile(in_filename);
    int friendshipNum = 0;
 
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        if (!getline( infile, s )) break;

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            if (!getline( ss, next, ' ' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 2) {
            continue;
        }

        // Storing the ids of the users
        int first = (stoi(record[0]));
        int second = (stoi(record[1]));

        // Creating the new Users to place in tree
        User * newUser1;
        User * newUser2;

        // Checking whether users are in graph or not
        if (users.find(first) == users.end()) {
          newUser1 = new User(first);
          pair<int, User*> insertUser(first,newUser1);
          users.insert(insertUser);
        }
        else { 
          newUser1 = users.find(first)->second;
        } 

        if (users.find(second) == users.end()) {
          newUser2 = new User(second);
          pair<int, User*> insertUser(second,newUser2);
          users.insert(insertUser);
        }
        else { 
          newUser2 = users.find(second)->second;
        } 

        // Connecting the Users with a friendship/follow
        Friendship* newFriendship = new Friendship(newUser1, newUser2);
        friendships.push_back(newFriendship);
       
        // Storing the reverse of the friendship for findSCC algorithm
        Friendship* reverse = new Friendship(*newFriendship);
        reverse->flip();
        reverseFriendships.push_back(reverse); 
        newUser1->newFriendship(newFriendship);
        newUser2->newReverseFriendship(reverse);

    }
    cout << "#users: " << users.size() << endl;
    cout << "#follows/retweets/friendships: " << friendships.size() << endl;
     
    // Checking if file opened properly  
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    cout << "done\n";

    return true;
}

    /* 
     * This is the function that finds the user in the map of user nodes 
     * depending on the id of the user provided. Takes the id of the user and 
     * returns a pointer to the user. 
     */
User* FriendGraph::findUser(int id) {
  // search for user using his id
  auto it = users.find(id);
  // if not present
  if (it == users.end()) {
    return nullptr;
  }
  // if present
  return it->second;
}


    /* 
     * This is the destructor for the FriendGraph, which is responsible for 
     * deallocating all the users in the tree, but not the friendships as that
     * is the responsibility of the User destructor. Takes in no parameters and
     * returns nothing.
     */

FriendGraph::~FriendGraph() {
  // deleting all users from graph
  for (auto it : users) {
    if (it.second) {
      delete it.second;
      it.second = nullptr;
    }
  }
}

    /* 
     * This is the function that is responsible for creating an egonetwork 
     * with the source being the id provided as the parameter. Returns the 
     * smaller subgraph that is the egonetwork of the id node.
     */

FriendGraph FriendGraph::getEgonet(int source) {
  FriendGraph returnGraph; // graph to be constructed

  // Checking validity of source
  if (!findUser(source)) {
    cerr << "Source for Egonet is not in the graph!\n";
    return returnGraph;
  }

  // finding the source node 
  User * sourceUser = new User(*findUser(source));
  returnGraph.users.insert(make_pair(source,sourceUser));

  // looping to push all users that the source is connected to
  for (auto newFriendship : sourceUser->friendships) {
    returnGraph.friendships.push_back(newFriendship);
    User * neighborUser =
                new User(*sourceUser->followFriendship(newFriendship));
    returnGraph.users.insert(make_pair(neighborUser->id,neighborUser));
  }
  // connecting the other nodes to each other if a friendship exists already
  for (auto user : returnGraph.users) {
    for (auto friendship = user.second->friendships.begin();
          friendship !=  user.second->friendships.end();++friendship) {
    User * newNeighbor = user.second->followFriendship(*friendship);
      if (returnGraph.findUser(newNeighbor->id)) {
        returnGraph.friendships.push_back(*friendship);
      }
      // otherwise, delete it from the graph
      else { 
        delete *friendship;
        *friendship = nullptr;
        user.second->friendships.erase(friendship);
        --friendship;
      }
    }
  }
  return returnGraph;
} 

    /* This is the function responsible for performing a depth first search 
     * of the graph that is performing the function call. Takes in a 
     * stack of vertices, an output parameter stack for visited nodes, 
     * a boolean determining if this is the forward search of the graph or 
     * the second backward search in the SCC algorithm, and a pointer to a 
     * vector to pointers to vectors of user pointers that will be filled with
     * the resulting strongly connected components as the output parameter.
     */

void FriendGraph::DFS(stack<User*>* vertices, stack<User*>* finished,
                       bool forward, vector<vector<User*>*>* returnVec) { 

  vector<User*> fake; // to be passed in when doing the forward DFS visit
  // until no vertices are present
  while (!vertices->empty()) {
    User* vertex = vertices->top();
    vertices->pop();
    if (!vertex->visited) { 
      // if going backward, make sure you store nodes in same SCC in a vector
      if (!forward) {
        vector<User*> * newVec = new vector<User*>;
        returnVec->push_back(newVec);
        (*returnVec)[returnVec->size()-1]->push_back(vertex);
        DFSVisit(vertex,finished,forward,(*(*returnVec)[returnVec->size()-1]));
      }
      // if going forward, just pass in the fake one
      else {
        DFSVisit(vertex,finished,forward, fake);
      }
    }
  }
  return;
}

    /* This is a helper function that is responsible for visiting a single 
     * node and is called by the DFS algorithm to perform the visit. Takes 
     * in the vertex to visit, the stack of finished nodes as an output 
     * parameter, a boolean to indicate a forward or backward search, and 
     * the vector to place the nodes in the same SCC to return to DFS.
     */
void FriendGraph::DFSVisit(User* vertex, stack<User*>* finished, bool forward,
                           vector<User*>& returnVec) {
  vertex->visited = true;
  // if going forward, just visit the neighbors
  if (forward) {
    for (auto friendship : vertex->friendships) { 
      User * neighbor = vertex->followFriendship(friendship);
      if (!neighbor->visited) {
        DFSVisit(neighbor,finished, forward, returnVec);
      }
    }
  }
  // if going backward, store the node in the proper SCC
  else {
    for (auto friendship : vertex->reverseFriendships) { 
      User * neighbor = vertex->followFriendship(friendship);
      if (!neighbor->visited) {
        returnVec.push_back(neighbor);
        DFSVisit(neighbor,finished, forward, returnVec);
      }
    }
  }
    finished->push(vertex);
}

    /*
     * An algorithm that takes the map of users and places them in a stack to 
     * pass in to the DFS algorithm. Takes in an output parameter stack to fill
     * and return nothing.
     */
void FriendGraph::stackVertices(stack<User*>* vertices) {
  // Place all vertices in a stack
  for (auto vertex : users) {
    vertex.second->visited = false;
    vertices->push(vertex.second);
  }
}

    /* 
     * This is the function responsible for finding the different strongly 
     * connected components in a twitter graph, and place all the nodes of 
     * a strongly connected component in a 2 dimensional array that contains
     * all the SCCs. Takes in the output parameter 2D vector to fill with 
     * SCCs, and returns nothing.
     */
void FriendGraph::findSCC(vector<vector<User*>*>* returnVec) { 
  // allocated needed stacks
  stack<User*> * vertices = new stack<User*>; 
  stack<User*> * finished = new stack<User*>;
  stack<User*> temp;
  stackVertices(vertices);
  // perform forward DFS
  DFS(vertices, finished, true, returnVec);

  // Set back visited attribute to false
  while (!finished->empty()) { 
    finished->top()->visited = false; 
    temp.push(finished->top());
    finished->pop();
  }
  while (!temp.empty()) {
    finished->push(temp.top());
    temp.pop();
  }
  // Backward DFS
  DFS(finished, vertices, false, returnVec);

  // Free up that memory yo
  delete vertices;
  delete finished;
  return;
}

    /* 
     * This is the function that calls findSCC, and based on the output of that
     * loops through the strongly connected components and finds the nodes that
     * have no connection between them (neither one follows the other). The 
     * function takes the name of an output file to write the suggestions as a
     * parameter, and return nothing.
     */
void FriendGraph::suggestFollows(const char * outfile) {
  // open the file
  ofstream out_file(outfile);
  out_file << OUTHEADER;
  // initialize input vector for SCC algorithm
  vector<vector<User*>*>* outputVec = new vector<vector<User*>*>;

  // finding the strongly connected components
  findSCC(outputVec);

  // looping through strongly connected components and checking what users are
  // not connected together
  for (auto vec : *outputVec) { 
    for (auto it : *vec) {
      for (auto it2 : *vec) {
        if (!(it==it2) && !(it->areFriends(it2)) && !(it2->areFriends(it))) {
          out_file << "Suggestion for " <<it->id << ": you should follow ";
          out_file << it2->id << "!" <<  endl;
        }
      }
    }
  }
  out_file.close();
  // freeing up that space yo
  for (auto vec : *outputVec) {
    delete vec; 
  }
  delete outputVec;
}

    /* 
     * This is the function that calls getEgonet, and is responsible for 
     * firstly creating the egonetwork and then looping through the users in the
     * network to check who is not friends with the other, and makes the 
     * suggestion for both to friends each other. Takes in the id of the user 
     * to source the Egonetwork as an input parameter, and also the name of the
     * output file to write the suggestions to. Returns nothing.
     */
void FriendGraph::suggestFriends(const char * outfile, int id) {
  // opening the output file
  ofstream out_file(outfile);
  out_file << OUTHEADER2;
  bool suggestions = false; // to check if any suggestions have been written
  FriendGraph newGraph = getEgonet(id);
  
  // looping though users of the egonetwork and checking the nodes that are 
  // not connected togther
  for (auto user : newGraph.users) {
    for (auto user2 : newGraph.users) {
      if (user.first != user2.first && !user.second->areFriends(user2.second)) { 
        suggestions = true;
        out_file << "Suggestion for: ";
        out_file << user.first << ": you should add ";
        out_file << user2.first << endl;
      }
    }
  }
  // if all of the users are already friends
  if (!suggestions) {
    out_file << "All of " << id << "'s friends are already friends! Woohoo!\n";
  }
  out_file.close();
}
