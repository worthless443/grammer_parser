#include<regex>
#include<vector>
#include<string>
#include<utility>

#include<eval.h>

std::pair<std::vector<int>, std::vector<std::string>> parse_values(std::vector<std::string> vec) {
	std::regex r_int("[0-9]+");
	std::regex r_op("[*+-]");
	std::vector<int> vec_int;
	std::vector<std::string> vec_op;
	for(auto S : vec) {
		if(std::regex_match(S, r_int)) vec_int.push_back(std::stoi(S));
		else if(std::regex_match(S, r_op)) vec_op.push_back(S);
	}
	
	return std::make_pair(vec_int, vec_op);
}


