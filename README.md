# friendSuggestions
Description: This File is responsible for the description of the extension 
section of this project. A detailed overview of why this program was designed
and how to run the program can be found written in this document. 

# The problem solved  
In large social networks such as Facebook and Twitter, you find a large array of
users, and attempting to know what suggestion should be made for each user (in 
terms of who to befriend or follow), can be rather difficult. The problem solved
is basically finding closed components within these social networks and writing 
out the friend/follow suggestion to be made.

# How it was solved 
This problem was solved for both social networks Facebook and Twitter.
Since Facebook is based on an undirected relationship (friendship goes both
ways), then using an egoNetwork was implemented to find the friend suggestion
for a user specified group. The user would provide a specific user, and the
program designs an edgNetwork centered around the specified user, suggesting
friendships to those not connected within to each other but are still within 
the same network. 
Otherwise, since twitter is a graph of undirected edges (representing follows
or retweets), then a strongly connected component algorithm is used instead.
The entire graph is separated into strongly connected components by running 
a DFS in forward order and backward order after traversing the edges of the 
graph. The output follow suggestions are for any two nodes within the same 
strongly connected component that are not directly connected to each other in 
any way. (This means it only occurs when neither twitter users follow the 
other user at hand).

# How it was tested
After implementing the necessary function for this program to work, different
test files were created with the actual graph already drawn out by hand. That 
way, we can use the drawing to validate the output of the program by comparing
the manual results with the program ones.  The testing for the facebook one took
place using the small_test_graph and designated facebook testing files. 

# HOW TO RUN THE PROGRAM 
1. For the facebook version: 

   Run the following command; 
   ./extension [input file] [output file] facebook [center node for egonet] 

   Example: 
   ./extension small_test_graph.txt outfile1 facebook 1

2. For the twitter version: 

   Run the following command;
   ./extension [input file] [output file] twitter

   Example:
   ./extension twitter_file1.txt outfile2 twitter

# Database used 
The database used, showed minimally in the test files, is given in the formating of two numbers per line, where each number represents a user in either the facebook social network or the twitter social network. The two numbers on the same line in the facebook version means they are mutual friends (represented as a directed relationship, although it is mutual and undirected in nature). The twitter version shows the first number as the account following that of the second number (represnted as a directed relationship

# Smaller testing files
For the facebook implementation: 
1. facebook_test1.txt
2. facebook_test2.txt
3. facebook_test3.txt

For the twitter implementation:
1. twitter_test1.txt
2. twitter_test2.txt
3. twitter_test3.txt
