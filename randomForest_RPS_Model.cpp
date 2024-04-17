
// **** Implementation of the random decision forest (RDF) strategy. This strategy uses a collection of randomly created/trained decision trees, and takes the consensus among them 
//      for the next play ****

#include "randomForest_RPS_Model.h"

//Constructor
randomForest_RPS_Model::randomForest_RPS_Model() {
    roundsPlayed = 0;
    roundsWon = 0;
    roundsTied = 0;
    changeStrategyCount = 0;
}

// Accessors
vector<int> randomForest_RPS_Model::getOpponentPlays() {
    return opponentPlays;
}
vector<int> randomForest_RPS_Model::getAgentPlays() {
    return agentPlays;
}
int randomForest_RPS_Model::getRoundsPlayed() {
    return roundsPlayed;
}
int randomForest_RPS_Model::getRoundsWon() {
    return roundsWon;
}
int randomForest_RPS_Model::getRoundsTied() {
    return roundsTied;
}

// Mutators
void randomForest_RPS_Model::setRoundsPlayed(int input) {
    roundsPlayed = input;
}
void randomForest_RPS_Model::setRoundsWon(int input) {
    roundsWon = input;
}
void randomForest_RPS_Model::setRoundsTied(int input) {
    roundsTied = input;
}


// Used to return the agent's next play in the RPS game
int randomForest_RPS_Model::getNextPlay() {
    roundsPlayed++;
    if(forest.size() == 0) { // If no decision trees made yet, returns random play
        return rand() % 3;
    }

    vector<int> consensusCounts; // Vector for counting the votes of all decision trees in the forest

    for(decisionTree tempTree : forest) { // For loop to iterate through each tree in the forest
        int depth = tempTree.depth;
        int toFind = 0;
        int powerInt = depth - 1;
        int decrementCounter = 1;
        int toAddToConsensus;
        for(int i = 0; i < depth; i++) { // For loop finds correct decision tree node given the opponent's previous plays
            toFind += opponentPlays[opponentPlays.size() - decrementCounter] * pow(3, powerInt); 
            powerInt--;
            decrementCounter++;
        }
        treeNode cur = tempTree.decisionTree[toFind];

        // If statements to determine what is the most likely play by opponent
        if (cur.rockTimes == cur.paperTimes && cur.paperTimes == cur.scissorsTimes && cur.rockTimes != 0) { // case of all plays equally likely, vote for random
            toAddToConsensus = rand() % 3;
        }
        else if (cur.rockTimes == cur.paperTimes && cur.rockTimes > cur.scissorsTimes) { // case of rock and paper being equally most likely, vote for paper or scissors
            toAddToConsensus = rand() % 2 + 1;
        }
        else if (cur.paperTimes == cur.scissorsTimes && cur.paperTimes > cur.scissorsTimes) { // case of paper and scissors being equally most likely, vote for rock or rcissors
            toAddToConsensus = (rand() % 2) == 0? 0 : 2;
        }
        else if (cur.rockTimes == cur.scissorsTimes && cur.rockTimes > cur.paperTimes) { // case of rock and scissors being equally most likely, vote for rock or paper
            toAddToConsensus = rand() % 2;
        }
        else if (cur.rockTimes > cur.paperTimes && cur.rockTimes > cur.scissorsTimes) { // case of rock being most likely, vote for paper
            toAddToConsensus = 1;
        }
        else if (cur.paperTimes > cur.rockTimes && cur.paperTimes > cur.scissorsTimes) { // case of paper being most likely, vote for scissors
            toAddToConsensus =  2;
        }
        else if (cur.scissorsTimes > cur.rockTimes && cur.scissorsTimes > cur.paperTimes) { // case of scissors being most likely, vote for rock
            toAddToConsensus =  0;
        }
        // else { // catch all in case of oversight, vote for random
        //     toAddToConsensus =  rand() & 3;
        // }

        consensusCounts.push_back(toAddToConsensus);
    }

    // Integers for tallying votes among decision trees
    int rockCount = 0;
    int paperCount = 0;
    int scissorsCount = 0;

    // For loop counts votes among decision trees
    for(int i : consensusCounts) {
        if (i == 0) rockCount++;
        else if (i == 1) paperCount++;
        else if (i == 2) scissorsCount++;
        else continue;
    }

    // vector<int> counts {rockCount, paperCount, scissorsCount};
    // for(int i : counts) {
    //     for(int j : counts) {
    //         if(i != j) {
    //             if(std::abs(i-j) < (i/3) && i != 0 && j != 0) {
    //                 forest.resize(0);
    //                 cout << "Strategy Change" << endl;
    //             }
    //         }
    //     }
    // }

    int play;

    if (rockCount == paperCount && paperCount == scissorsCount) { // Votes for all plays equal, return random
        play = rand() % 3;
    }
    else if (rockCount == paperCount && rockCount > scissorsCount) { // Equally most votes for rock and paper, return either
        play = rand() % 2;
    }
    else if (paperCount == scissorsCount && paperCount > rockCount) { // Equally most votes for paper and scissors, return either
        play = rand() % 2 + 1;
    }
    else if (rockCount == scissorsCount && rockCount > paperCount) { // Equally most votes for rock and scissors, return either
        int tempInts[2] = {0, 2};
        play = tempInts[rand() % 2];
    }
    else if (rockCount > paperCount && rockCount > scissorsCount) { // Most votes for rock, return rock
        play = 0;
    }
    else if (paperCount > rockCount && paperCount > scissorsCount) { // Most votes for paper, return paper
        play = 1;
    }
    else if (scissorsCount > rockCount && scissorsCount > paperCount) { // Most votes for scissors, return scissors
        play = 2;
    }
    else { // catch all in case of oversight, return random
        play = rand() % 3;
    }

    agentPlays.push_back(play);
    return play;
}

