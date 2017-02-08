#include <iostream>
#include <string>
#include <cctype>
#include <stack>
#include <cassert>
using namespace std;
int precedence(char ch){
	switch (ch){
	case '!':return 3;
	case '&':return 2;
	case '|':return 1; 
	default:return -1;
	}
}


int evaluate(string infix, const bool values[], string& postfix, bool& result) {
	postfix = "";//empty postfix
	stack<char> operatorS;//stack to hold operator
	for (unsigned int a = 0; a < infix.size(); a++) {//traversal the infix string
		char temp = infix[a];//copy a character to a char temp
		switch (temp) {
		default: //a not acceptable operator appear
			return 1;case '0': case '1':case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9'://when temp is number
			postfix += temp;//append to the end
			break; case '('://when '(' appear
				if (a + 1 >= infix.size() || infix[a + 1] == '|' || infix[a + 1] == '&')return 1;//situation that operator and number inside( ) is not valid
				operatorS.push(temp);//push '(' into stack
			break;case ')'://when ) appear
				while (operatorS.top() != '(') {
					postfix += operatorS.top();//append the operator to the end to postfix string
					operatorS.pop();//pop out front operator
					if (operatorS.empty())return 1;//if there is not '(' not a valid infix string
				}
			operatorS.pop();//pop out (
			break; case '|': case '&' : case '!':
				while (!operatorS.empty() && operatorS.top() != '('&&precedence(temp) <= precedence(operatorS.top())) {
					/*when operator stack is not empty and the first operator on that stack is not '(' 
					and the precedence of char temp is <= than precedence of top operator*/
					postfix += operatorS.top();//append the top operator to end of string postfix
					operatorS.pop();//pop out the top one
				}
				operatorS.push(temp);//push temp into operator stack
				break; case ' ':  break;//when character space ignore it
		}
	}
	while (!operatorS.empty()) {
		if (operatorS.top() == '(')return 1;//'(' should not appear
		postfix += operatorS.top();//append the top operator to end of string postfix
		operatorS.pop();//pop out the top one
	}
	//complete the building of postfix
	if (postfix.empty())return 1;//if postfix is empty, not a valid postfix
	stack<bool>resultS;//stack of result
	for (unsigned int a = 0; a < postfix.size(); a++) {//traversal the postfix
		char temp = postfix[a];//copy a character to a char temp
		switch (temp) {
		default: //operator or number should not appear
			return 1;case '0': case '1':case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9'://case of number
			resultS.push(values[temp - '0']);//push the bool value which each number represented into stack
			break;
		case '!': {//case of !
			if (resultS.empty())return 1;//there is no bool value in stack but there is operator return false
			//change the top value into it's inverse
			bool ch = resultS.top();
			resultS.pop();
			resultS.push(!ch);
			break;
		}case '|': {//case of |
			if (resultS.empty())return 1; // there is no bool value in stack but there is operator return false
			//pop out two value and combine them with ||
			bool val1 = resultS.top();
			resultS.pop();
			if (resultS.empty())return 1;
			bool val2 = resultS.top();
			resultS.pop();
			resultS.push(val1 || val2);
			break;}
		case '&': {//case of &
			 //pop out two value and combine them with &&
			if (resultS.empty())return 1;
			bool val1 = resultS.top();
			resultS.pop();
			if (resultS.empty())return 1;
			bool val2 = resultS.top();
			resultS.pop();
			resultS.push(val1&&val2);
			break;
			}
		}
	}
	if (resultS.size() != 1)return 1;//case when missing operator
	result = resultS.top();//assign result
	return 0;//return true
}

void test1() {
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false, true,  false
	};
	string pf;
	bool answer;
	assert(evaluate("2| 3", ba, pf, answer) == 0  &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
		&& !answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && !answer);
	cout << "Passed all tests" << endl;
}
int main(){
	test1();
	bool ba[10] = {
		//  0      1      2        3      4      5       6      7      8       9
		true,  true,  true, false,false,false,true, false,true, false
	};
	string pf;
	bool result;
	assert(evaluate("2", ba, pf, result) == 0 && result);
	assert(evaluate("(3)", ba, pf, result) == 0 && !result);
	assert(evaluate("2&(3)", ba, pf, result) == 0 && !result);
	assert(evaluate("0 & !9", ba, pf, result) == 0 && result);
	assert(evaluate("!(7|8)", ba, pf, result) == 0 && !result);
	assert(evaluate("!7|8", ba, pf, result) == 0 && result);
	assert(evaluate("6|4&5", ba, pf, result) == 0 && result);
	assert(evaluate("1&!(9|1&1|9) | !!!(9&1&9)", ba, pf, result) == 0 && result);
	cout << "pass all tests\n" << endl;
}