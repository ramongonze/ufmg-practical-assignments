#include "data.hpp"
#include "evaluate.hpp"

using namespace std;

float NDCG(userRanks predictions, userRanks answers){
  return -1.0;
}

float MAP(userRanks predictions, userRanks answers){
  vector<float> results;
  float sumResult = 0;

  for(userRanks::iterator itp = predictions.begin(); itp != predictions.end(); itp++){
    if(itp->second.size() > 1 && answers.find(itp->first) != answers.end() && answers[itp->first].size() > 1){
      float sum = 0;
      int correct = 0;
      int count = 0;
      int position = 1;
      int size = answers[itp->first].size();
      for(rankUser::iterator itrp = itp->second.begin(); itrp != itp->second.end(); itrp++){
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
      } else {
        results.push_back(0.0);
      }
    }
  }

  for(vector<float>::iterator it = results.begin(); it != results.end(); it++){
    sumResult += (*it);
  }

  return sumResult/results.size();
}

userRanks GetPredictions(string predictionsCsv){
  ifstream content;
	string buffer;
  userRanks response;

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

userRanks GetAnswers(string answersCsv){
  ifstream content;
	string buffer;
  userRanks response;
  map<int,vector<pair<float,int>>> userBooksRatings;

	content.open(answersCsv);

	getline(content, buffer);
	while(!content.eof()){
		getline(content, buffer);
		if(buffer.size() == 0) break;

		vector<string> tokens = split(buffer, ',');

    int book_id = stoi(tokens[0]);
    int user_id = stoi(tokens[1]);
    float rating = stod(tokens[2]);

    if(userBooksRatings.find(user_id) == userBooksRatings.end() || userBooksRatings[user_id].size() < RankSize || userBooksRatings[user_id][RankSize - 1].first < rating){
      userBooksRatings[user_id].push_back(make_pair(rating, book_id));

      if(userBooksRatings[user_id].size() >= RankSize){
        sort(userBooksRatings[user_id].rbegin(),userBooksRatings[user_id].rend());
        if(userBooksRatings[user_id].size() > RankSize){
          userBooksRatings[user_id].pop_back();
        }
      }
    }
	}

  content.close();

  for(map<int,vector<pair<float,int>>>::iterator it = userBooksRatings.begin(); it != userBooksRatings.end(); it++){
    for(vector<pair<float,int>>::iterator itt = it->second.begin(); itt != it->second.end(); itt++){
      response[it->first].push_back(itt->second);
    }
  }

  return response;
}

void evaluate(){
  userRanks Predictions = GetPredictions(PREDICTIONS);
  userRanks Answers = GetAnswers(ANSWERS);

  float ndcgValue = NDCG(Predictions, Answers);
  float mapValue = MAP(Predictions, Answers);

  //usar para conferir os ranks das respostas
  // for(userRanks::iterator it = Answers.begin(); it != Answers.end(); it++){
  //   cout << "User: " << it->first << '\n';
  //   for(rankUser::iterator itt = it->second.begin(); itt != it->second.end(); itt++){
  //     cout << "--Book: " << itt->second << " - Rating: " << itt->first << '\n';
  //   }
  // }

  if(ndcgValue < 0){
    cout << "Erro ao usar a métrica NDCG" << '\n';
  } else {
    cout << "NDCG:" << ndcgValue << '\n';
  }

  if(mapValue < 0){
    cout << "Erro ao usar a métrica MAP" << '\n';
  } else {
    cout << "MAP:" << mapValue << '\n';
  }
}
