#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define sentence "(1 + 2) x 3"

typedef struct rule {
	const char *param1, *param2;
	struct rule *next1, *next2;
	struct rule *prev  = this;
} rule_t;


char *filter_str(char *sentence_) {
	char *_sentence = (char*)malloc(sizeof(char)*strlen(sentence_));
	strcpy(_sentence, sentence_);
	for(int i=0;_sentence[i]!=0;i++) { 
		if(_sentence[i]==" "[0]) _sentence[i] = 0x1;
	}
	return _sentence;
}
//printf("catched\n");
rule_t  *mk_parse(rule_t *rr, char *exp) {
	rule_t *r;
	char *exp_ = (char*)malloc(100);
	int i;
	if(strlen(exp)==1) return rr;

	for(int j=0;j<strlen(exp);j++) {
		if(exp[j]=="("[0] || exp[j] == ")"[0]) {
			//printf("catched\n");
			//printf("catched\n");
		for(i=1;i<strlen(exp);i++)  { 
			if(exp[i]==")"[0]) {
				break;
			}
			else {
				exp_[i-1] = exp[i];
				exp = exp + 1;
			}
		}
		break;
	}
	else  {
//		//exp_ = exp;
		for(int i=0;i<strlen(exp);i++) exp = exp + 1;
//		break;
	}
	
	}

	rr = (rule_t*)malloc(sizeof(rule_t)*10);
	rr->param1 = exp;
	rr->param2 = exp_;
	rr->next1 = mk_parse(rr->next1, exp);
	//rr->gnext2 = mk_parse(rr->next2, exp);
	return rr;
}


void free_tree(rule_t *r) {
	printf("ssize\n");
	if(r==NULL) return;
	free(r);
	free_tree(r->next1);
	
}

void print_test(rule_t *r, rule_t **path_mem = NULL, int level=0) {
	if(level<1) path_mem = (rule_t**)malloc(8*10);
	if(r==NULL) { 
		int i;
		for(i=0;*(int*)(path_mem + i)!=0;i++);
		for(int j=i-1;j>=0;j--) printf("%s\n", path_mem[j]->param1);
		return;
	}
	path_mem[level] = r;
	print_test(r->next1, path_mem, level+1);
}

int main() {
	//char *fuck = "(2 x 4)(4+2(3+2))+3";
	char *fuck = "(1 x 2 )(2+2)(2-1)(3x1)+2";
	fuck = filter_str(fuck);
	rule_t *rr; //, 
	rule_t *r = mk_parse(rr, fuck);
	//free_tree(rr);
	print_test(r);
	
}
