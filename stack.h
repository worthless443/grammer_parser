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
	std::vector<T> vec;
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
	void compute_without(T val) {
		value = val;	
		string+=value;
	}
	
	void stackUp(T a)  {
		vec.push_back(a);
	}
	std::vector<T> getVector() {return vec;}
	size_t size() {return vec.size();}
	
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
	Stack operator+(int size) {
		Stack<T> stack_;
		std::vector<T> v;
		for(int i=size;i<begin.size();i++)  v.push_back(begin[i]);
		stack_.begin = v;
		//std::cout << begin.size() << "\n";
		return stack_;
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

//std::string convertToString(int i) {
//	std::ostringstream ss;
//	ss << i;
//	return std::string(ss.str());
//}
