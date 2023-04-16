#include<iostream>
#include<vector>
#include<cstdio>
#include<utility>
#include<string>
#include<sstream>
#include<iterator>
#include<utility>
#include<regex>
#include<thread>
#include<functional>
#include<future>

#include<cstring>
#include<cstdlib>

#include<parser_prot.h>
#include<eval.h>
#include<stack.h>
#include<TableGen.h>
#include<ActionGen.h>

#include<fmt/core.h>

std::vector<Item> Iter2Vec_Main(std::vector<Item>::iterator begin, std::vector<Item>::iterator end) {
		std::vector<Item> v;
		for(auto i = begin;i<end;i++) v.push_back(*i);
		return v;
};

template<class T>
std::vector<T> str2list(const char *string) {
	std::vector<T> vec;
	for(int i=0;i<strlen(string);i++) { 
		unsigned int v = string[i];
		if(v!=" "[0]) {
			const char *s = (const char*)&v;
			vec.push_back(T(s));
		}
	}
	vec.push_back(T("EOF"));
	return vec;
}


Stack<States<std::string>> skeleton_lr(rule_t *tree) {
	Stack<States<std::string>> stack;
	Stack<int> Goto;
	States<std::string> s;
	while(tree!=NULL) {
		if(tree->param1_s != "(null)") {
			s.compute(tree->param1_s);
			stack.push(s);
		}
		else if(tree->param2_s != "(null)") {
			s.compute(tree->param2_s);
			stack.push(s);
		}

		Goto.push(s.getGoto());
		tree = tree->next1;
	}
	return stack;

}
Stack<std::string> skeleton_values(rule_t *tree) {
	Stack<std::string> stack;
	while(tree->next1!=NULL) {
		if(tree->data_s ==  "(null)" && tree->next1->data_s == "\n" ) break;
		if(tree->data_s != "(null)")  {
			stack.push(tree->data_s);
		}
		tree = tree->next1;
	}
	return stack;
}

// needs the class Stack, inorder to turn into a vector

std::vector<std::string> vectorize_stack_lr(char *name) {
	rule_t *tree;
	std::vector<std::string> vec_lr;
	tree = store_as_two_lookaheads(tree, name);
	Stack<States<std::string>> stack_lr = skeleton_lr(tree);
	free_tree(tree); // leak for once please
	while(stack_lr.size()>0) {
		stack_lr++;
		vec_lr.push_back(stack_lr.top().getstr());
	}
	return vec_lr;
}
// needs the class Stack, inorder to turn into a vector. That's why can not put them in another function
// now that we are using std::vector, we might as well use them in untils.h

std::vector<std::string> vectorize_stack_values(char *name) {
	rule_t *tree = NULL;
	std::vector<std::string> vec_values;
	tree = store_as_two_lookaheads(tree, name);
	Stack<std::string> stack_values = skeleton_values(tree);
	free_tree(tree);
	while(stack_values.size()>0) {
		stack_values++;
		vec_values.push_back(stack_values.top());
	}
	return vec_values;
}

auto intVec(std::vector<std::string> vec) {
	std::vector<int> vec_i;
	std::regex  re("[0-9]");
	std::string s;
	for(auto S : vec)  s += S;
	for(auto c : s) {
		if( c == "5"[0] ) vec_i.push_back(5);
		else if( c == "3"[0] ) vec_i.push_back(3);
	}
	return vec_i;
}

int rec_actions(char *name) {
	rule_t *tree = NULL;
	tree = store_as_two_lookaheads(tree, name);
	Stack<States<std::string>> stack = skeleton_lr(tree);
	Stack<int> s_stack;
	int count_x = 0, count_y = 0;
	while(stack.size()>0) {
		 stack+=2;
		 s_stack++;
		 if(stack[0].getGoto() == 5 && stack[1].getGoto() == 3) {
			 s_stack.push(10);
			 s_stack.pout(5);
			 s_stack.pout(3);
		 }
		 else {
			 s_stack.push(stack[0].getGoto());
			 s_stack.push(stack[1].getGoto());
		 }
		if(s_stack.top()==3 ) count_x++;
		else if(s_stack.top()==5 ) count_y++;
	}
	return (count_x + count_y)%2;
}

int actions(char *name) {
	rule_t *tree = NULL;
	tree = store_as_two_lookaheads(tree, name);
	Stack<States<std::string>> stack = skeleton_lr(tree);
	Stack<int> s_stack;
	int count_x = 0, count_y = 0;
	while(stack.size()>0) {
		 stack+=2;
		 if(stack[0].getGoto() == 5 && stack[1].getGoto() == 3) {
			 s_stack.push(10);
			 s_stack.pout(5);
			 s_stack.pout(3);
		 }
		 else {
			 s_stack.push(stack[0].getGoto());
			 s_stack.push(stack[1].getGoto());
		 }
	}

	while(s_stack.size()>1) {
		s_stack++;
		if(s_stack.top()==3 ) count_x++;
		else if(s_stack.top()==5 ) count_y++;
	}
	
	printf("%d : %d\n", count_x, count_y);
	return count_y ? 1 : 0;
}


auto AllItems(std::vector<std::string> vec_lr){
	auto vec_i = intVec(vec_lr);
	auto stack = create_stack(vec_i);
	auto vi = create_identity(stack);
	return process(vi);
}
// -> std::vector<std::vector<Item>> 
auto FirstRobust(auto items) {
	std::vector<std::vector<Item>> itms;
	int i=0;
	std::vector<std::vector<Item>> copy_items = items;
	while(copy_items.size()>2)  {
		copy_items= Iter2Vec(copy_items.begin() + 2, copy_items.end());
		auto items_ = Iter2Vec(copy_items.begin(), copy_items.end()-2);
		//std::cout << items_[0][0].getContainer()[0] << "\n";
		//std::cout << items_.size() << "\n";
		if(items_.size()<1) break;
		itms.push_back(closure(items));
	}
	return itms;
}


