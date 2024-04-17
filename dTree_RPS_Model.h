
// **** Header file for the decision tree strategy. This strategy builts a decision tree using the opponent's previous two plays to guess the next one ****

#include <iostream>
#include <time.h>

using namespace std;

//Struct for each node of the decision tree, with win percentages of opponent plays for next play

class dTree_RPS_Model {

    private:
        struct treeNode {
            int rockTimes = 0;
            int paperTimes = 0;
            int scissorsTimes = 0;
        };

        int lastOpponentPlay;
        int secondLastOpponentPlay;
        int roundsPlayed;
        int roundsWon;
        int roundsTied;
        struct treeNode decisionTree[9];

    public:

        //Constructor
        dTree_RPS_Model();

        // Accessors
        int getLastOpponentPlay();
        int getSecondLastOpponentPlay();
        int getRoundsPlayed();
        int getRoundsWon();
        int getRoundsTied();

        // Mutators
        void setLastOpponentPlay(int);
        void setSecondLastOpponentPlay(int);
        void setRoundsPlayed(int);
        void setRoundsWon(int);
        void setRoundsTied(int);

        int getNextPlay(); // Used to return the agent's next play in the RPS game
        void teachAgent(int opponentPlay, int winLoseDraw); // Used to teach the decision tree in the model
        void printStats(); // Used to print the current win/lose statistics for the agent
};