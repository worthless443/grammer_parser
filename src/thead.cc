#include<thread>
#include<future>
#include<vector>
#include<iostream>

void print(int i) {
	for(int j=0;j<100000000;++j){
	for(int i=0;i<100000000;++i);
	for(int i=0;i<100000000;++i);
	for(int i=0;i<100000000;++i);
	for(int i=0;i<100000000;++i);
	}
	std::cout << i << "\n";
}
void create_thread(std::vector<int> v) {
	std::vector<std::thread> th;
	for(int i : v) {
		th.push_back(std::thread(print,i));
		th[th.size() - 1].join();
	}
	//for(auto &t : th)
		//t.join();
	
}

void create_thread_async(std::vector<int> v) {
	for(int i : v)
		auto future = std::async(print, i);
    //int simple = future.get();
}
int main() {
	std::vector v{1,2,3,4,5,6};
	create_thread_async(v);
}