// -> std::vector<std::vector<Item>> 
auto First(auto items) {
	std::vector<std::vector<Item>> itms;
	int i=0;
	std::vector<std::vector<Item>> copy_items = items;
	while(copy_items.size()>2)  {
		copy_items= Iter2Vec(copy_items.begin() + 2, copy_items.end());
		auto items_ = Iter2Vec(copy_items.begin(), copy_items.end()-2);
		if(items_.size()<1) break;
		itms.push_back(closure(items_));
	}
	return itms;
}

template<class T>
std::vector<T> splitVector(T vec2d) {
	std::vector<T> vec2d_split;
	T vec_tmp;
	size_t size = vec2d.size()/4; // later replace with threads num invovked by system check
	for(int i=0,k=0;i<vec2d.size();i++,k++) {
		vec_tmp.push_back(vec2d[i]);
		if(k==size) {
			vec2d_split.push_back(vec_tmp);
			vec_tmp.clear();
			k=0;
		}
	}
	return vec2d_split;
}
template<class T> std::vector<std::thread> makeThreads(T vec_split) {
	std::vector<std::thread> ths;
	for(int i=0;i<4;i++) {
		auto atab = ActionGen(vec_split[i]);
		ths.push_back(std::thread(TimesAction,std::ref(atab)));
	}
	return ths;
}

void item_iter(std::vector<std::vector<Item>> vec2d) {
	for(auto vec : vec2d) 
		for(Item item : vec)
			skLr(item.getContainer());
}

template<class T> std::promise<T> set_promise(T value) {
	std::promise<T> rep;
	rep.set_value(value);
	return rep;
}
int work(std::promise<int> &&p, std::vector<std::vector<Item>> spitems) {
	return TimesAction(ActionGen(spitems));
}
void n_work(std::vector<std::vector<Item>> spitems, int *ret) {
	*ret = TimesAction(ActionGen(spitems));
}

void single_work(std::vector<std::vector<Item>> spitems, int *ret) {
	*ret = TimesAction(ActionGen(spitems));
}

int parall_thread(std::vector<std::vector<std::vector<Item>>> v_split) {
	std::vector<std::thread> th;
	int actions = 0,action;
	for(auto t : v_split) {
	      std::promise<int> p;
	      auto fu = p.get_future();
	      th.push_back(std::thread(work, std::move(p), t));
	      th[th.size() -1].join();
		  action = fu.get();
	      actions+=action;
		  fmt::print("executed action: {}\n",action);
	}
	return actions;
}

int work_(int i,int *ac) {
	for(int j=0;j<10;++j){
	for(int i=0;i<100000000;++i);
	for(int i=0;i<100000000;++i);
	for(int i=0;i<100000000;++i);
	for(int i=0;i<100000000;++i);
	}
	*ac = i;
	std::cout << i << "\n";
	return 1;
}

int _parall_thread(std::vector<std::vector<std::vector<Item>>> v_split) {
	std::vector<std::thread> th;
	int actions = 0,action,i=0;
	for(auto t : v_split) {
	      //auto fu = p.get_future();
	      th.push_back(std::thread(n_work,t,&action));
		  //action = fu.get();
	      //actions+=action; 
		  fmt::print("started thread {}\n",++i);
	}
	for(auto &t : th) {
		t.join();
		fmt::print("action: {}\n",action);
		actions+=action;
	}

	return actions;
}
std::vector<std::vector<std::vector<Item>>> 
serialize_count_8(std::vector<std::vector<std::vector<Item>>> v3d) {
	std::vector<std::vector<std::vector<Item>>> out3d;
	for(auto v2d : v3d)	{
		std::vector<std::vector<Item>> out2d;
		int tcount = THREAD/2;
		for(int i=0;i<v2d.size();++i) {
			out2d.push_back(v2d[i]);
			if(i == v2d.size()/tcount) {
				out3d.push_back(out2d);
				out2d.clear();
				if(tcount>=1) tcount/=2;
			}
			//if(i == v2d.size() * 3/4) {
			//	out3d.push_back(out2d);
			//	out2d.clear();
			//}
			//if(i == v2d.size()/2) {
			//	out3d.push_back(out2d);
			//	out2d.clear();
			//}
			//if(i == v2d.size() - 1) {
			//	out3d.push_back(out2d);
			//	out2d.clear();
			//}
		}
	}
	return out3d;
}

int single_thread(std::vector<std::vector<std::vector<Item>>> v_split) {
	int actions = 0,action;
	for(auto t : v_split) {
	      single_work(t,&action);
	      actions+=action;
		  fmt::print("executed action: {}\n",action);
	}
	return actions;
}

void get_split_vec(char *buf, struct ParseParam &sp) {
	std::vector<std::string> vec_lr = vectorize_stack_lr(buf);
	std::vector<std::string> vec_values = vectorize_stack_values(buf); 
	auto items = AllItems(vec_lr);
	auto items_ = First(items);
	//std::cout << "Items size  -> " << items_[0].size() << "\n";
	//auto gtitems_ = build_GtTable1D(items_);
	auto gtitems = build_GtTable(items);
	auto spitems = iterate_vec2d(gtitems);
	auto v_split =  splitVector(spitems);
	sp.vec_lr = vec_lr;
	sp.vec_values = vec_values;
	sp.v_split = v_split;
}

