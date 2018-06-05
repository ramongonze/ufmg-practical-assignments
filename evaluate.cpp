#include "data.hpp"
#include "evaluate.hpp"
#include <iostream>

using namespace std;

double NDCG(Ranks predictions, Ranks answers, Graph &G2){
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

double MAP(Ranks predictions, Ranks answers){
    try{
        vector<double> results;
        double sumResult = 0;

    for(RanksIt itp = predictions.begin(); itp != predictions.end(); itp++){
        if(itp->second.size() > 1 && answers.find(itp->first) != answers.end() && answers[itp->first].size() > 1){
            double sum = 0;
            int correct = 0;
            int count = 0;
            int position = 1;
            int size = answers[itp->first].size();
            for(UserRank::iterator itrp = itp->second.begin(); itrp != itp->second.end(); itrp++){
                if(size >= count){
                    if((*itrp) == answers[itp->first][count]){ //confere se o ranking retornado acertou nessa posição
                        correct++;
                        sum += (double)correct/position;
                    }
                }
                count++;
                position++;
            }

            if(correct > 0){
                results.push_back(sum/correct);
            }else{
                results.push_back(0.0);
            }
        }
    }

    for(vector<double>::iterator it = results.begin(); it != results.end(); it++){
        sumResult += (*it);
    }

    return sumResult/results.size();

    }catch(int e){
        return -1.0;
    }
}

Ranks GetAnswers(Graph &G2){
    Ranks response;
    map<int,vector<pair<double,int>>> userBooksRatings;

    for(GraphIt it = G2.begin(); it != G2.end(); it++){
        int idUser = it->first;
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

    for(map<int,vector<pair<double,int>>>::iterator it = userBooksRatings.begin(); it != userBooksRatings.end(); it++){
        for(vector<pair<double,int>>::iterator itt = it->second.begin(); itt != it->second.end(); itt++){
            response[it->first].push_back(itt->second);
        }
    }

    return response;
}

void evaluate(Ranks R, Graph &G2){
    Ranks Answers = GetAnswers(G2);

    double ndcgValue = NDCG(R, Answers, G2);
    double mapValue = MAP(R, Answers);

    //usar para conferir os ranks das respostas
    // for(RanksIt it = Answers.begin(); it != Answers.end(); it++){
    //   cout << "User: " << it->first << '\n';
    //   for(UserRank::iterator itt = it->second.begin(); itt != it->second.end(); itt++){
    //     cout << (*itt) << ",";
    //   }
    //   cout << '\n';
    // }

    if(ndcgValue < 0){
        cout << "Erro ao usar a métrica NDCG" << '\n';
    }else{
        cout << "NDCG:" << ndcgValue << '\n';
    }

    if(mapValue < 0){
        cout << "Erro ao usar a métrica MAP" << '\n';
    }else{
        cout << "MAP:" << mapValue << '\n';
    }
}
