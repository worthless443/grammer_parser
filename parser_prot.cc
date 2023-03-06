#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<define.h>
#include<parser_prot.h>

#include<string>
#include<regex>
#include<iostream>
#include<vector>

static std::string filter_str(const char *sentence_) {
	char *sen = (char*)sentence_;
	for(int i=0;sen[i]!=0;i++) { 
		if(sen[i]==" "[0]) sen[i] = 0x1;
	}
	std::string _sentence(sen);
	return _sentence;
}
//printf("catched\n");
rule_t  *mk_parse_paren(rule_t *rr, char *exp) {
	rule_t *r;
	char *exp_ = (char*)malloc(100);
	int i;
	if(strlen(exp)==1) return rr; //use check for terminal symbols instead of converage of senetence `exp`

	for(int j=0;j<strlen(exp);j++) { 
		if(exp[j]=="("[0] || exp[j] == ")"[0]) { //select which "rules" to apply.
		for(i=1;i<strlen(exp);i++)  { 
			if(exp[i]==")"[0]) {
				break;
			}
			else {
				exp_[i-1] = exp[i]; //check the login since it produces invalid tree
				exp = exp + 1;
			}
		}
		break;
	}
	else  for(int i=0;i<strlen(exp);i++) exp = exp + 1;
	}
	rr = (rule_t*)malloc(sizeof(rule_t)*10);
	rr->param1 = exp;
	rr->param2 = exp_;
	rr->next1 = mk_parse_paren(rr->next1, exp);
	//rr->gnext2 = mk_parse(rr->next2, exp);
	return rr;
}

static int *parse_string(const char *expr) {
	int* vec = (int*)malloc(50*sizeof(int));
	for(int i=0;i<strlen(expr);i++) {
		int each = expr[i];
		vec[i] = each;
	}
	return vec;
}
static int checkfor_eval_regex(const char *s) {
	int times = 0;
	std::vector<std::string> str_v;
	for(int i=0;i<strlen(s);++i) {
		char _tmp = *(s + i);
		str_v.push_back(std::string(&_tmp));
	}
	std::regex r("\(");
	for(auto str : str_v) { 
		times+=std::regex_match(std::string("("), r);
		std::cout << times << "\n";
	}
	return times;
}

int checkfor_eval(const char *s) {
	int times = 0;
	for(int i=0;i<strlen(s);++i) 
		if(*(s+i)=='(' || *(s+i)==')' || *(s+i)==' ') ++times;
	return strlen(s) - times - 1;
}
size_t size_int_ptr(int *vec) {
	size_t size;
	for(size=0;*(vec+size);size++);
	return size;
}
rule_t *store_as_two_lookaheads(rule_t *tree, char *expr) {
	int i = 0;
	tree = new rule_t;
	std::string _expr = filter_str(expr);
	std::string expr_(_expr) ;
	int *vec = parse_string(expr);
	std::string comp((char*)&vec[0]);
	size_t size = size_int_ptr(vec);
	if(comp=="(") {
		tree->param1_s = comp;
		tree->param2_s  = "(null)";
		tree->data_s  = "(null)";
	}
	else if(comp==")") {
		tree->param1_s = "(null)";
		tree->param2_s  = comp;
		tree->data_s  = "(null)";
	}
	else {
		tree->param1_s = "(null)";
		tree->param2_s  = "(null)";
		tree->data_s  = comp;
	}

	free(vec);
	if(size==2) {
		tree->next1 = NULL;
		return tree;
	}
	std::cout << i << "\n";
	tree->next1 =  store_as_two_lookaheads(tree->next1, &(*(expr_.begin() + 1)));
	return tree;
}

void free_tree(rule_t *r) {
	if(r->next1==NULL) return;
	else {
		free(r);
		free_tree(r->next1);
	}
}

void print_tree(rule_t *tree, int i) {
	if(tree!=NULL) {
		printf("b%d :: %s && b%d :: %s\ndata%d = %s\n\n", i,tree->param1_s.c_str(), i+1,tree->param2_s.c_str(), i, tree->data_s.c_str()); 
		//const char *p = tree->data;
		print_tree(tree->next1, i+2);
	}
}

void print_test(rule_t *r, rule_t **path_mem = NULL, int level=0) {
	if(level<1) path_mem = (rule_t**)malloc(8*10);
	if(r==NULL) { 
		int i;
		for(i=0;*(int*)(path_mem + i)!=0;i++);
		for(int j=i-1;j>=0;j--) printf("%s\n", path_mem[j]->param1); // satisfy the condition then do something
		return;
	}
	path_mem[level] = r;
	print_test(r->next1, path_mem, level+1);
}

#ifdef __main__ 
int main(int argc, char **argv) {
	if(argc<2) return -1;
	std::string fuck = filter_str(argv[1]);
	
	rule_t *tree = NULL;
	rule_t *tree_ =store_as_two_lookaheads(tree, fuck.c_str());
	print_tree(tree_);
	//rule_t *rr; //, 
	//rule_t *r = mk_parse_paren(rr, fuck);
	//print_test(r);
	//free_tree(rr);
	return 0;
}
#endif 
