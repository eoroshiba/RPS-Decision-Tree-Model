#include "dTree_RPS_Model.h"
#include "randomForest_RPS_Model.h"

int main() {
    srand(time(NULL));
    randomForest_RPS_Model RDF;
    dTree_RPS_Model dTree;
    int gamesToPlay;
    int outputFreq;
    cout << "How many games would you like to play?" << endl;
    cin >> gamesToPlay;
    cout << "Every how many games should the score be output?" << endl;
    cin >> outputFreq;
    for(int i = 1; i <= gamesToPlay; i++) {
        int dTreePlay = dTree.getNextPlay();
        // cout << "Enter next play:" << endl;
        // int dTreePlay;
        // cin >> dTreePlay;
        int rdfPlay = RDF.getNextPlay();
        if (dTreePlay == rdfPlay) {
            RDF.teachAgent(dTreePlay, 1);
            dTree.teachAgent(rdfPlay, 1);
        }
        else if ((dTreePlay == 0 && rdfPlay == 2) || (dTreePlay == 1 && rdfPlay == 0) || (dTreePlay == 2 && rdfPlay == 1)) {
            RDF.teachAgent(dTreePlay, 2);
            dTree.teachAgent(rdfPlay, 0);
        }
        else {
            dTree.teachAgent(rdfPlay, 2);
            RDF.teachAgent(dTreePlay, 0);
        }

        if(i % outputFreq == 0) {
            cout << endl << "Decision Tree: " << endl;
            dTree.printStats();
            cout << endl << "RDF: " << endl;
            RDF.printStats();
        }
    }
    cout << endl;
    if (dTree.getRoundsWon() > RDF.getRoundsWon()) {
        cout << "Winning Algorithm: Decision Tree" << endl;
        cout << "Win Percentage: " << ((double)dTree.getRoundsWon()/(double)dTree.getRoundsPlayed()*100) << "%" << endl;
        cout << "Tie Percentage: " << ((double)dTree.getRoundsTied()/(double)dTree.getRoundsPlayed()*100) << "%" << endl;
        cout << endl;
        cout << "Losing Algorithm: Random Decision Forest" << endl;
        cout << "Win Percentage: " << ((double)RDF.getRoundsWon()/(double)RDF.getRoundsPlayed()*100) << "%" << endl;
        cout << "Tie Percentage: " << ((double)RDF.getRoundsTied()/(double)RDF.getRoundsPlayed()*100) << "%" << endl;
    }
    else if (dTree.getRoundsWon() < RDF.getRoundsWon()) {
        cout << "Winning Algorithm: Random Decision Forest" << endl;
        cout << "Win Percentage: " << ((double)RDF.getRoundsWon()/(double)RDF.getRoundsPlayed()*100) << "%" << endl;
        cout << "Tie Percentage: " << ((double)RDF.getRoundsTied()/(double)RDF.getRoundsPlayed()*100) << "%" << endl;
        cout << endl;
        cout << "Losing Algorithm: Decision Tree" << endl;
        cout << "Win Percentage: " << ((double)dTree.getRoundsWon()/(double)dTree.getRoundsPlayed()*100) << "%" << endl;
        cout << "Tie Percentage: " << ((double)dTree.getRoundsTied()/(double)dTree.getRoundsPlayed()*100) << "%" << endl;
    }
    else {
        cout << "It's a tie!" << endl;
    }
    system("pause");
    return 0;
}