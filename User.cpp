/*
 * User.cpp
 * Author: Hasan Al Jamaly & Nicole Stone
 * Date:   6/5/2017
 * Assignment: PA4 
 * Description: This file contains the class implementation of the User class, 
 * used to represent the vertices in the friend graph used for social network
 * paltform representation. The users can be connected by each other with 
 * friendships (or follows).
 */
#include "User.h"
#include "iostream"
using namespace std;

  /*
   * This is the constructor that create a User with a specific id. Takes the 
   * id as the parameter and returns nothing.
   */
User::User(int id) : id(id) {} 

  /* 
   * THis is the copy constructor of the user class. Takes another user as a
   * parameter and performs a deep copy of the object. 
   */
User::User(User& other) {
  id = other.id;
  // deep copy of all the friendships to avoid memory issues
  for (auto friendship : other.friendships) {
    Friendship * newFriend = new Friendship(*friendship);
    friendships.push_back(newFriend);
  }
}


  /* 
   * This is the function that connects two users together by adding the 
   * friendship to the vector of friendships. Takes the friendship pointer
   * as a parameter an returns true or false on success.
   */ 
bool User::newFriendship(Friendship* friendship) {
  friendships.push_back(friendship);
  return true;
}

  /* 
   * This function connects two users together in reverse by adding the 
   * friendship to the vector of reverse friendships. Takes the friendship 
   * pointer as a parameter and returns true or false based on success.
   */
bool User::newReverseFriendship(Friendship* friendship) {
  reverseFriendships.push_back(friendship);
  return true;
}

  /* 
   * This is an operator overload that takes in another User and compares 
   * for equality based on whether or not the ids are equal. Returns true or
   * false depending on the id of the users. 
   */
bool User::operator==(User other) const {
  return (id == other.id);
}

  /* 
   * This functions is provided with a friendship pointer as a parameter and
   * follows that pointer to retrieve the user that it is connecting to the
   * current user. Returns the other end of the edge. 
   */
User* User::followFriendship(Friendship* friendship) {
  if (id == friendship->first->id) {
    return friendship->second;
  }
  else if (id == friendship->second->id) {
    return friendship->first;
  }
}

  /* 
   * THis function validates whether the two users are friends by providing the
   * user pointer and checking to see if they are friends by looping through the
   * friendships and seeing if any other Users the current user is the same as
   * the one provided. Returns true or false based on the status of their 
   * connection.
   */
bool User::areFriends(User * other) {
  // loops to check all friendships 
  for (auto friendship : friendships) {
    if (followFriendship(friendship)->id == other->id) { 
      return true;
    }
  }
  return false;
}

  /* 
   * This is the destructor that is responsible for freeing up all the memory
   * associated with the edges. Takes no parameters and no returns.
   */
User::~User() { 
  // deletes all friendships to free up that space yo
  for (auto it : friendships) {
    delete it; 
    it = nullptr;
  }
  for (auto it : reverseFriendships) {
    delete it; 
    it = nullptr;
  }
}
