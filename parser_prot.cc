#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<define.h>
#include<parser_prot.h>

#include<string>
#include<iostream>
#include<vector>


static char *filter_str(const char *sentence_) {
	char *_sentence = (char*)malloc(sizeof(char)*strlen(sentence_));
	strcpy(_sentence, sentence_);
	for(int i=0;_sentence[i]!=0;i++) { 
		if(_sentence[i]==" "[0]) _sentence[i] = 0x1;
	}
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

static std::vector<std::string> parse_string(const char *expr) {
	std::vector<std::string> vec;
	for(int i=0;i<strlen(expr);i++) {
		int each = expr[i];
		vec.push_back(std::string((char*)&each));
	}
	return vec;
}

rule_t *store_as_two_lookaheads(rule_t *tree, char *expr) {
	tree = new rule_t;
	expr = filter_str(expr);
	std::string expr_(expr) ;
	std::vector<std::string> vec = parse_string(expr);
	if(vec[0]=="(") {
		tree->param1_s = vec[0];
		tree->param2_s  = "(null)";
		tree->data_s  = "(null)";
	}
	else if(vec[0]==")") {
		tree->param1_s = "(null)";
		tree->param2_s  = vec[0];
		tree->data_s  = "(null)";
	}
	else {
		tree->param1_s = "(null)";
		tree->param2_s  = "(null)";
		tree->data_s  = vec[0];
	}

	if(vec.size()==1) return tree;
	tree->next1 =  store_as_two_lookaheads(tree->next1, &(*(expr_.begin() + 1)));
	return tree;
}

void free_tree(rule_t *r) {
	if(r==NULL) return;
	free(r);
	free_tree(r->next1);
	
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
	char *fuck = filter_str(argv[1]);
	
	rule_t *tree = NULL;
	rule_t *tree_ =store_as_two_lookaheads(tree, fuck);
	print_tree(tree_);
	//rule_t *rr; //, 
	//rule_t *r = mk_parse_paren(rr, fuck);
	//print_test(r);
	//free_tree(rr);
	return 0;
}
#endif 
