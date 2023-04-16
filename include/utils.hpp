template<class T>
T Iter2Vec_Main(T::iterator begin, T::iterator end) {
		std::vector<std::vector<Item>> v;
		for(auto i = begin;i<end;i++) v.push_back(*i);
		return v;
}

