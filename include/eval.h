#include<utility>
#include<regex>
#include<vector>
#include<string>

#pragma once 

template<class T>
concept ShouldPair = requires(T a) {
	{a} -> std::pair;
};

std::pair<std::vector<int>, std::vector<std::string>> parse_values(std::vector<std::string> vec);

template<ShouldPair T> int eval_main(T pair) {
	auto first = pair.first;
	auto second = pair.second;
	int res = -1;
	for(int i=0;i<second.size();i++)  {
		if(second[i]  == "+")  res += first[i] + first[i+1];
		else if(second[i]  == "-")  res -= first[i] - first[i+1];
		else if(second[i]  == "*")  res *= first[i] * first[i+1];
	}

	return res;
}

	
