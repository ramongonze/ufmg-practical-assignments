#include "predict.h"

void readContent(movieContent &movies, string file_content){
	string buffer;
	ifstream content;
	Document d;

	content.open(file_content.c_str());

	getline(content, buffer); // Ignores the header

	while(!content.eof()){
		getline(content, buffer);
		if(buffer.size() == 0 || buffer[0] == '\n'){
			break;
		}

		string id = buffer.substr(0,8);
		string desc = buffer.substr(9); 
		d.Parse(desc.c_str());
		
		if(!d.HasMember("Error")){
			for(Features f = d.MemberBegin(); f != d.MemberEnd(); f++){
				movies[id][f->name.GetString()] = f->value.GetString();
			}
		}
	}
}