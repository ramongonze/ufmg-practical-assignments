#include "data.hpp"
#include "evaluate.hpp"

using namespace std;

float NDCG(Ranks predictions, Ranks answers){
  try{
    vector<float> results;
    float sumResult = 0;

    for(Ranks::iterator itp = predictions.begin(); itp != predictions.end(); itp++){
        if(itp->second.size() > 1 && answers.find(itp->first) != answers.end() && answers[itp->first].size() > 1){
            int dcg = 0;
            float idcg = 0;
            int i = 0;

            for(UserRank::iterator itrp = itp->second.begin(); itrp != itp->second.end(); itrp++){
                 dcg += (*itrp)/log2((i++) + 1);
            }

            i = 0;
            for(UserRank::iterator itrp = answers[itp->first].begin(); itrp != answers[itp->first].end(); itrp++){
               idcg += (*itrp)/log2((i++) + 1);
            }

            if(dcg > 0 && idcg > 0){
                 results.push_back(dcg/idcg);
            }else{
                results.push_back(0.0);
            }
        }
    }

    for(vector<float>::iterator it = results.begin(); it != results.end(); it++){
        sumResult += (*it);
    }

    return sumResult/results.size();
    
    }catch(int e){
        return -1.0;
    }
}

float MAP(Ranks predictions, Ranks answers){
    try{
        vector<float> results;
        float sumResult = 0;

    for(Ranks::iterator itp = predictions.begin(); itp != predictions.end(); itp++){
        if(itp->second.size() > 1 && answers.find(itp->first) != answers.end() && answers[itp->first].size() > 1){
            float sum = 0;
            int correct = 0;
            int count = 0;
            int position = 1;
            int size = answers[itp->first].size();
            for(UserRank::iterator itrp = itp->second.begin(); itrp != itp->second.end(); itrp++){
                if(size >= count){
                    if((*itrp) == answers[itp->first][count]){ //confere se o ranking retornado acertou nessa posição
                        correct++;
                        sum += (float)correct/position;
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

    for(vector<float>::iterator it = results.begin(); it != results.end(); it++){
        sumResult += (*it);
    }

    return sumResult/results.size();
    
    }catch(int e){
        return -1.0;
    }
}

Ranks GetPredictions(string predictionsCsv){
    ifstream content;
    string buffer;
    Ranks response;

    content.open(predictionsCsv);

    getline(content, buffer);
    while(!content.eof()){
        getline(content, buffer);
        if(buffer.size() == 0) break;

        vector<string> tokens = split(buffer, ',');

        if(tokens.size() > 0){
            for(int i = 1, max = tokens.size(); i < max; i++){
                response[stoi(tokens[0])].push_back(stoi(tokens[i]));
            }
        }
    }

    content.close();

    return response;
}   

Ranks GetAnswers(Graph *graph_answers){
    Ranks response;
    map<int,vector<pair<float,int>>> userBooksRatings;

    for(GraphIt it = (*graph_answers).begin(); it != (*graph_answers).end(); it++){
        int idUser = it->first;
        for(AdjListIt itt = it->second.neighboors.begin(); itt != it->second.neighboors.end(); itt++){
            int idBook = itt->first;
            int rating = itt->second;

            if(userBooksRatings.find(idUser) == userBooksRatings.end() || 
               userBooksRatings[idUser].size() < RANK_SIZE ||
               userBooksRatings[idUser][RANK_SIZE - 1].first < rating){
                userBooksRatings[idUser].push_back(make_pair(rating, idBook));

                if(userBooksRatings[idUser].size() >= RANK_SIZE){
                    sort(userBooksRatings[idUser].rbegin(),userBooksRatings[idUser].rend());
                    if(userBooksRatings[idUser].size() > RANK_SIZE){
                        userBooksRatings[idUser].pop_back();
                    }
                }
            }
        }
    }

    for(map<int,vector<pair<float,int>>>::iterator it = userBooksRatings.begin(); it != userBooksRatings.end(); it++){
        for(vector<pair<float,int>>::iterator itt = it->second.begin(); itt != it->second.end(); itt++){
            response[it->first].push_back(itt->second);
        }
    }

    return response;
}

void evaluate(Graph *graph_answers){
    Ranks Predictions = GetPredictions(PREDICTIONS);
    Ranks Answers = GetAnswers(graph_answers);

    float ndcgValue = NDCG(Predictions, Answers);
    float mapValue = MAP(Predictions, Answers);

    //usar para conferir os ranks das respostas
    // for(Ranks::iterator it = Answers.begin(); it != Answers.end(); it++){
    //   cout << "User: " << it->first << '\n';
    //   for(UserRank::iterator itt = it->second.begin(); itt != it->second.end(); itt++){
    //     cout << "--Book: " << itt->second << " - Rating: " << itt->first << '\n';
    //   }
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