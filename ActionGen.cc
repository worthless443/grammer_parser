#include<iostream>
#include<vector>
#include<cstdio>
#include<utility>
#include<string>
#include<sstream>
#include<iterator>
#include<utility>
#include<regex>
#include <initializer_list>

#include<cstring>
#include<cstdlib>

#include<parser_prot.h>
#include<eval.h>
#include<stack.h>
#include<TableGen.h>
#include<ActionGen.h>

// there should be a table 
ActionsEnum action(Stack<int> stack, int word) {
	if(stack.top() ==  10) {
		return Accept;
	}
	if(stack.top()==word) return Reduce;
	else if(stack.top()!=word) return Shift;
	else return Fail;
}

ActionsEnum action_(std::vector<Item> items) {
	for(Item item : items) {
		if(item.getPlaceHolder() == 2) return Accept;
		else if(item.getPlaceHolder() == 1) return Shift;
		else if(item.getPlaceHolder() == 0) return Reduce;
		else return Fail;
	}
	return Fail;
}

ActionsEnum mapEnum(Item item) {
	if(item.getPlaceHolder() == 2) return Accept;
	else if(item.getPlaceHolder() == 1) return Reduce;
	else if(item.getPlaceHolder() == 0) return Shift;
	else return Fail;
}

ActionTable ActionGen(std::vector<std::vector<Item>> items2d) {
	ActionTable actions;
	int idx = 0;
	for(auto vec : items2d) {
		for(Item item : vec) {
			if(item.getPlaceHolder()==0) actions[idx] = Shift;
			if(item.getPlaceHolder()==1) actions[idx] = Reduce ;
			if(item.getPlaceHolder()==2) actions[idx] = Accept ;
			idx++;
		}
	}
	return actions;
}

#ifdef __main__ 
int main() {
	skLr words{1,2,4,3,5,4,1,3,5,1,4};
	//skLr(words);
}
#endif
