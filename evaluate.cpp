#include "data.hpp"
#include "evaluate.hpp"

using namespace std;

float NDCG(string predictionsCsv, answers a){
  return -1.0;
}

float MAP(string predictionsCsv, answers a){
  return -1.0;
}

answers GetAnswers(string answersCsv){
  ifstream content;
	string buffer;
  answers response;

	content.open(answersCsv);

	getline(content, buffer);
	while(!content.eof()){
		getline(content, buffer);
		if(buffer.size() == 0) break;

		vector<string> tokens = split(buffer, ',');

    int book_id = stoi(tokens[0]);
    int user_id = stoi(tokens[1]);
    float rating = stod(tokens[2]);

    if(response.find(user_id) == response.end() || response[user_id].size() < RankSize || response[user_id][RankSize - 1].first < rating){
      response[user_id].push_back(make_pair(rating, book_id));

      if(response[user_id].size() >= RankSize){
        sort(response[user_id].rbegin(),response[user_id].rend());
        if(response[user_id].size() > RankSize){
          response[user_id].pop_back();
        }
      }
    }
	}

  content.close();

  return response;
}

void evaluate(){
  string predictionsCsv = PREDICTIONS;
  string answersCsv = ANSWERS;

  answers Answers = GetAnswers(answersCsv);

  float ndcgValue = NDCG(predictionsCsv, Answers);
  float mapValue = MAP(predictionsCsv, Answers);

  //usar para conferir os ranks das respostas
  // for(answers::iterator it = Answers.begin(); it != Answers.end(); it++){
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
