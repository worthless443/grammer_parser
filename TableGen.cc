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
			 if(vec[i] != vec[i+1]) {
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

std::vector<std::vector<Item>> Iter2Vec(std::vector<std::vector<Item>>::iterator begin, std::vector<std::vector<Item>>::iterator end) {
		std::vector<std::vector<Item>> v;
		for(auto i = begin;i<end;i++) v.push_back(*i);
		return v;
	};


void decorate1(Item item) {
	
	if(item.getPlaceHolder() == 0) 
		printf("A(%s) -> (%d)By\n", 
				item.getContainer()[0] == 10 ? "List" : "Symbol"
				, item.getPlaceHolder());
		else if(item.getPlaceHolder() == 1) printf("A(%s) -> B(%d)y\n", 
				item.getContainer()[0] == 10 ? "List" : "Symbol",
				item.getPlaceHolder());
		else if(item.getPlaceHolder() == 2) printf("A(%s) -> By(%d)\n", 
					item.getContainer()[0] == 10 ? "List" : "Symbol",
					item.getPlaceHolder());
}

void decorate2(Item item) {
	if(item.getPlaceHolder() == 0) printf("A(%s) -> (%d)By\n", 
						 "U_Symbol"
						, item.getPlaceHolder());
				else if(item.getPlaceHolder() == 1) printf("A(%s) -> B(%d)y\n", 
						 "U_Symbol",
						item.getPlaceHolder());
				else if(item.getPlaceHolder() == 2) printf("A(%s) -> By(%d)\n", 
						"U_Symbol",
						item.getPlaceHolder());
}

void vis_lr_item(std::vector<std::vector<Item>> vec2d) {
	for (auto vec : vec2d) {
		// diversify it 
		if(vec.size()<1) break;
		for(Item item : vec) {
			if(item.getList().first == item.getList().second) {
				decorate1(item);
			}
			else decorate2(item);
		}
		printf("\n\n\n");
	}
}

// produces items with placeholder in the middle
std::vector<Item> closure(std::vector<std::vector<Item>> vec2d)  {
	std::vector<Item> items;
	while(vec2d[0][0].getContainer()[0] !=10 && vec2d[1][0].getContainer()[0] !=10) {
		vec2d = Iter2Vec(vec2d.begin() + 2, vec2d.end());
		if(vec2d.size()<=3 || vec2d[0].size()<2 || vec2d[1].size()<2) break;
		for(auto each_vec : vec2d) {
			for(Item item : each_vec) {
				if(item.getPlaceHolder()==1) items.push_back(item);
				}
			}
		}

	return items;
}

template<class T>
std::vector<T> make_union(std::vector<T> vec1, std::vector<T> vec2) {
	int size = vec1.size() < vec2.size() ? vec1.size() : vec2.size();
	//std::cout << size << "\n";
	for(int i=0;i<size;i++) {
		if(vec1[i]!=vec2[i]) { // the placeholder always equal, id understand why
			vec1.erase(vec1.begin() + size, vec1.end());
			vec1.push_back(vec2[i]);
			vec1.push_back(vec1[i]);
		}
		else vec1.push_back(vec2[i]);
	}
	return vec1;
}

std::vector<Item> Gt(std::vector<Item> vec) {
	std::vector<Item> temp = {vec[3]}; // a random index to start at
	for(Item item : vec) {
		size_t prevsize = temp.size();
		temp = make_union(temp,vec);  // temp U vec	
		size_t cursize = temp.size();
		if(cursize-prevsize == 30) break;
	}
	return temp;
}

std::vector<std::vector<Item>> build_GtTable(std::vector<std::vector<Item>> vec2d ) {
	std::vector<std::vector<Item>> out;
	for(auto vec : vec2d) out.push_back(Gt(vec));
	return out;
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
