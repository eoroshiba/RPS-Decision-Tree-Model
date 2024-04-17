
// **** Header file for the random decision forest (RDF) strategy. This strategy uses a collection of randomly created/trained decision trees, and takes the consensus among them 
//      for the next play ****

#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <cstdlib>

using namespace std;

//Struct for each node of the decision tree, with win percentages of opponent plays for next play

class randomForest_RPS_Model {

    private:

        // Nodes/"leaves" that make up the decision trees
        struct treeNode {
            int rockTimes = 0;
            int paperTimes = 0;
            int scissorsTimes = 0;
        };

        struct decisionTree {
            int depth = 0;
            vector<treeNode> decisionTree;
        };
        // Collection of nodes that make up a decision tree
        int roundsPlayed;
        int roundsWon;
        int roundsTied;
        int changeStrategyCount;
        vector<int> opponentPlays; // Stores all opponent's plays
        vector<int> agentPlays; // Stores all of the agent's own plays
        vector<decisionTree> forest; // A collection of random decision trees

    public:

        //Constructor
        randomForest_RPS_Model();

        // Accessors
        vector<int> getOpponentPlays();
        vector<int> getAgentPlays();
        int getRoundsPlayed();
        int getRoundsWon();
        int getRoundsTied();

        // Mutators
        void setRoundsPlayed(int);
        void setRoundsWon(int);
        void setRoundsTied(int);

        int getNextPlay(); // Used to return the agent's next play in the RPS game
        void teachAgent(int opponentPlay, int winLoseDraw); // Used to teach the decision tree in the model
        void printStats(); // Used to print the current win/lose statistics for the agent
};