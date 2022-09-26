enum ActionsEnum {
	Shift,Reduce, Accept, Fail
};
ActionsEnum action(Stack<int> stack, int word);
class skLr {
	std::vector<int> vec;
public:
	skLr(std::initializer_list<int> l)  : vec{l} {
		vec.push_back(10); // eof
		init();
	}
	skLr(std::vector<int> v) : vec{v} {
		vec.push_back(10);
		init();
	}
	void init() {
		Stack<int> stack;
		int word;
		stack.push(0);
		int i=0;
		ActionsEnum actions;
		while(1) {
			std::cout << "nigger\n";
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

