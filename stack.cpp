#include<iostream>
#include<vector>
#include<cstdio>
#include<utility>
#include<string>
#include<sstream>
#include<iterator>
#include<utility>

#include<cstring>
#include<cstdlib>

#include<parser_prot.h>
#include<decorate_error.h>
#include<eval.h>

typedef struct {} Action; // shoudl be filled with function pointers
typedef struct {
public:
	int action;	
} Goto ;

template<class T>
class States {
	std::pair<Action, Goto> pair;
	T value;
	T string;
	Action ac;
	Goto gt;
public:
	States() : value(T()) {}
	States(T word) : value(word) {
		if(value == "(") gt.action  = 3;
		else if(value==")") gt.action = 5;
		else gt.action = 0;
	}
// How to find handle?
	T forPrint(int i) {
		std::ostringstream os;
		os << i;	
		return std::string{os.str()};
}
	void compute(T val) {
		if(val== "(") gt.action  = 3;
		else if(val==")") gt.action = 5;
		else gt.action = 0;
		value = val;
		string += value + " "  + forPrint(gt.action);
	}
	int getGoto() {return gt.action;}
	T getValue() {return value;}
	T getstr() {return string;}
};
template<class T>
std::ostream &operator<<(std::ostream &os, States<T> &s) {
		os << s.getstr();
		return os;
}

template<class T>
class Stack { 
private:
	std::vector<T> begin;
	std::vector<T> replace(std::vector<T> begin_) {
		int size = begin_.size();
		for(int i=(size/2);i<size-1;i+=1)  {
			int next  = begin_[i];
			begin_[i] = begin_[i+1];
			begin_[i+1] = next;
		}
		return begin_;
}
	void swapmid(std::vector<T> &begin_) { // does not  work
		int size = begin_.size();
		for(int i=size/2;i<(size/2)+2;i++) {
			int next  = begin_[i];
			begin_[i] = begin_[i+1];
			begin_[i+1] = next;
		}
	}

	int idx =0;

public:
	Stack(T init) : begin{init} {}
	Stack() {
		T padding;
		begin.push_back(padding);
	}
	~Stack() {
			begin.clear();
			idx = 0;
	} void reset() {
			begin.clear();
	}
	T top() {return begin[0];}
	T bottom() {return begin[begin.size() - 2];}
	void push(T val) {
		begin.push_back(val);
		int size = begin.size();
		for(int i=0;i<begin.size() - 1;i+=1) {
			T first = begin[i];
			if(i==0) {
				begin[i] = begin[size - 1] ;
				begin[size -1 ] = begin[size - 2];
			}
			else {
				begin[size - (i+1)] = begin[size - (i+2)];
			}
		}
				
	}
	T mid() {return begin[begin.size()/2];}
	std::vector<T> operator++(int) {
		std::vector<T> v;
		for(int i=1;i<begin.size();i++)  v.push_back(begin[i]);
		begin = v;
		//std::cout << begin.size() << "\n";
		return v;
	}
	std::vector<T> operator+=(int size) {
		std::vector<T> v;
		for(int i=size;i<begin.size();i++)  v.push_back(begin[i]);
		begin = v;
		//std::cout << begin.size() << "\n";
		return v;
	}
	int operator*() { return begin[0].getGoto(); }

	T operator[](int idx) {return begin[idx];}

	int size() {return begin.size();}
	
	void pop(T val) {
		int size = begin.size();
		int k=0;
		std::vector<T> begin_ = begin ;
		for(int i=0;i<begin.size()-1;i++) {
			if(begin[i]==val) {
				T last = begin[size - 1];
				T cur = begin[i];
				begin_[size - 1] = cur;
				begin_[i] = last;
				k++;	
			}
		}
		for(int i=0;i<1;i++) begin_ = replace(begin_);
		begin = begin_;
	} 

	void onlypop(T val) {
		std::vector<T> begin_ = begin ;
		int size = begin.size();
		pop(val);
		for(int i=0;i<(size/2)-1;i++) begin_ = replace(begin_);
		begin = begin_;
		
	}

	void pout(T val) {
		int i;
		for(i=0;i<begin.size()-1;i++) if(val==begin[i]) begin[i] = 0;
	}
	std::vector<T> get() {return begin;}
	void print() { for(T a : begin) std::cout << a << " "; std::cout << "\n"; }

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
	free_tree(tree);
	return stack;

}

Stack<std::string> skeleton_values(rule_t *tree) {
	Stack<std::string> stack;
	while(tree!=NULL) {
		if(tree->data_s != "(null)")  stack.push(tree->data_s);
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
	free_tree(tree);
	while(stack_lr.size()>0) {
		stack_lr++;
		vec_lr.push_back(stack_lr.top().getstr());
	}
	return vec_lr;
}
// needs the class Stack, inorder to turn into a vector. That's why can not put them in another function
// now that we are using std::vector, we might as well use them in untils.h

std::vector<std::string> vectorize_stack_values(char *name) {
	rule_t *tree;
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

int main(int argc, const char **argv) {
	if(argc<2)  {
		std::cout << "no file provided\n";
		return 1;
	}
	char *buf = (char*)malloc(100* sizeof(char));
	FILE *f = fopen(argv[1], "r");
	char shortbuf[1];
	int rec = 0, normal = 0;
	while(fread(shortbuf, 1,1,f)) {
		strcat(buf,shortbuf);
	}
	if(!actions(buf)) { 
		std::cout << "Normal:extra parms\n";
		normal += 1;
	}
	else std::cout << "Normal:success\n";
	if(rec_actions(buf)) {
		std::cout << "Recrusive:extra parms\n";
		rec += 1;
	}
	else std::cout << "Recrusive:success\n";
	std::cout << "\n\n\n";

	std::cout << "stats:\n\n";

	std::vector<std::string> vec_lr = vectorize_stack_lr(buf);
	std::vector<std::string> vec_values = vectorize_stack_values(buf);

	std::pair<std::vector<int>, std::vector<std::string>> pair = parse_values(vec_values);
	//int ret = eval_main(pair.first, pair.second);
	
	int res = eval_main(pair);

	std::cout << "EVAL Val == " << res << "\n";

	DecorError decor(vec_lr, vec_values);

	std::cout << decor << "\n";
	free(buf);
	std::cout << "exiting with error code: " << rec + normal << "\n";
	return rec + normal ;
	//stack.print();
	std::vector<std::string> list = str2list<std::string>(argv[1]);
	if(list.size()<1) return 1;
	Stack<States<std::string>> ss;
	States<std::string> s;
	for(auto S : list) {
		s.compute(S);
		ss.push(s);
		std::cout << s << "\n";
	}
	return 0;
	//ss.print();
}
