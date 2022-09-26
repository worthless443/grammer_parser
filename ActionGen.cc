#include<iostream>
#include<vector>
#include<cstdio>
#include<utility>
#include<string>
#include<sstream>
#include<iterator>
#include<utility>
#include<regex>
#include <initializer_list>


#include<cstring>
#include<cstdlib>

#include<parser_prot.h>
//#include<decorate_error.h>
#include<eval.h>
#include<stack.h>
#include<TableGen.h>
#include<ActionGen.h>

ActionsEnum action(Stack<int> stack, int word) {
	if(stack.top() ==  10) {
		return Accept;
	}
	if(stack.top()==word) return Reduce;
	else if(stack.top()!=word) return Shift;
	else return Fail;
}

#ifdef __main__ 
int main() {
	skLr words{1,2,4,3,5,4,1,3,5,1,4};
	//skLr(words);
}
#endif
