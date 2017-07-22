/*
 * Friendship.h
 * Author: Hasan Al Jamaly & Nicole Stone
 * Date:   6/5/2017
 * Assignment: PA4 
 * Description: This file contains the class declaration of the Friendship 
 * class, which represents the edge in a social media platform. The friendship
 * is used to connect two users. THis friendship is directed, meaning there 
 * would be two going in opposite directions to represent a facebook friendship
 * but only one to represent a twitter follow (since it is directed).
 */
#ifndef FRIENDSHIP_H
#define FRIENDSHIP_H

#include <iostream>
#include <string>
#include "User.h"
#include "FriendGraph.h"

class User;

class Friendship {
  friend class FriendGraph;
  friend class User;

  private: 
    User* first; // tail of the edge
    User* second;  // head of the edge

  public: 

    /* 
     * This is the constructor that takes in two user pointers to initialize 
     * the relationship between two users.
     */
    Friendship(User*& first, User*& second); 
    
    /* 
     * This is the ccopy constructor that takes a friendship and performs a
     * shallow copy of the users in the edge.
     */
    Friendship(Friendship& other); 

    /* 
     * This is the overloaded equality check operator that checks if two 
     * friendships are the same depending on the value of their first and 
     * last User ids. Takes in another friendship and returns true or false
     * based on the status of the equality.
     */
    bool operator==(Friendship other) const;
   
    /* 
     * This is a function that exchanges the first and second user pointer 
     * together to place the reverse friendship in the reverse vector for 
     * use in the SCC algorithm.
     */
    Friendship * flip();
};

#endif // FRIENDSHIP_H

