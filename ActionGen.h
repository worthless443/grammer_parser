
enum ActionsEnum {
	Shift,Reduce, Accept, Fail
};
ActionsEnum action(Stack<int> stack, int word);
ActionsEnum action_(std::vector<Item>);
ActionsEnum mapEnum(Item item);
class ActionTable {
	int lookahead, idx, action;
	std::vector<std::pair<int,int>> cor;
	using T = std::vector<std::pair<int,int>>;
public:
	T::iterator begin() {return cor.begin();}
	T::iterator end() {return cor.end();}
	size_t size() {return cor.size();}
	ActionTable(int lookahead, int idx, int action) : lookahead(lookahead), action(action), idx(idx) {}
	ActionTable() = default;
	int &operator=(int lookahead) {cor.push_back(std::make_pair(cor.size(), lookahead));return cor[cor.size()-1].second;}
	ActionTable &operator[](int lookahead) {
		return *this;
	}
};

class skLr {
	std::vector<int> vec;
	std::vector<std::vector<Item>> items2d;
	Stack<int> stack;
public:
	skLr(std::initializer_list<int> l)  : vec{l} {
		vec.push_back(10); // eof
		init();
	}
	skLr(std::vector<int> v) : vec{v} {
		vec.push_back(10);
		init();
	}
	skLr(std::vector<std::vector<Item>> items2d) : items2d{items2d} { }
	const bool reduction_shifting() { // to see if they are homomorphic (and they will be)
		int reduction = 0, shift=0, fail=0;
		for(auto vec_ : items2d) {
			while(1)  {
				if(action_(vec_)==Reduce) {
					++reduction;
				}
				if(action_(vec_)==Shift) {
					++shift;
				}
				if(action_(vec_)==Fail) ++fail;
				if(action_(vec_)==Accept) break;

				vec_ = Iter2VecT<Item>(vec_.begin()+1, vec_.end());
			}
		}
		return reduction == shift;
	}
	void test() {
		for (auto vec : items2d) {
			while(1) {
			 if(mapEnum(vec[0])==Reduce) std::cout << "Reduce\n"; //reduce
			 if(mapEnum(vec[0])==Shift) std::cout << "Shift\n"; //reduce
			 if(mapEnum(vec[0])==Accept) break ; //reduce
			 vec = Iter2VecT<Item>(vec.begin()+1, vec.end());
			}
			std::cout << "\n";
		}
	}
private:
	void init() {
		int word;
		stack.push(0);
		int i=0;
		ActionsEnum actions;
		while(1) {
			if(action(stack, word)==Reduce) {
				stack.pout(word);
				word = vec[i];
			}
			else if(action(stack, word)==Shift) {
				stack.push(word);
				word = vec[i];
			}
			else if(action(stack,word)==Accept) break;
			i++;
		}
	}
};

ActionTable ActionGen(std::vector<std::vector<Item>> items2d);
void ActionStates(std::vector<int> &states, std::pair<int,int> p);
std::vector<int> ActionExec(std::vector<int>, Stack<int> &stack, ActionTable p_vec);
int TimesAction(ActionTable p_vec);
