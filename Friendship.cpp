/*
 * Friendship.cpp
 * Author: Hasan Al Jamaly
 * Date:   6/5/2017
 * Description: This file contains the class implementation of the Friendship 
 * class, which represents the edge in a social media platform. The friendship
 * is used to connect two users. THis friendship is directed, meaning there 
 * would be two going in opposite directions to represent a facebook friendship
 * but only one to represent a twitter follow (since it is directed).
 */
#include <iostream>
#include <string>
#include "Friendship.h"

    /* 
     * This is the constructor that takes in two user pointers to initialize 
     * the relationship between two users.
     */
Friendship::Friendship(User*& first, User*& second):first(first),second(second) {}

    /* 
     * This is the overloaded equality check operator that checks if two 
     * friendships are the same depending on the value of their first and 
     * last User ids. Takes in another friendship and returns true or false
     * based on the status of the equality.
     */
bool Friendship::operator==(Friendship other) const { 
  return ((first == other.first || first == other.second) 
           && (second == other.first || second == other.second));
}


    /* 
     * This is the ccopy constructor that takes a friendship and performs a
     * shallow copy of the users in the edge.
     */
Friendship::Friendship(Friendship& other) {
  first = other.first;
  second = other.second;
} 

    /* 
     * This is a function that exchanges the first and second user pointer 
     * together to place the reverse friendship in the reverse vector for 
     * use in the SCC algorithm.
     */
Friendship* Friendship::flip() {
    User * temp = first; 
    first = second;
    second = temp;
}
  
