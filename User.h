/*
 * User.h
 * Author: Hasan Al Jamaly
 * Date:   6/5/2017
 * Description: This file contains the class declaration of the User class, 
 * used to represent the vertices in the friend graph used for social network
 * paltform representation. The users can be connected by each other with 
 * friendships (or follows).
 */
#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>
#include "FriendGraph.h"
#include "Friendship.h"

class User { 
  friend class FriendGraph;
  friend class Friendship;

  private: 
  int id; // the qualifier for the user
  std::vector<Friendship*> friendships; // all friendships the user has
  std::vector<Friendship*> reverseFriendships; // reversed friendships for SCC
  bool visited = false; // indicator if DFS visit occured or not
  
  public: 
  
  /* This is the default constructor that does not take any parameter and 
   * returns nothing. Creates an empty User.
   */
  User() {} 

  /*
   * This is the constructor that create a User with a specific id. Takes the 
   * id as the parameter and returns nothing.
   */
  User(int id); 

  /* 
   * THis is the copy constructor of the user class. Takes another user as a
   * parameter and performs a deep copy of the object. 
   */
  User(User& other);
  
  /* 
   * This is the function that connects two users together by adding the 
   * friendship to the vector of friendships. Takes the friendship pointer
   * as a parameter an returns true or false on success.
   */ 
  bool newFriendship(Friendship * friendship);

  /* 
   * This function connects two users together in reverse by adding the 
   * friendship to the vector of reverse friendships. Takes the friendship 
   * pointer as a parameter and returns true or false based on success.
   */
  bool newReverseFriendship(Friendship * friendship);

  /* 
   * This is an operator overload that takes in another User and compares 
   * for equality based on whether or not the ids are equal. Returns true or
   * false depending on the id of the users. 
   */
  bool operator==(User other) const;
  
  /* 
   * This functions is provided with a friendship pointer as a parameter and
   * follows that pointer to retrieve the user that it is connecting to the
   * current user. Returns the other end of the edge. 
   */
  User * followFriendship(Friendship * friendship);

  /* 
   * THis function validates whether the two users are friends by providing the
   * user pointer and checking to see if they are friends by looping through the
   * friendships and seeing if any other Users the current user is the same as
   * the one provided. Returns true or false based on the status of their 
   * connection.
   */
  bool areFriends(User * other);

  /* 
   * This is the destructor that is responsible for freeing up all the memory
   * associated with the edges. Takes no parameters and no returns.
   */
  ~User(); 
};
#endif // USER_H 
