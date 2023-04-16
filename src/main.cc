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
#include<decorate_error.h>
#include<eval.h>
#include<stack.h>
#include<TableGen.h>
#include<ActionGen.h>


int main(int argc, const char **argv) {
	if(argc<2)  {
		std::cout << "no file provided\n";
		return 1;
	}
	char *buf = (char*)malloc(10000* sizeof(char));
	FILE *f = NULL;
	if((f=fopen(argv[1], "r"))==NULL) {
			perror("parser");
			return 1;
	}
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
	std::cout << "\n";

	std::cout << "stats:\n\n";
	struct ParseParam sp;

	
	//auto threads = makeThreads(Iter2VecT<std::vector<std::vector<Item>>>(v_split.begin()+1,v_split.end()));
//	std::cout << serialize_count_8(v_split).size() << "\n";
	get_split_vec(buf, sp);
	int actions = _parall_thread(serialize_count_8(sp.v_split));
	//int actions = single_thread(v_split);
	std::cout << "times action " <<  actions << "\n";
	//for(int st : states) std::cout << st << "\n";
	//generate_derivation(gtitems);
	//vis_lr_item(gtitems);
	//for(int S : intVec(vec_lr)) std::cout << S << "\n";
	//message fuck
	std::pair<std::vector<int>, std::vector<std::string>> pair = parse_values(sp.vec_values);
	
	int res = eval_main(pair);

	std::cout << "EVAL Val == " << res << "\n";

	DecorError decor(sp.vec_lr, sp.vec_values);

	std::cout << decor << "\n";
	free(buf);
	std::cout << ((rec + normal > 0) ? "\033[1;31m" : "") << "exiting with error code: " << rec + normal << "\033[00m" << "\n";
	return rec + normal ;
}
