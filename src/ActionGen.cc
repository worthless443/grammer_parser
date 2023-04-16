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
#include<future>

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
			else if(item.getPlaceHolder()==1) actions[idx] = Reduce ;
			else if(item.getPlaceHolder()==2) actions[idx] = Accept ;
			else actions[idx] = Fail;
			idx++;
		}
	}
	return actions;
}

void ActionStates(std::vector<int> &states, std::pair<int,int> p) {
	if(p.second == 0) states.push_back(1);
	if(p.second==1) states.push_back(1);
	if(p.second==2) states.push_back(-1);
	if(states.size()>0) {
		if(states[states.size()-1] == 1) states.push_back(0);
		if(states[states.size()-1] == 0) states.push_back(0);
	}
	else{
		states.push_back(0);
	}
	if(states[states.size()-1] == 0) states.push_back(1);
	if(states[states.size()-1] == 1) states.push_back(1);
}

std::vector<int> ActionExec(std::vector<int> vec, Stack<int> &stack, ActionTable p_vec) {
	int idx = 0;
	if(vec.size() > 0) {
		while(1) {

			if(vec[idx]==0) stack.push(3);
			else if(vec[idx]==1) stack.push(5);
			if(vec[idx]==1 && vec[idx + 1] == 1 && vec[idx+2]==-1) {
				break;
			}
			idx+=1;
		}
	}
	return Iter2VecT<int>(vec.begin() + idx + 1, vec.end());
}

int TimesAction(ActionTable p_vec) {
	//ActionTable p_vec = rep.get_future().get();
	int times = 0;
	std::vector<int> vec;
	Stack<int> stack;
	for(auto pair : p_vec) ActionStates(vec, pair);
	vec = ActionExec(vec, stack, p_vec);

	for(;vec.size()>1;++times) 
		vec = ActionExec(vec, stack,p_vec);
	
	return times;
}
#ifdef __main__ 
int main() {
	skLr words{1,2,4,3,5,4,1,3,5,1,4};
	//skLr(words);
}
#endif
