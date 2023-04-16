#include<string>
#include<iostream>
#include<vector>
#include<fmt/core.h>

#include<cstdio>
class DecorError {
private:
	std::vector<std::string> lr;
	std::vector<std::string> lr_values;
	int lr_size,lr_value_size;
	std::string mainStr;
public:
	DecorError(std::vector<std::string> lr, std::vector<std::string> lr_values) : lr(lr), lr_values(lr_values), lr_size(lr.size()), lr_value_size(lr_values.size()) {
		mainStr = decorPrint();
	}

	std::string decorPrint() {
			std::string str;
			str = fmt::format("total size of parse : (parens = {}), (values = {})", lr_size, lr_value_size);
		
			
			//std::cout << str;
			for(auto s : lr) str = str +  fmt::format("parse parens {}\n",  s);
			for(auto s : lr_values) str = str + fmt::format("parse values {}\n",  s);
//
			return str;
	}

	std::string getStr() {return mainStr;}

};

std::ostream &operator<<(std::ostream &os, DecorError &decor) {
	os << decor.getStr();
	return os;
}
