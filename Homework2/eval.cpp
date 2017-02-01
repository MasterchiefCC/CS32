#include <iostream>
#include <string>
#include <cctype>
#include <stack>
#include <cassert>
using namespace std;
int precedence(char ch){
	switch (ch){
	case '!':       
		return 2;
	case '&':      
		return 1;
	case '|':
		return 0; 
	default:
		return -1;
	}
}


int evaluate(string infix, const bool values[], string& postfix, bool& result) {
	postfix = "";
	stack<char> operatorS;
	for (unsigned int a = 0; a < infix.size(); a++) {
		char temp = infix[a];
		switch (temp) {
		case '0': case '1':case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			postfix += temp;
			break; case '(':
				if (a + 1 >= infix.size() || infix[a + 1] == '|' || infix[a + 1] == '&')return 1;
				operatorS.push(temp);
			break;case ')':
				while (operatorS.top() != '(') {
					postfix += operatorS.top();
					operatorS.pop();
				}
			operatorS.pop();
			break; case '|': case '&' : case '!':
				while (!operatorS.empty() && operatorS.top() != '('&&precedence(temp) <= precedence(operatorS.top())) {
					postfix += operatorS.top();
					operatorS.pop();
				}
				operatorS.push(temp);
				break;
		}
	}
	while (!operatorS.empty()) {
		if (operatorS.top() == '(')return 1;
		postfix += operatorS.top();
		operatorS.pop();
	}
	if (postfix.empty())return 1;
	stack<bool>resultS;
	for (unsigned int a = 0; a < postfix.size(); a++) {
		char temp = postfix[a];
		switch (temp) {
		case '0': case '1':case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			resultS.push(values[temp - '0']);
			break;
		case '!': {
			if (resultS.empty())return 1;
			bool ch = resultS.top();
			resultS.pop();
			resultS.push(!ch);
			break;
		}case '|': {
			if (resultS.empty())return 1;
			bool val1 = resultS.top();
			resultS.pop();
			if (resultS.empty())return 1;
			bool val2 = resultS.top();
			resultS.pop();
			resultS.push(val1 || val2);
			break;}
		case '&': {
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
	if (resultS.size() != 1)return 1;
	result = resultS.top();
	return 0;
}

void test1() {
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false, true,  false
	};
	string pf;
	bool answer;
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("", ba, pf, answer) == 1);
	evaluate("4  |  !3 & (0&3) ", ba, pf, answer);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0&& pf == "43!03&&|" && !answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);
	cout << "Passed all tests" << endl;
}
int main(){
	test1();
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false,true,  false
	};
	string pf;
	bool result;
	assert(evaluate("1&!(9|1&1|9) | !!!(9&1&9)", ba, pf, result) == 0 && result);
}