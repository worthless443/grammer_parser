#include<iostream>
#include<vector>
#include<cstdio>
#include<sstream>

#include<stack.h>

// convert into a hashable type

template<class T>
concept VecType = requires(T a ) {
	{a} -> std::convertible_to<std::vector<int>>;
};

template<class T>
concept StackType = requires(T a ) {
	{a} -> std::convertible_to<Stack<States<int>>>;
};


class Item {
	int placeholder  = -1; // has not been set yet
	struct List {int a, b;};
	struct Production {
		int direction; // 0 means [A -> By], 1 means the opposite
		List list;
		int lookahead;
	};
	Production prod;
public:
	Item(int lookahead, int direction=0) :  prod{.direction = direction, .lookahead = lookahead}, placeholder(0) {}
	void placehold() {
		switch(placeholder) {
			case -1:
				placeholder = 0;
				break;
			case 0:
				placeholder = 1;
				break;
			case 1:
				placeholder = 2;
				break;
			case 2:
				placeholder = 3;
				break;
			default:
				placeholder = -1;
				break;
		}
	}

	int getPlaceHolder() {return placeholder;}
	int getLookahead() {return prod.lookahead;}
	void reduce() { 
		prod.lookahead = 10; //  resembles a List
	}
};


int generic_arraySearch(std::vector<int> vec, int what) {
	int idx = 0;
	for(int i : vec) {
		idx++;
		if(i == what) return idx;
	}
	return -1;
}

template<VecType T>
auto create_stack(T vec) {
	Stack<States<int>> stack;
	States<int> states;
	//create the stack
	for(auto each : vec) {
		states.stackUp(each);
		stack.push(states);
	}
	return stack;
}

template<StackType T>
auto create_identity(T stack) {
	// create identity
	std::vector<int> identity;
	int i=0;
	while(stack.size()>1) {
		stack++;
		for(auto s : stack.top().getVector()) {
			auto _stack = stack+1;
			identity.push_back(generic_arraySearch(_stack.top().getVector(), s));
		}
	} 
	return identity;
}
template<VecType V> 
auto process(V vec)  {
	std::vector<Item> vec_item;
	for(int i=0;i<vec.size();i+=2) {
		 if(vec[i]==vec[i+1]) {
		 	Item item(10);
			vec_item.push_back(item);
		 }
		
		 else {
		 	Item item1(vec[i]);
		 	Item item2(vec[i+1]);
			vec_item.push_back(item1);
			vec_item.push_back(item2);
		 }
	}

	return vec_item;
}

auto vis_lr_item(std::vector<std::vector<Item>> vec2d) {
	for (auto vec : vec2d) {
		for(Item item : vec) {
			if(item.getPlaceHolder() == 0) printf("A(%d) -> (%d)By\n", item.getLookahead(), item.getPlaceHolder());
			else if(item.getPlaceHolder() == 1) printf("A(%d) -> B(%d)y\n", item.getLookahead(), item.getPlaceHolder());
			else if(item.getPlaceHolder() == 2) printf("A(%d) -> By(%d)\n", item.getLookahead(), item.getPlaceHolder());
			else printf("(end)\n");
		}
		printf("\n\n");
	}
}

int main() {
	std::vector<int> vec = {3,7,3,7,3,3,7};
	auto stack = create_stack(vec);
	auto vi = create_identity(stack);
	auto items = process(vi);
	for(Item item : items) std::cout << item.getLookahead() << "\n";

	//for(int i :vi) std::cout << i << "\n";
	//vis_lr_item(vec2d);
	//Production prod(1,1);
}
