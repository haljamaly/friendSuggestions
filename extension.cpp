/* 
 * Authors: Hasan Al Jamaly & Nicole Stone
 * Date: 6/5/2017
 * Assignment: PA4
 * File name: extension.cpp
 * Description:
 *  This is the driver program for the extension, which is responsible for
 *  producing different friend/follow suggestions in facebook or twitter 
 *  depending on the current relationships between users. Input arguments
 *  are the input file of connections, output file to write suggestions,
 *  either facebook or twitter to determine what program to run, and if 
 *  facebook is chosen then the node to be used as the source of the EgoNet
 *  has to be indicated as well.
 */
#include <string>
#include <chrono>
#include "FriendGraph.h"
#include "User.h"
#include "Friendship.h"
#define NUMARGS 4
#define USAGE "Wrong number of input arguments for ./extension.\n" 
#define USAGE2 "Usage: ./extension input_file output_file [twitter|facebook] "
#define USAGE3 "[egoNetwork center only if facebook is chosen].\n";

using namespace std;

/* 
 * This is the driver program responsible for parsing the command line arguments
 * and passing them to either the facebook program or the twitter program. 
 * Returns -1 for failure and 0 for success, and has argc (input argument 
 * count) and argv (array of input arguments) as input.
 */

int main (int argc, char * argv[]) {
  /* Check for input arguments and the validity of their count */
  if (argc < NUMARGS) {
    cerr << USAGE;
    cerr << USAGE2; 
    cerr << USAGE3;
    return -1;
  }

  char * database = argv[1]; // the input file with graph information
  char * outfile = argv[2]; // the output file to write suggestions
  string option = argv [3]; // either facebook or twitter
  
  /* if the facebook option is chosen, a node has to be provided */
  if (option == "facebook" && argc == NUMARGS) {
    cerr << "Choose a user as the center of the egoNetwork!\n"; 
    cerr << USAGE2;
    cerr << USAGE3; 
    return -1;
  }
  
  /* if option is neither facebook or twitter */
  if (option != "facebook" && option != "twitter") { 
    cerr << USAGE2;
    cerr << USAGE3;
    return -1;
  }

  /* loading the graph from the input file */
  FriendGraph graph; 
  graph.loadFromFile(database);
  
  /* running the twitter program */
  if (option == "twitter") { 
    graph.suggestFollows(outfile);
  }
  /* running the facebook program for the given node */
  if (option == "facebook") { 
    int egoNetCenter = stoi(argv[4]);
    graph.suggestFriends(outfile,egoNetCenter);
  }
  return 0; // SUCCESS! 
}
