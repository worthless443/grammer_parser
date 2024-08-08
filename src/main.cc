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

#include<main.h>


static inline int __isValidFn(char *fn) {
	int i;
    char ext[64] = {0};
    int size_fn = strlen(fn);
	for(i=size_fn;fn[i - 1]!='.';--i)  {
        int k = size_fn - i;
        memcpy(ext,fn + i - 1, 1);
        if(fn[i - 2] != '.')
            memcpy(ext + 1, ext,k+1);
        if(i == 0)
			return 0;
    }
    memcpy(fn, ext,size_fn - i);
    memset(fn + size_fn - i, '\0', i);
    return (0 == strcmp(ACCEPT_EXT, fn)) ? 1 : 0;
}

int main(int argc, char **argv) {
	rule_t *tree = NULL;
	int _actions = 0 ;
	FILE *f = NULL;
    char tmp_filenm[64] = {0};
	std::vector opt = {0,0};
	if(argc<2)  {
		std::cout << "no file provided\n";
		return 1;
	}
	for(int i=1;i<argc;++i) {
		if(std::string(argv[i] + 1) == "p") 
			opt[0] = 1;
		else if(std::string(argv[i] + 1) == "v") 
			opt[1] = 1;
	}

    memcpy(tmp_filenm, argv[1], strlen(argv[1]));
	if(!__isValidFn(tmp_filenm)) {
		fprintf(stderr,"Invalid file extension (\"%s\"), need \"" ACCEPT_EXT "\"\n", tmp_filenm);
		return 1;
	}

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
	std::cout << "input -> " << buf << "\n";
	
	//char fn[strlen(argv[1])] = {0};
	//memcpy(fn, argv[1], strlen(argv[1]));
	//if(!isValidFn((const char*)fn)) {
	//	fprintf(stderr,"%s: unrecognized format\n",argv[1]);
	//	return 1;
	//}
	tree = store_as_two_lookaheads(tree, buf);
	if(!actions(tree)) { 
		std::cout << "Normal:extra parms\n";
		normal += 1;
	} else std::cout << "Normal:success\n";
	if(rec_actions(tree)) {
		std::cout << "Recrusive:extra parms\n";
		rec += 1;
	}
	else std::cout << "Recrusive:success\n";
	std::cout << "\n";

	std::cout << "stats:\n\n";
	struct ParseParam sp;

	
	//auto threads = makeThreads(Iter2VecT<std::vector<std::vector<Item>>>(v_split.begin()+1,v_split.end()));
	//std::cout << serialize_count_8(sp.v_split).size() << "\n";
	int siz = 0;
	get_split_vec(tree, sp);
	rule_t *_tree = tree;
	while(_tree->next1 != NULL)
		_tree = _tree->next1,
		++siz;
	printf("size -> %d\n", sp.v_split.size());
	free_tree(tree);
	if(opt[0])
		_actions = parall_thread(serialize_count_8(sp.v_split)),
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
