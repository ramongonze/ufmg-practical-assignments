#include "data.hpp"
#include "evaluate.hpp"
#include <iostream>

using namespace std;

double nDCG(Ranks predictions, Ranks answers, Graph &G2){
    double nDCG = 0;

    for(RanksIt itp = predictions.begin(); itp != predictions.end(); itp++){
        int u = itp->first;
        double dcg = 0;
        double idcg = 0;
        for(unsigned int i = 0; i < itp->second.size() && i < RANK_SIZE; i++){
            dcg += (pow(2,G2[u].neighboors[itp->second[i]])-1)/(log2(i+2));
            idcg += (pow(2,G2[u].neighboors[answers[u][i]])-1)/(log2(i+2));
        }
        if(idcg != 0)
            nDCG += dcg/idcg;
    }

    return nDCG/predictions.size();
}

Ranks getAnswers(Graph &G2, Ranks &R){
    Ranks response;
    map<int,vector<pair<double,int>>> userBooksRatings;

    for(GraphIt it = G2.begin(); it != G2.end(); it++){
        int idUser = it->first;
        if(R.find(idUser) != R.end()){
            // If the negative percentade of user's feedback is >= NEG_PERCENTAGE
            for(AdjListIt itt = it->second.neighboors.begin(); itt != it->second.neighboors.end(); itt++){
                int idBook = itt->first;
                int rating = itt->second;

                if(userBooksRatings.find(idUser) == userBooksRatings.end() ||
                   userBooksRatings[idUser].size() < RANK_SIZE ||
                   userBooksRatings[idUser][RANK_SIZE - 1].first < rating){
                    userBooksRatings[idUser].push_back(make_pair(rating, idBook));

                    sort(userBooksRatings[idUser].rbegin(),userBooksRatings[idUser].rend());
                    if(userBooksRatings[idUser].size() > RANK_SIZE){
                        userBooksRatings[idUser].pop_back();
                    }
                }
            }
        }
    }
    for(map<int,vector<pair<double,int>>>::iterator it = userBooksRatings.begin(); it != userBooksRatings.end(); it++){
        for(vector<pair<double,int>>::iterator itt = it->second.begin(); itt != it->second.end(); itt++){
            response[it->first].push_back(itt->second);
        }
    }

    return response;
}

void evaluate(Ranks R, Graph &G2){
    Ranks answers = getAnswers(G2, R);

    double ndcgValue = nDCG(R, answers, G2);

    cout << "nDCG:" << ndcgValue << '\n';
}