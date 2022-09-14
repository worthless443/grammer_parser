#include<iostream>
#include<vector>
#include<cstdio>
#include<sstream>
#include<cassert>

#include<stack.h>
#include<TableGen.h>

// convert into a hashable type

int generic_arraySearch(std::vector<int> vec, int what) {
	int idx = 0;
	for(int i : vec) {
		idx++;
		if(i == what) return idx;
	}
	return -1;
}

Stack<States<int>> create_stack(std::vector<int> vec) {
	Stack<States<int>> stack;
	States<int> states;
	//create the stack
	for(auto each : vec) {
		states.stackUp(each);
		stack.push(states);
	}
	return stack;
}
// templated functions don't work in header where impl in source 
std::vector<int> create_identity(Stack<States<int>> stack) {
	// create identity
	std::vector<int> identity;
	int i=0;
	while(stack.size()>0) {
		stack++;
		auto _stack = stack+1;
		auto cur_vec = stack.top().getVector();
		auto next_vec = _stack.top().getVector();
		if(next_vec.size() > 0) {
			if(cur_vec[cur_vec.size()-1] == next_vec[next_vec.size()-1])
				identity.push_back(1);
			else {
				identity.push_back(cur_vec[cur_vec.size()-1]);
			}
		}
		
	} 
	return identity;
}

std::vector<std::vector<Item>> process(std::vector<int> vec)  {
	std::vector<std::vector<Item>> vec2d;
	for(int i=0;i<vec.size();i+=2) {
		std::vector<Item> vec_item;
		Item item(-1);
		while(true) {
			 if(vec[i] == vec[i+1]) {
				item.stackUpCurrent(10);
				item.makeList(vec[i],vec[i+1]);
				vec_item.push_back(item);
			 }
			
			 else {
				item.stackUpCurrent(vec[i]);
				item.stackUpCurrent(vec[i+1]);
				vec_item.push_back(item);
			 }
			 item.placehold();
			 if(item.getPlaceHolder()==3) break;
		}
		vec2d.push_back(vec_item);
	}

	return vec2d;
}

void vis_lr_item(std::vector<std::vector<Item>> vec2d) {
	for (auto vec : vec2d) {
		for(Item item : vec) {
				if(item.getPlaceHolder() == 0) printf("A(%s) -> (%d)By\n", 
						item.getContainer()[0] == 10 ? "List" : "Symbol"
						, item.getPlaceHolder());
				else if(item.getPlaceHolder() == 1) printf("A(%s) -> B(%d)y\n", 
						item.getContainer()[0] == 10 ? "List" : "Symbol",
						item.getPlaceHolder());
				else if(item.getPlaceHolder() == 2) printf("A(%s) -> By(%d)\n", 
						item.getContainer()[0] == 10 ? "List" : "Symbol",
						item.getPlaceHolder());
		}
		printf("\n\n\n");
	}
}

#ifdef __main__ 
int main() {
	std::vector<int> vec = {3,3,7,3,7,3,3,3,7,3,3,3,3,7,3,3,7,3,7,3};
	assert(vec.size()%2==0);
	auto stack = create_stack(vec);
	auto vi = create_identity(stack);
	auto items = process(vi);
//	//std::cout << items.size();
//	for(auto vec : items) {
//		for(Item item : vec) 
//			item.print();
//		std::cout << "\n\n";
//	}
		
	vis_lr_item(items);
	//
	//for(Item item : items) item.print();
	//for(int i :vi) std::cout << i << "\n";
	//vis_lr_item(vec2d);
	//Production prod(1,1);
}
#endif
