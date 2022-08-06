#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define sentence "(1 + 2) x 3"

typedef struct rule {
	const char *param1, *param2;
	struct rule *next1, *next2;
} rule_t;


char *filter_str(char *sentence_) {
	char *_sentence = malloc(sizeof(char)*strlen(sentence_));
	strcpy(_sentence, sentence_);
	for(int i=0;_sentence[i]!=0;i++) { 
		if(_sentence[i]==" "[0]) _sentence[i] = 0x1;
	}
	return _sentence;
}

rule_t  *mk_parse(rule_t *rr, char *exp) {
	rule_t *r;
	char *exp_ = malloc(100);
	int i;
	if(strlen(exp)==0) return rr;

	for(int i=0;i<strlen(exp); i++) {
		if(exp[i]=="("[0] || exp[i] == ")"[0]) {
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
	}
//	else  {
//		//exp_ = exp;
//		for(int i=0;i<strlen(exp);i++) exp = exp + 1;
//		break;
//	}
	
	}

	rr = malloc(sizeof(rule_t)*10);
	rr->param1 = exp;
	rr->param2 = exp_;
	rr->next1 = mk_parse(rr->next1, exp_);
	//rr->next2 = mk_parse(rr->next2, exp);
	return rr;
}


void free_tree(rule_t *r) {
	printf("ssize\n");
	if(r==NULL) return;
	free(r);
	free_tree(r->next1);
	
}

void print_test(rule_t *r) {
	if(r==NULL) return;
	printf("%s\n", r->param2);
	print_test(r->next1);
}

int main() {
	char *fuck = "(2 x 4)(4+2)+3";
	fuck = filter_str(fuck);
	rule_t *rr; //, 
	rule_t *r = mk_parse(rr, fuck);
	//free_tree(rr);
	print_test(r);
	
}
