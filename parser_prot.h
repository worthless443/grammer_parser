#include<vector>
#include<string>
typedef struct rule {
	const char *param1, *param2;
	std::string param1_s, param2_s;
	const char *data;
	std::string data_s;
	struct rule *next1, *next2;
} rule_t;

rule_t  *mk_parse_paren(rule_t *rr, char *exp);
rule_t *store_as_two_lookaheads(rule_t *tree, char *expr);
void free_tree(rule_t *r);
void print_tree(rule_t *tree, int i=0);
