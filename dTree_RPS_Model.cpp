
// **** Implementation of the decision tree strategy. This strategy builds a decision tree and trains it on the opponent's previous two moves ****

#include "dTree_RPS_Model.h"

//Constructor
dTree_RPS_Model::dTree_RPS_Model() {
    roundsPlayed = 0;
    roundsWon = 0;
    roundsTied = 0;

    for(treeNode cur : decisionTree) {

        // These variables count the number of times these plays have been made in the given scenario
        cur.rockTimes = 0;
        cur.paperTimes = 0;
        cur.scissorsTimes = 0;
    }
}

// Accessors
int dTree_RPS_Model::getLastOpponentPlay() {
    return lastOpponentPlay;
}
int dTree_RPS_Model::getSecondLastOpponentPlay() {
    return secondLastOpponentPlay;
}
int dTree_RPS_Model::getRoundsPlayed() {
    return roundsPlayed;
}
int dTree_RPS_Model::getRoundsWon() {
    return roundsWon;
}
int dTree_RPS_Model::getRoundsTied() {
    return roundsTied;
}


// Mutators
void dTree_RPS_Model::setLastOpponentPlay(int input) {
    lastOpponentPlay = input;
}
void dTree_RPS_Model::setSecondLastOpponentPlay(int input) {
    secondLastOpponentPlay = input;
}
void dTree_RPS_Model::setRoundsPlayed(int input) {
    roundsPlayed = input;
}
void dTree_RPS_Model::setRoundsWon(int input) {
    roundsWon = input;
}
void dTree_RPS_Model::setRoundsTied(int input) {
    roundsTied = input;
}

// Used to return the agent's next play in the RPS game
int dTree_RPS_Model::getNextPlay() {
    // In case of not having previous two moves, plays random move
    if (roundsPlayed < 2) {
        roundsPlayed++;
        return rand() % 3;
    }

    roundsPlayed++;
    int toFind = lastOpponentPlay * 3 + secondLastOpponentPlay; // Finds the correct node of the decision tree based on the previous two plays
    treeNode cur = decisionTree[toFind];

    // If statements to determine what is the most likely play by opponent
    if (cur.rockTimes == cur.paperTimes && cur.paperTimes == cur.scissorsTimes) { // case of all plays equally likely, return random
        return rand() % 3;
    }
    else if (cur.rockTimes == cur.paperTimes && cur.rockTimes > cur.scissorsTimes) { // case of rock and paper being equally most likely, return either paper or scissors
        return rand() % 2 + 1;
    }
    else if (cur.paperTimes == cur.scissorsTimes && cur.paperTimes > cur.rockTimes) { // case of paper and scissors being equally most likely, return either rock or scissors
        int tempInts[2] = {0, 2};
        return tempInts[rand() % 2];
    }
    else if (cur.rockTimes == cur.scissorsTimes && cur.rockTimes > cur.paperTimes) { // case of rock and scissors being equally most likely, return either rock or paper
        return rand() % 2;
    }
    else if (cur.rockTimes > cur.paperTimes && cur.rockTimes > cur.scissorsTimes) { // case of rock being most likely, return paper
        return 1;
    }
    else if (cur.paperTimes > cur.rockTimes && cur.paperTimes > cur.scissorsTimes) { // case of paper being most likely, return scissors
        return 2;
    }
    else if (cur.scissorsTimes > cur.rockTimes && cur.scissorsTimes > cur.paperTimes) { // case of scissors being most likely, return rock
        return 0;
    }
    else { // catch all in case of oversight, return random
        return rand() % 3;
    }
}

// Used to teach the decision tree in the model
void dTree_RPS_Model::teachAgent(int opponentPlay, int winLoseDraw) {
    // Case of Agent not having 2 previous plays to use to learn. Does not update decision tree, only totals
    if (roundsPlayed < 3) {
        if(roundsPlayed == 1) {
            lastOpponentPlay = opponentPlay;
        }
        else if(roundsPlayed == 2) {
            secondLastOpponentPlay = lastOpponentPlay;
            lastOpponentPlay = opponentPlay;
        }

        if (winLoseDraw == 0) {
        roundsWon++;
        }
        else if (winLoseDraw == 1) {
            roundsTied++;
        }
        return;
    }

    int toFind = lastOpponentPlay * 3 + secondLastOpponentPlay; // Finds correct node of decision tree given previous two plays
    secondLastOpponentPlay = lastOpponentPlay;
    lastOpponentPlay = opponentPlay;
    treeNode cur = decisionTree[toFind];

    // Incrementing count of what opponent played in this scenario
    switch (opponentPlay) {
        case 0:
            cur.rockTimes++;
            break;
        case 1:
            cur.paperTimes++;
            break;
        case 2:
            cur.scissorsTimes++;
            break;
        default:
            break;
    }

    //Updating totals
    if (winLoseDraw == 0) {
        roundsWon++;
    }
    else if (winLoseDraw == 1) {
        roundsTied++;
    }
}

// Used to print the current win/lose statistics for the agent
void dTree_RPS_Model::printStats() {
    double winPercent = (double)roundsWon / (double)roundsPlayed*100;
    double tiePercent = (double)roundsTied / (double)roundsPlayed*100;
    double losePercent = ((double)(roundsPlayed - (roundsTied + roundsWon)) / (double)roundsPlayed)*100;
    cout << endl << "Round " << roundsPlayed<< endl;
    cout << "Win: " << roundsWon << "(" << winPercent << "%)" << endl;
    cout << "Tied: " << roundsTied << "(" << tiePercent << "%)" << endl;
    cout << "Lost: " << roundsPlayed-roundsWon-roundsTied << "(" << losePercent << "%)" << endl;
}