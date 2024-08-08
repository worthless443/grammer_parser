#include<iostream>
#include<vector>
#include<cstdio>
#include<sstream>
#include<cassert>

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
	int dstict = 0;
	for(int i=0;i<vec.size();i+=2) {
		std::vector<Item> vec_item;
		Item item(-1);
		while(true) {
			 item.make_dstict(dstict);
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
			 dstict++;
		}
		vec2d.push_back(vec_item);
	}

	return vec2d;
}

std::vector<std::vector<Item>> Iter2Vec(std::vector<std::vector<Item>>::iterator begin, std::vector<std::vector<Item>>::iterator end) {
		std::vector<std::vector<Item>> v;
		for(auto i = begin;i<end;i++) v.push_back(*i);
		return v;
}

std::vector<std::vector<Item>> iterate_vec2d(std::vector<std::vector<Item>> vec2d) {
	std::vector<std::vector<Item>> out;
	int i =0,cl_size=0,vsize=0;
	while(vec2d.size() > 2) {
		auto vtemp = Iter2Vec(vec2d.begin() , vec2d.end() - (vec2d.size()-2));
		cl_size+=closure(vtemp).size();
		out.push_back(closure(vtemp));
		vec2d = Iter2Vec(vec2d.begin() + 2, vec2d.end());
		vsize+=vec2d.size();
	}
	printf("closure end adds up to %d\n",cl_size);
	printf("vector.size() end adds up to %d\n",vsize);
	return out;
}

void decorate1(Item item) {
		for(int x : item.getContainer()) {
			if (item.getPlaceHolder() == 0) 
			printf("A(%s) -> (%d)By\n", 
					x == 10 ? "List" : "Symbol"
					, item.getLookahead());
			else if(item.getPlaceHolder() == 1) printf("A(%s) -> B(%d)y\n", 
					x == 10 ? "List" : "Symbol",
					item.getLookahead());
			else if(item.getPlaceHolder() == 2) printf("A(%s) -> By(%d)\n", 
				x == 10 ? "List" : "Symbol",
				item.getLookahead());
		}
}

void decorate2(Item item) {
	if(item.getPlaceHolder() == 0) printf("A(%s) -> (%d)By\n", 
						 item.getPlaceHolder() == 2 ? "U_List" : "U_Symbol"
						, item.getLookahead());
				else if(item.getPlaceHolder() == 1) printf("A(%s) -> B(%d)y\n", 
						 item.getPlaceHolder() == 2 ? "U_List" : "U_Symbol",
						item.getLookahead());
				else if(item.getPlaceHolder() == 2) printf("A(%s) -> By(%d)\n", 
						 item.getPlaceHolder() == 2 ? "U_List" : "U_Symbol",
						item.getLookahead());
}

void generate_derivation(std::vector<std::vector<Item>> vec2d)  {
	long int x=0, y=0,dum=0;
	std::string out;
	for(auto vec : vec2d) {
		for(Item item : vec) {
			if(item.getPlaceHolder()==1) {
				for(int x : item.getContainer())
					out = out + std::string(x==1 ? "()"  : "(") + " ";
			} 
		}
	}
	std::cout << out << "\n";
}

void vis_lr_item(std::vector<std::vector<Item>> vec2d) {
	for (auto vec : vec2d) {
		// diversify it 
		//if(vec.size()<1) break;
		for(Item item : vec) {
			if(item.getList().first == item.getList().second) {
				decorate1(item);
			}
			else decorate2(item);
		}
		printf("\n\n\n");
	}
}

// The proper way make closure(), it should take a set of list of distinct items and return a list of items with placeholder 2 which is to mean that they are recognised
// produces items with placeholder in the middle
std::vector<Item> closure(std::vector<std::vector<Item>> vec2d)  {
	std::vector<Item> items;
	int limit = 0;
	 while(1) {
		if(vec2d.size()<=1 || vec2d[0].size()<1 || vec2d[1].size()<1) break;
		for(auto each_vec : vec2d) {
			if(limit >= 1<<8) break;
			limit++;
			for(Item item : each_vec) {
				items.push_back(item);
				//if(item.getPlaceHolder()==2) items.push_back(item);
				}

			}
		vec2d = Iter2Vec(vec2d.begin() + 2, vec2d.end());
		 //if(vec2d[0][0].getContainer()[0] ==10 && vec2d[1][0].getContainer()[0] ==10) break; 
	 }

	return items;
}

template<class T>
std::vector<T> make_union_(std::vector<T> vec1, std::vector<T> &vec2) { 
	int size = vec1.size() < vec2.size() ? vec1.size() : vec2.size();
	for(auto iter=vec2.begin();iter<(vec2.begin() + 2);iter++) if(*++iter==*iter) vec1.push_back(*iter); // i does not seem to distinct since all Items seem the same
	vec2 = Iter2VecT<Item>(vec2.begin()+2, vec2.end());
	return vec1;
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
std::vector<Item> Convert2D(std::vector<std::vector<Item>> vec2d) {
	std::vector<Item> items;
	for(auto vec : vec2d) for(Item item : vec) items.push_back(item);
	return items;
}
std::vector<Item> Gt(std::vector<Item> vec) {
	std::vector<Item> temp; 
	temp.push_back(vec[vec.size() -1]);
	for(Item item : vec) {
		if(vec.size()==0) break;
		size_t prevsize = temp.size();
		auto temp_ = make_union(temp,vec);
		temp.insert(temp.end(),temp_.begin(),temp_.end());  // temp -> temp_ U vec	
		size_t cursize = temp.size();
		if(cursize>700) break; // limit to what vector can hold, not a complete construction thus
		if(cursize-prevsize == 30) break;
	}
	return temp;
}

std::vector<std::vector<Item>> build_GtTable(std::vector<std::vector<Item>> vec2d ) {
	std::vector<std::vector<Item>> out_, out;
	int count = 0;
	for(auto vec : vec2d) {
		if(vec.size()<1) {
			std::cout << "got zero values\n";
			return out;
		}
		//if(out.size()>=7) {
		//	std::cout << "Warning: Size is too big, quitting for now\n";
		//	break;
		//}
		out_.push_back(Gt(vec));
		out.push_back(closure(out_));
	}
	return out;
}

std::vector<std::vector<Item>> build_GtTable1D(std::vector<std::vector<Item>> vec2d ) {
	std::vector<std::vector<Item>> items2d, out2d;
	int count = 0;
	auto vec = Convert2D(vec2d);
	for(Item item : vec) {
		items2d.push_back(vec);
		if(item.getPlaceHolder()==2) out2d.push_back(Gt(closure(items2d)));
	}
	return out2d;
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
