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


#define RED "\033[1;31m"
#define GREEN "\033[1;32m"

int isValidFn(const char *fn) {
	int i;
	for(i=0;fn[i]!='.';++i)
		if(i > strlen(fn))
			return 0;
	char buf[strlen(fn) - i - 1];
	memcpy(buf,fn + i,i+ 1);
	printf("%s\n",buf);
	return std::string(buf) == ".br";
}

int main(int argc, char **argv) {
	if(argc<2)  {
		std::cout << "no file provided\n";
		return 1;
	}
	FILE *f = NULL;
	
		
	if((f=fopen(argv[1], "r"))==NULL) {
			perror("parser");
			return 1;
	}
	char *buf = (char*)malloc(sizeof(char));
	char *shortbuf = buf;
	int rec = 0, normal = 0;
	for(int i=0;fread(shortbuf + i, 1,1,f);++i) {
		buf = (char*)realloc(buf, sizeof(char) * (i + 5));
	}
	char _buf[strlen(buf)] = {0};
	memcpy(_buf,buf,strlen(buf) - 1);
 	if(*buf < 40 || *buf > 41) {
		fprintf(stderr,"Unkown char \"%s\"\n", _buf);
		return 1;
	}
	std::cout << "buf -> " << buf << "\n";
	//char fn[strlen(argv[1])] = {0};
	//memcpy(fn, argv[1], strlen(argv[1]));
	//if(!isValidFn((const char*)fn)) {
	//	fprintf(stderr,"%s: unrecognized format\n",argv[1]);
	//	return 1;
	//}
	if(actions(buf) == 0) { 
		std::cout << "Normal:extra parms\n";
		normal += 1;
	} else std::cout << "Normal:success\n";
	//return 1;
	if(rec_actions(buf)) {
		std::cout << "Recrusive:extra parms\n";
		rec += 1;
	}
	else std::cout << "Recrusive:success\n";
	std::cout << "\n";

	std::cout << "stats:\n\n";
	struct ParseParam sp;

	
	//auto threads = makeThreads(Iter2VecT<std::vector<std::vector<Item>>>(v_split.begin()+1,v_split.end()));
	//std::cout << serialize_count_8(sp.v_split).size() << "\n";
	get_split_vec(buf, sp);
	int _actions = parall_thread(serialize_count_8(sp.v_split));
	//int actions = single_thread(v_split);
	std::cout << "times action " <<  _actions << "\n";
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
	std::cout << ((rec + normal > 0) ? "\033[1;31m" : "") << "exiting with error code: " << rec + normal << "\033[00m\n";
	buf[strlen(buf) - 1] = '\0';
	if(rec + normal)
		printf("(\"" RED "%s\"\033[00m is invalid)\n",buf, (rec + normal) ? RED : GREEN);
	else
		printf("(\"" GREEN "%s\033[00m\" is valid)\n",buf, (rec + normal) ? RED : GREEN);

	free(buf);
	return rec + normal ;
}
