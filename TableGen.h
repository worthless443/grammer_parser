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
	std::vector<int> container;
public:
	Item(int lookahead, int direction=0) :  prod{.direction = direction, .lookahead = lookahead}, placeholder(0) { }

	void stackUpCurrent(int i) {
		container.push_back(i);
	}
	std::pair<int,int> getList() {return std::make_pair(prod.list.a, prod.list.b);}
	void makeList(int a,int b) {prod = {a,b};}
	void print() {for(int i : container) std::cout << i << " ";std::cout << "\n";}
	std::vector<int> getContainer() {return container;}
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
	int size() {return container.size();}
	int getPlaceHolder() {return placeholder;}
	int getLookahead() {return prod.lookahead;}
	void reduce() { 
		prod.lookahead = 10; //  resembles a List
	}
	bool operator==(Item item) {return placeholder == item.placeholder;}
};

std::vector<std::vector<Item>> Iter2Vec(std::vector<std::vector<Item>>::iterator begin, std::vector<std::vector<Item>>::iterator end);

int generic_arraySearch(std::vector<int> vec, int what);
Stack<States<int>> create_stack(std::vector<int> vec);
std::vector<int> create_identity(Stack<States<int>> stack);
std::vector<std::vector<Item>> process(std::vector<int> vec);
std::vector<Item> closure(std::vector<std::vector<Item>> vec2d);
void vis_lr_item(std::vector<std::vector<Item>> vec2d);
std::vector<Item> Gt(std::vector<Item> vec);
std::vector<std::vector<Item>> build_GtTable(std::vector<std::vector<Item>> vec2d );
std::vector<std::vector<Item>> iterate_vec2d(std::vector<std::vector<Item>> vec2d);
void generate_derivation(std::vector<std::vector<Item>> vec2d);

template<typename T>
std::vector<T> Iter2VecT(typename std::vector<T>::iterator begin, typename std::vector<T>::iterator end) {
		std::vector<T> v;
		for(auto i = begin;i<end;i++) v.push_back(*i);
		return v;
	};