// Used to teach the decision tree in the model
void randomForest_RPS_Model::teachAgent(int opponentPlay, int winLoseDraw) {
    if (roundsPlayed < 3) { // If less than 3 rounds have been played, no decision tree made
        opponentPlays.push_back(opponentPlay);
        if (winLoseDraw == 0) {
            roundsWon++;
        }
        else if (winLoseDraw == 1) {
            roundsTied++;
        }
        return;
    }

    else if (roundsPlayed == 3 || forest.size() == 0) { // If only three rounds have been played, creates and updates new decision tree of depth 2
        decisionTree tempTree;
        tempTree.depth = 2;
        treeNode tempNode;

        if (opponentPlay == 0) {
            tempNode.rockTimes++;
        }
        else if (opponentPlay == 1) {
            tempNode.paperTimes++;
        }
        else if (opponentPlay == 2) {
            tempNode.scissorsTimes++;
        }

        int toPut = opponentPlays.back() * 3 + opponentPlays[opponentPlays.size()-2];
        opponentPlays.push_back(opponentPlay);

        for(int i = 0; i < toPut; i++) {
            treeNode randoNode;
            tempTree.decisionTree.push_back(randoNode);
        }
        tempTree.decisionTree.push_back(tempNode);
        for(int i = toPut + 1; i < 9; i++) {
            treeNode randoNode;
            tempTree.decisionTree.push_back(randoNode);
        }

        forest.push_back(tempTree);

        if (winLoseDraw == 0) {
            roundsWon++;
        }
        else if (winLoseDraw == 1) {
            roundsTied++;
        }
        return;
    }

    else if (roundsPlayed < 5) { // If less than 5 rounds have been played, updates only decision tree
        int toFind = opponentPlays.back() * 3 + opponentPlays[opponentPlays.size()-2];
        decisionTree curTree = forest.back();
        treeNode cur = curTree.decisionTree[toFind];

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
        opponentPlays.push_back(opponentPlay);
        if (winLoseDraw == 0) {
            roundsWon++;
        }
        else if (winLoseDraw == 1) {
            roundsTied++;
        }
        return;
    }

    else if (roundsPlayed % 5 == 0 && roundsPlayed > 10) { // Every 5 rounds, creates new decision tree of random depth (<5) trained on random set of opponent's plays and adds to forest

        int newTreeDepth = rand() % 3 + 1;
        int randTreeStart = (rand() % (opponentPlays.size() - newTreeDepth - 1)) + newTreeDepth;
        decisionTree treeToAdd;
        treeToAdd.depth = newTreeDepth;

        int numOfNodes = pow(3,newTreeDepth);
        for (int i = 0; i < numOfNodes; i++) {
            treeNode nodeToAdd;
            treeToAdd.decisionTree.push_back(nodeToAdd);
        }

        for (int i = randTreeStart; i < newTreeDepth; i++) {
            int powerInt = newTreeDepth - 1;
            int decrementCounter = 1;
            int toPut = 0;

            for (int j = 0; j < newTreeDepth; j++) {
                toPut += opponentPlays[i - decrementCounter] * pow(3,powerInt);
                powerInt--;
                decrementCounter++;
            }

            if (opponentPlays[i] == 0) {
                treeToAdd.decisionTree[toPut].rockTimes++;
            }
            else if (opponentPlays[i] == 1) {
                treeToAdd.decisionTree[toPut].paperTimes++;
            }
            else if (opponentPlays[i] == 2) {
                treeToAdd.decisionTree[toPut].scissorsTimes++;
            }
        }

        forest.push_back(treeToAdd);
        opponentPlays.push_back(opponentPlay);
        if (winLoseDraw == 0) {
            roundsWon++;
        }
        else if (winLoseDraw == 1) {
            roundsTied++;
        }
        return;
    }

    else { // Trains random tree in the forest

        decisionTree treeToUpdate = forest[rand() % forest.size()];
        int nodeToUpdate = 0;
        int powerInt = treeToUpdate.depth - 1;
        int decrementCounter = 1;
        
        for (int i = 0; i < treeToUpdate.depth; i++) {
            nodeToUpdate += opponentPlays[opponentPlays.size() - decrementCounter] * pow(3,powerInt);
            powerInt--;
            decrementCounter++;
        }
        
        treeNode toUpdate = treeToUpdate.decisionTree[nodeToUpdate];
        if(toUpdate.rockTimes == 0 && toUpdate.paperTimes == 0 && toUpdate.scissorsTimes == 0) {
            changeStrategyCount++;
        }

        if (opponentPlay == 0) {
            toUpdate.rockTimes++;
        }
        else if (opponentPlay == 1) {
            toUpdate.paperTimes++;
        }
        else if (opponentPlay == 2) {
            toUpdate.scissorsTimes++;
        }

        if(changeStrategyCount > 10) {
            forest.resize(0);
            changeStrategyCount = 0;
            forest.push_back(treeToUpdate);
        }

        opponentPlays.push_back(opponentPlay);
        if (winLoseDraw == 0) {
            roundsWon++;
        }
        else if (winLoseDraw == 1) {
            roundsTied++;
        }
        return;
    }
}

// Used to print the current win/lose statistics for the agent
void randomForest_RPS_Model::printStats() {
    double winPercent = ((double)roundsWon / (double)roundsPlayed)*100;
    double tiePercent = (double)roundsTied / (double)roundsPlayed*100;
    double losePercent = ((double)(roundsPlayed - (roundsTied + roundsWon)) / (double)roundsPlayed)*100;
    cout << endl << "Round " << roundsPlayed << endl;
    cout << "Win: " << roundsWon << "(" << winPercent << "%)" << endl;
    cout << "Tied: " << roundsTied << "(" << tiePercent << "%)" << endl;
    cout << "Lost: " << roundsPlayed-roundsWon-roundsTied << "(" << losePercent << "%)" << endl;
} 